#import "@local/folio:0.1.0": *
#show: notes.with(
  title: "Project 3 Review: ER Queue (Skew + Leftist Heaps)",
  course: "CMSC 341",
  author: "Aren Vista",
  date: "2026-11-09",
)

#show raw: set text(ligatures: false, features: (calt: 0))

#let pass = text(fill: c-ops, weight: 600)[Pass]
#let fail = text(fill: c-solve, weight: 600)[Fail]
#let partial = text(fill: c-iter, weight: 600)[Partial]
#let fixed = text(fill: c-ops, weight: 600)[Fixed]

= Summary

This is the second review. It checks the "finish er cpp" commit (`er.cpp` and
the new `mytest.cpp`). The spec page isn't in the folder, so I checked against
the spec text quoted in the function comments and against `driver.txt`.

#important(title: "Big improvement since last week")[
  These are fixed: the size counter, the `Region` array size, NPL copying,
  the `&&`/`||` merge check, and the stray `dump()`. `getPatient` and the
  tests are now written, with gaps noted below. `clear()` hasn't changed.
  `er.cpp` has three problems left, and each is a small change:
  + `clear()` still erases the hospital's settings (@sec-clear).
  + `printPatientQueue` still doesn't match the `driver.txt` format
    (@sec-output).
  + `getPatient` can change which hospital is on top when priorities tie
    (@sec-getpatient).

  In `mytest.cpp`, `isHeapOrdered` only checks the root and its two children,
  which weakens six tests (@sec-heaporder). Two required tests are still stubs.
]

What I ran:
- Compiled `er.cpp` and `mytest.cpp` with
  `g++ -Wall -Wextra -g -fsanitize=address,undefined`. *No warnings.* Last
  week's `-Wdeprecated-copy` warning is gone.
- Ran `mytest`: all 12 tests in `main` print "Passed". AddressSanitizer,
  UndefinedBehaviorSanitizer and LeakSanitizer report nothing. It takes 0.04 s.
- Ran last week's driver (`mytest.cpp` from commit `caefe56`) against the new
  `er.cpp`. It's clean under the sanitizers, and the stray `Hospital 16: => …`
  line is gone.
- Reran my probe (a friend `Tester`) on your exact file this time, with no
  patches. It walks every heap and checks heap order, the leftist property,
  the stored NPL at every node, the node count against `m_size`, and that
  patients come out in priority order.
- Large input: 100,000 leftist inserts, then a switch to skew and a switch to
  a max-heap, then 100,000 dequeues. It runs cleanly.

#remark(title: [About `driver.txt`])[
  Same as last week: the IDs here differ from the sample (the spec says they
  are machine dependent), so don't chase the numbers. The one real difference
  left is the `printPatientQueue` format.
]

== At a glance

#table(
  columns: (auto, auto, 1fr),
  align: (left, left, left),
  stroke: 0.5pt + rule-line,
  inset: 6pt,
  table.header([*Function*], [*Status*], [*Notes*]),
  [`Hospital()` / `Hospital(…)` / `~Hospital()`], pass, [],
  [`Hospital::insertPatient`], fixed, [Size counter fixed. Also returns `false` on a hospital with no priority function],
  [`Hospital::numPatients`], fixed, [Matches the real node count in every trial],
  [`Hospital::getNextPatient`], pass, [Correct order in all 4 combos, and throws `out_of_range` when empty],
  [`Hospital::mergeWithQueue`], fixed, [Throws `domain_error` on a different function, type or structure, including into an empty host],
  [`Hospital::clear`], fail, [Still erases the priority function and settings. See @sec-clear],
  [Copy constructor / `operator=`], fixed, [Deep copy, and NPLs are preserved],
  [`Hospital::setPriorityFn`], pass, [Correct rebuild that reuses the same nodes],
  [`Hospital::setStructure`], pass, [Same, and NPLs are correct after rebuild],
  [`Hospital::printPatientQueue`], fail, [Format differs from `driver.txt`. See @sec-output],
  [`Region(int)`], fixed, [Array is now `m_capacity + 1` slots],
  [`Region::addHospital` / `getHospital`], pass, [Correct order. A full region rejects the next add],
  [`Region::getNthHospital`], pass, [],
  [`Region::setPriorityFn`], fixed, [Stray `dump()` removed],
  [`Region::setStructure`], pass, [],
  [`Region::getPatient`], partial, [Correct result, but it can reorder hospitals with equal priority. See @sec-getpatient],
  [`mytest.cpp`], partial, [12 tests run and pass. The heap-order checker is shallow, and 2 tests are stubs. See @sec-tests],
)

= Remaining issues in er.cpp

== `clear`: still erases the hospital's settings <sec-clear>

`clear()` (lines 32–48) still resets `m_priorFunc`, `m_heapType`,
`m_structure`, `m_ID` and `m_regPrior`, and only when the heap had patients.
The new `nullptr` guard in `insertPatient` means this no longer crashes, but
the hospital silently stops accepting patients:

```text
probe: Hospital h(fnMin, MINHEAP, LEFTIST, 7, 123456); insert; h.clear();
       h.getPriorityFn() == fnMin            ->  false
       h.insertPatient(p)                    ->  false   (expected true)
```

`clear` is supposed to remove the patients and nothing else. *Fix:*

```cpp
void Hospital::clear() {
  clearHelper(m_heap);
  m_heap = nullptr;
  m_size = 0;
}
```

`operator=` and the destructor still work after this change: `operator=`
copies every field from `rhs` after calling `clear()`, and the destructor
only needs the nodes freed.

== `printPatientQueue` format <sec-output>

The brackets are fixed now, but the rest of the line still uses your own
format instead of the provided `operator<<`:

```text
yours:   [78] Patient ID: 278310 blood Pressure: 88 temperature: 36
sample:  [88] Patient ID: 205596, blood pressure: 99, temperature: 36
```

The commas are missing and "Pressure" is capitalized. If the grader diffs
output, this fails. *Fix* (lines 450–452):

```cpp
cout << "[" << m_priorFunc(*root) << "] " << *root << endl;
```

== `getPatient`: reorders hospitals with equal priority <sec-getpatient>

The logic is correct. It skips and discards empty hospitals, returns the
best patient, and returns `false` once the region is empty. The probe confirmed
all of that. The problem is how it does it: it removes the top hospital and
adds it back (lines 704–718). When several hospitals have the same
`m_regPrior`, a different one can end up on top:

```text
probe: 4 hospitals, all priority 5 (IDs 900001-900004), 3 patients each
       top hospital before getPatient: 900001     after: 900004
```

The spec says "the state of the rest of objects must be preserved". Removing
one patient doesn't change `m_regPrior`, so the region heap never needs
fixing. Take the patient from the root in place:

```cpp
bool Region::getPatient(Patient & patient){
  while (m_size > 0){
    if (m_heap[ROOTINDEX].numPatients() == 0){
      Hospital discard;
      getHospital(discard);      // drop the empty hospital
    } else {
      patient = m_heap[ROOTINDEX].getNextPatient();
      return true;
    }
  }
  return false;
}
```

This also avoids two deep copies of the hospital's patients on every call.

== Verified correct

- `mergeHelper`: over 300 random trials for each of the four combinations,
  there were 0 heap-order failures, 0 leftist or NPL failures, 0 cases where
  `m_size` didn't match the node count, and 0 out-of-order dequeues.
- Copy constructor and `operator=` on a leftist heap: no nodes are shared,
  every NPL is correct, and inserting 30 more patients keeps the leftist
  property. Copying or assigning from an empty hospital works.
- `setPriorityFn` and `setStructure`: over 200 trials there were 0 invariant
  failures, and the set of node addresses never changed.
- `mergeWithQueue`: a compatible merge empties `rhs`, and self-merge is a
  no-op. A different priority function or structure throws `domain_error` and
  leaves both heaps untouched. An empty host that has its own settings also
  rejects an incompatible `rhs`. One gap is still there from last week: when
  `rhs` is *empty*, the function returns at line 123 before checking
  compatibility, so it doesn't throw. The spec's wording doesn't exempt empty
  queues, so moving the compatibility check above that `return` is the safer
  choice.
- `Region`: over 200 random regions (capacity 1–30, priorities with
  duplicates), there were 0 failures in add, get, get-n-th, or adding to a full
  region. A leftist hospital that passes through a `Region` keeps its NPLs.

= mytest.cpp <sec-tests>

The `Tester` list matches the required tests from the spec comments, and most
of them are well built. Highlights:
- Unique IDs.
- Checks of node count against `m_size`.
- An NPL check that recomputes each NPL instead of trusting the stored value.
- A node-identity check in `testChangingPriorFunc`.
- A field-by-field tree comparison for the copy tests.

== `isHeapOrdered` only checks the top of the tree <sec-heaporder>

`isHeapOrdered` (lines 353–380) compares the root with its two children and
then returns `true` without recursing. A violation anywhere below the second
level is never seen:

```text
probe: MINHEAP  71 -> left 80 -> left 75     (75 under 80 is a violation)
       isHeapOrdered(root, priorityFn2, MINHEAP)  ->  true
```

Six tests rely on it: `testInsertMIN`, `testInsertMAX`,
`testChangingPriorFunc`, `testMergeOnEmptyQue`, and both normal copy tests.
*Fix:* recurse into both children before returning:

```cpp
return isHeapOrdered(root->m_left, priFn, heapType) &&
       isHeapOrdered(root->m_right, priFn, heapType);
```

== Two required tests are stubs

`testEmptyDequeue` and `testIncompatibleMerge` (lines 972–981) just
`return true`, and `main` doesn't call them. Sketches:

```cpp
bool Tester::testEmptyDequeue(){
  Hospital hospital(priorityFn2, MINHEAP, SKEW, 10, 100001);
  try {
    hospital.getNextPatient();
  } catch (out_of_range &e) {
    return true;
  }
  return false;                       // no exception thrown
}

bool Tester::testIncompatibleMerge(){
  Hospital lhs(priorityFn2, MINHEAP, LEFTIST, 10, 100001);
  Hospital rhs(priorityFn1, MAXHEAP, LEFTIST, 10, 100002);
  insertPatients(lhs, 50);
  insertPatients(rhs, 50);
  try {
    lhs.mergeWithQueue(rhs);
  } catch (domain_error &e) {
    // nothing should have moved
    return lhs.m_size == 50 && rhs.m_size == 50;
  }
  return false;
}
```

`testIncompatibleMerge` could also try a same-function, different-structure
pair (`LEFTIST` vs `SKEW`). Then add both tests to `main`.

== Smaller gaps

- *Copy tests use `SKEW`.* Every NPL in a skew heap is 0, so
  `testTreeEquality`'s NPL comparison checks nothing. Using `LEFTIST` in
  `testHospitalCCNormal` and `testHospitalAONormal` would cover last week's
  NPL bug.
- *Deep copy isn't checked directly.* Add
  `hospitalOne.m_heap != hospitalTwo.m_heap` to both normal copy tests. A
  shallow copy happens to crash at destruction today, but a test should say
  so explicitly.
- *`testChangingPriorFunc`* checks heap order after the rebuild but not the
  NPLs. The hospital is `LEFTIST`, so `testNPLVals(hospital.m_heap)` is one
  extra line.
- *`idUsed`* returns `false` when the ID *is* used. The logic works, but the
  name reads backwards. Rename it (e.g. `isUnused`) or flip it.
- *No `Region` tests.* The spec comments don't seem to require any. If you
  have time, a `getNthHospital` test and a `getPatient` test with an empty
  top hospital would be worth adding.

= Housekeeping

- *Leftover comments.* Remove the `//t`, `//Patient * tempRoot = m_heap;` and
  `//m_heap = nullptr;` lines in `setPriorityFn` and `setStructure`
  (lines 325–328, 351–353), and the commented-out block in `getNthHospital`
  (lines 610–621).
- *Typos* in comments and messages: "recursivley", "handel", "leftits",
  "recusrive", "strucutre", "hopsital", "hempty", "pazzed", "asignment", and
  the exception text "Que is empty". The test labels in `main` say "a edge
  case" and "asignment".
- *Repo.* The `driver` and `tester` binaries are committed, and `driver.cpp`
  isn't. Remove the binaries and add the provided `driver.cpp`.

= Fix checklist

Ordered by how much each one is worth.

+ `clear`: free the nodes and reset `m_heap` / `m_size` only.
+ `preOrderTraversal`: `"[" << prio << "] " << *root`.
+ `mytest.cpp`: write `testEmptyDequeue` and `testIncompatibleMerge`, and
  call them from `main`.
+ `mytest.cpp`: make `isHeapOrdered` recursive.
+ `getPatient`: take the patient from `m_heap[ROOTINDEX]` in place.
+ `mytest.cpp`: use `LEFTIST` in the normal copy tests and check that the
  root pointers differ.
+ Clean up the comments and typos.
+ Compile on GL, run `mytest` under valgrind, and submit.
