#import "@local/folio:0.1.0": *
#show: notes.with(
  title: "Project 1 Review: List of Sparse Matrices",
  course: "CMSC 341",
  author: "Aren Vista",
  date: "2026-09-28",
)

#show raw: set text(ligatures: false, features: (calt: 0))

#let pass = text(fill: c-ops, weight: 600)[Pass]
#let fail = text(fill: c-solve, weight: 600)[Fail]
#let partial = text(fill: c-iter, weight: 600)[Partial]
#let untested = text(fill: muted, weight: 600)[Untested]

= Summary

This review checks `csr.cpp` and `mytest.cpp` against the Project 1 spec. The
project is due *Tuesday Sep 29, before 9:00 pm*.

#important(title: "The sample driver does not match driver.txt")[
  Running `driver.cpp` prints `Object is not in the list!` where `driver.txt`
  expects `70`. The cause is `CSRList::insertAtHead`: on the first insert it
  returns before `m_size++`, so `m_size` is always one less than the real node
  count. The grader will see this first.
]

What I ran:
- Compiled with `g++ -Wall -Wextra -g`. The only warning is the unused
  `tester` in the provided `driver.cpp`.
- Ran `driver.cpp` and `mytest.cpp`. All 10 tests that `main` calls print
  "Passed". Several of them pass only because they don't check the broken
  behaviour (see @sec-tests).
- Wrote a separate probe program that calls each function on edge cases,
  built with `-fsanitize=address,undefined`. The results are in @sec-csr and
  @sec-list.
- *Valgrind would not start on this machine* (Arch's `ld.so` is missing debug
  info). AddressSanitizer found no leaks or memory errors in `mytest.cpp` or
  `driver.cpp` as written, but it did find leaks and crashes in the probes
  below. You still need to run valgrind on GL, because that's what gets
  graded.

`csr.h` is unchanged from the skeleton, which is required.

== At a glance

#table(
  columns: (auto, auto, 1fr),
  align: (left, left, left),
  stroke: 0.5pt + rule-line,
  inset: 6pt,
  table.header([*Function*], [*Status*], [*Issue*]),
  [`CSR()`], pass, [],
  [`~CSR()`], pass, [],
  [`CSR(const CSR&)`], fail, [`m_next` is left uninitialized when `rhs` is empty],
  [`compress`], fail, [No `clear()` first: it leaks, `m_nonzeros` keeps adding up, and `compress(0,0,…)` doesn't make the object empty],
  [`getAt`], pass, [],
  [`sparseRatio`], fail, [Empty object gives `INT_MIN`; floating-point truncation makes 29% come out as 28],
  [`CSR::operator==`], fail, [Uses `&&` where it needs `||`; misses the last `ROW_INDEX` entry; can dereference null],
  [`CSR::clear` / `empty`], pass, [],
  [`CSRList()`], pass, [],
  [`~CSRList()`], pass, [],
  [`CSRList(const CSRList&)`], fail, [`m_size` is never set],
  [`insertAtHead`], fail, [`m_size` is one too low],
  [`CSRList::operator=`], pass, [Correct, but it copies the wrong `m_size` from `rhs`],
  [`CSRList::operator==`], partial, [Correct once `m_size` is fixed],
  [`CSRList::getAt`], partial, [Correct once `m_size` is fixed],
  [`averageSparseRatio`], fail, [Multiplies by 100 a second time],
  [`CSRList::clear` / `empty`], pass, [],
)

= CSR class <sec-csr>

== Copy constructor

When `rhs` is empty, the early `return` happens before `m_next = nullptr`, so
`m_next` holds garbage. `insertAtHead` of an empty `CSR` into an empty list
keeps that garbage pointer, and `CSRList::clear()` later follows it:

```text
probe: CSRList l; CSR e; l.insertAtHead(e); l.insertAtHead(e);
ASan:  SEGV in CSRList::clear()  (address 0xbebebebebebebebe)
```

*Fix:* set `m_next = nullptr;` as the first line, before the early return.

#remark(title: [About "NOT SURE ABOUT THIS!!!"])[
  Setting `m_next = nullptr` in the copy constructor is correct. The spec says
  `CSR` must not manage `m_next`, and a copy of a node shouldn't point into the
  original's list. Remove the comment once the fix above is in.
]

== `compress`

The spec says `compress` "can be re-called on the same object" and "needs to
re-create new memory". The current code never frees the old arrays and never
resets `m_nonzeros`:

```text
probe: a.compress(2,2,{1,2,3,4},4); a.compress(2,2,{0,5,0,0},4);
out:   m_nonzeros = 5   (expected 1)
       values = 5 <garbage> <garbage> <garbage> <garbage>
ASan:  LeakSanitizer: 3 direct leaks from CSR::compress
```

A related problem: calling `compress(0,0,…)` on an object that already holds
data returns early and leaves the old matrix in place. `empty()` then returns
`false`, but the spec says a 0 × 0 matrix is an empty object.

*Fix:* call `clear();` as the first statement of `compress`. That handles both
problems.

Cases that already work:
- When `arraySize < m*n`, the rest is filled with zeros (`ROW_INDEX` is padded
  by the trailing `while` loop).
- When `arraySize > m*n`, the extra data is ignored.
- Size given with no data (`compress(2,3,nullptr,0)`) gives an all-zero
  matrix. `empty()` is `false` and `sparseRatio()` is 100.

Minor: the early return uses `m == 0 || n == 0`, which also makes `0 × 5`
empty. That's a reasonable reading of the spec. Negative `m`/`n` aren't
guarded. Treating them as empty would be the safe choice.

== `operator==`

```cpp
if (m_n != rhs.m_n && m_m != rhs.m_m && m_nonzeros != rhs.m_nonzeros)
    return false;
```

This only returns `false` when *all three* differ. It should return `false`
when *any one* differs, so use `||`. Two consequences:

```text
probe: 2x3 {1,0,0,0,0,0}  ==  3x2 {1,0,0,0,0,0}
out:   true   (expected false: different shapes)

probe: 2x2 all zeros  ==  default-constructed (empty) CSR
ASan:  SEGV (null deref of rhs.m_row_index) in CSR::operator==
```

The `ROW_INDEX` loop also runs `i < m_m`, but that array has `m_m + 1`
entries, so it needs `i <= m_m`. Once the dimension check uses `||`, the last
entry always equals `m_nonzeros`, so this is mostly harmless. It's still worth
fixing because the tests copy the same off-by-one.

== `sparseRatio`

1. *Empty object.* `numTotal` is 0, so this computes `0.0 / 0` = NaN, and
   casting NaN to `int` is undefined behaviour. The probe printed
   `-2147483648`. The spec says to return `0`. Add `if (empty()) return 0;`.
2. *Floating-point truncation.* `(double)29 / 100 * 100` is
   `28.999999999999996`, which truncates to `28`:
   ```text
   probe: 10x10 with 29 zeros -> 28 (expected 29)
          10x10 with 57 zeros -> 56 (expected 57)
   ```
   Integer arithmetic avoids the problem:
   ```cpp
   return (numZeros * 100) / numTotal;
   ```
   Integer division still truncates, as the spec wants (0.656 becomes 65).

== `getAt`, `clear`, `empty`, constructors

These are correct. `getAt` throws `runtime_error` for any out-of-range or
negative index and for an empty object. It returns 0 for zero entries.
`empty()` correctly treats an all-zero matrix (whose arrays are `new int[0]`
and non-null) as not empty.

= CSRList class <sec-list>

== `insertAtHead`: `m_size` is off by one

```cpp
if (m_head == nullptr) {
    m_head = newCSR;
    return;          // m_size++ never runs for the first node
}
```

```text
probe: insert 1 -> m_size = 0;  insert 2 -> m_size = 1
       getAt(1,1,1) on a 2-node list -> throws "Object is not in the list!"
```

This one bug breaks `getAt` (it rejects the last node, or every node in a
one-item list), `driver.cpp`, and `m_size` everywhere else. *Fix:* drop the
special case. `newCSR->m_next = m_head; m_head = newCSR; m_size++;` also works
when `m_head` is null.

#remark(title: [About "IS CSRLIST 0 INDEXED?!?!"])[
  Yes. In `driver.cpp`, `aCSR` is inserted first and `cCSR` second, so `cCSR`
  is at index 0. `getAt(1,2,4)` is expected to return `70`, which is in
  `aCSR`. So index 1 is the second node.
]

== Copy constructor

`m_size` is never assigned in either branch, so it holds whatever was in
memory:

```text
probe: 3 inserts, CSRList c(l);  ->  c.m_size = 0, l.m_size = 2, (c == l) = false
```

*Fix:* add `m_size = rhs.m_size;` at the top, and set `m_size = 0` in the
empty branch. The node-copying loop keeps the original order, which is what
the spec's note asks for.

== `averageSparseRatio`

`sparseRatio()` already returns a percentage, and this function multiplies by
100 a second time:

```text
probe: two 2x2 matrices at 75% each -> 7500   (expected 75)
```

*Fix:* `return totalRatio / count;`.

#aside(title: "Ambiguity")[
  The spec doesn't say whether to average the truncated integer percentages or
  the exact ratios, and the two can differ by 1. For example, 0.5% and 1.5%
  give (0 + 1) / 2 = 0 one way, and 1.0%, which is 1, the other.
  Averaging the `sparseRatio()` values is the natural reading of "average
  sparse ratio for all matrices", and it's what the code does once the extra `* 100` is removed. Ask on the
  course discussion board if you want to be sure.
]

== What's already correct

- `operator=`: self-assignment guard, clears old nodes, deep-copies in order,
  handles an empty `rhs`. It copies `rhs.m_size`, which becomes correct once
  `insertAtHead` is fixed.
- `operator==`: size check, then compares each node in order with
  `CSR::operator==`. It's correct once the `CSR` operator and `m_size` are
  fixed.
- `getAt`: bounds checks and the walk to the node are right. It throws on an
  empty list.
- `clear`, `empty`, default constructor, destructor.

= mytest.cpp <sec-tests>

== The spec's required tests

#table(
  columns: (1fr, auto, 1.4fr),
  stroke: 0.5pt + rule-line,
  inset: 6pt,
  table.header([*Required test*], [*Status*], [*Notes*]),
  [compress, normal case], pass, [`testCompresOne`],
  [compress, fewer values than $m times n$], pass, [`testCompresTwo`],
  [compress, 0 × 0 with data], partial, [`testCompresThree` uses a fresh object. Also call it on an object that already holds data (this would catch the missing `clear()`)],
  [CSR `==`, normal case], partial, [Only checks the equal case. Add an unequal pair, e.g. different shapes, which would catch the `&&` bug],
  [CSR `==`, both empty], pass, [`testOverloadedEqualityTwo`],
  [CSR `getAt` throws], pass, [`testGetAtOne`. Could also try negative indices],
  [List `=`, normal case], partial, [Compares `list2.m_size` to `list1.m_size`, and both are wrong in the same way. Compare to the constant `2` instead],
  [List `=`, empty into non-empty], fail, [`testOverloadedAsignmentTwo` assigns a *non-empty* list into an *empty* one, which is the reverse of what's asked],
  [List `getAt` throws on empty list], pass, [`testGetAtTwo`],
  [List `getAt`, normal case], fail, [`testGetAtThree` has no assertions and isn't called from `main`. *This is a required test.* Checking `getAt(2, …)` on a 3-node list would have caught the `m_size` bug],
)

== Other gaps

- `testDefaultConstructorTwo` is written but its call in `main` is commented
  out. `testCopyConstructorOne` just returns `true`.
- Nothing tests `sparseRatio`, `averageSparseRatio`, re-calling `compress`,
  the `CSRList` copy constructor, or `CSRList::operator==`. Each of those has
  a bug listed above, so a single test for each would have found it.
- The `ROW_INDEX` loops in the equality and assignment tests use `i < m_m`,
  which skips the last entry. They should use `i <= m_m`. The compress tests
  get this right.
- Suggest a deep-copy check: after `list2 = list1`, change or clear `list1`
  and confirm `list2` still has its values.
- The printed labels are inconsistent: "Equality … error case" is really an
  edge case, and "getAtTwo()" is a function name. That doesn't affect grading,
  but clear labels help the TA.

= Housekeeping

- *Provided code.* The spec says "provided function implementations in
  csr.cpp may not be modified." A format-on-save tool is rewriting `dump()`
  (braces and spacing only). The logic is unchanged, so the risk is low, but
  it's safest to restore those functions exactly as they were in the skeleton.
- Remove the leftover debug comments (`// cout << …`, the "NOT SURE" and
  "0 INDEXED" notes).
- Submit only `csr.h`, `csr.cpp` and `mytest.cpp`. The compiled binaries
  (`a.out`, `driver`, `mytest`, `test`) are already committed and should be
  removed from the repo. Don't commit the saved spec HTML either.

= Fix checklist

+ `CSR(const CSR&)`: set `m_next = nullptr` before the early return.
+ `compress`: call `clear()` first.
+ `CSR::operator==`: change `&&` to `||`, and make the row loop `i <= m_m`.
+ `sparseRatio`: return 0 when empty, and use `numZeros * 100 / numTotal`.
+ `insertAtHead`: always run `m_size++`.
+ `CSRList(const CSRList&)`: set `m_size`.
+ `averageSparseRatio`: remove the extra `* 100`.
+ `mytest.cpp`: finish and call `testGetAtThree`, fix
  `testOverloadedAsignmentTwo` to go the right direction, add unequal cases,
  and compare sizes to constants.
+ Rerun `driver.cpp` and diff against `driver.txt`, then run valgrind on GL.
