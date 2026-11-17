#import "@local/folio:0.1.0": *
#show: notes.with(
  title: "Project 2 Review: Drone Show (AVL + Splay)",
  course: "CMSC 341",
  author: "Aren Vista",
  date: "2026-10-20",
)

#show raw: set text(ligatures: false, features: (calt: 0))

#let pass = text(fill: c-ops, weight: 600)[Pass]
#let fail = text(fill: c-solve, weight: 600)[Fail]
#let partial = text(fill: c-iter, weight: 600)[Partial]
#let untested = text(fill: muted, weight: 600)[Untested]

= Summary

This review checks `show.h`, `show.cpp` and `mytest.cpp` against the Project 2
spec. The project is due *today, Tuesday Oct 20, before 9:00 pm*.

#important(title: "Mostly good news")[
  The AVL tree and the splay tree are both correct. The only real bugs are two
  return values in `Show`, and each is a one-line fix (see @sec-show). Fix those
  first, then do the test changes if there's time.
]

What I ran:
- Compiled with `g++ -Wall -Wextra -g`. *No warnings.*
- Ran `mytest.cpp`. All 13 tests print "Passed".
- *Valgrind works on this machine now.* `mytest` and `driver` both report
  "All heap blocks were freed" and 0 errors. AddressSanitizer is also clean.
  Still run valgrind once on GL before submitting.
- Wrote a separate probe that compares `Group` against a textbook AVL tree and
  `Show` against a textbook bottom-up splay tree. Each was run on 300 random
  trials covering inserts, duplicates, removals of present and missing IDs, and
  splays on hits and misses. *The tree shapes and heights matched every time.*
- Large input: 89,999 sorted inserts take 0.03 s (height 16), `removeDead` of
  30,000 drones takes 0.01 s, and 50,000 sorted group inserts into `Show` take
  0.01 s. Nothing overflows the stack.

`show.h` differs from the skeleton only by added private helpers, which the
spec allows. The provided `dump`, `Show::dump` and `dumpHelper` are unchanged.
`dumpTree` was split onto several lines with a comment added. The logic is the
same, but it's safest to put it back as the one-liner from the skeleton.

#remark(title: [Why `driver` doesn't match `driver.txt`])[
  Your tree is not the cause, so don't spend tonight on it. The IDs themselves
  are different (`79418, 36888, …` here vs `19224, 19289, …` in the sample). The
  spec says: _"the numbers are machine dependent, therefore, the numbers you
  see in the sample file driver.txt might be different from the numbers your
  machine generates."_ The probe above is a stronger check than the diff.
]

== At a glance

#table(
  columns: (auto, auto, 1fr),
  align: (left, left, left),
  stroke: 0.5pt + rule-line,
  inset: 6pt,
  table.header([*Function*], [*Status*], [*Issue*]),
  [`Group()` / `Group(…)` / `~Group()`], pass, [],
  [`Group::insert`], pass, [Rejects invalid and duplicate IDs; matches reference AVL],
  [`Group::remove`], pass, [Matches reference, including two-child (successor) case],
  [`Group::clear`], pass, [],
  [`Group::listDrones`], pass, [In-order, format matches `driver.txt`],
  [`Group::setState`], pass, [],
  [`Group::removeDead`], pass, [Balanced and correct, linear-time collection],
  [`Group::count` (both)], pass, [],
  [`Group::findDrone`], pass, [],
  [`Show()` / `~Show()`], pass, [],
  [`Show::insert`], pass, [Matches reference splay; bounds 10000/99999 accepted, 9999/100000 rejected. See @sec-empty],
  [`Show::count`], pass, [],
  [`Show::setState`], fail, [Returns `true` even when the drone isn't in the group],
  [`Show::removeDrone`], fail, [Returns `true` even when the drone isn't in the group],
  [`Show::getGroupHeight`], partial, [Correct, but an empty group also returns `-1`. See @sec-empty],
)

= Show class <sec-show>

== `setState`: return value is thrown away

```cpp
m_root->setState(drone, state);   // show.cpp:758
return true;
```

`Group::setState` already returns `false` when the drone isn't found, but the
result is ignored:

```text
probe: group 50000 holds drones 20000, 20001, 20002
       aShow.setState(50000, 77777, DEAD)  ->  true   (expected false)
```

*Fix:* `return m_root->setState(drone, state);`

== `removeDrone`: missing drone reports success

```cpp
m_root->remove(drone);            // show.cpp:710
return true;
```

```text
probe: aShow.removeDrone(50000, 77777, false)  ->  true   (expected false)
```

`Group::remove` is `void`, so check first:

```cpp
else{
    if (!m_root->findDrone(drone)){
        return false;
    }
    m_root->remove(drone);
    return true;
}
```

The spec says "In the error cases the function returns false" for both of
these functions, and a drone that isn't there is the obvious error case. The
grader will very likely test it.

== Empty groups <sec-empty>

A `Group` can end up with no drones in two ways: `Show::insert` with no valid
drones (population 0, or all IDs out of range), or removing the last drone.
Two things go wrong:

```text
probe: insert(30000, {Drone(), Drone()}, 2)  ->  true, group's m_root = null
probe: insert group with one drone, removeDrone that drone, dump(true)
ASan:  SEGV in Show::dumpHelper (show.cpp:651, root->m_root->m_id)
```

- `dumpHelper` is provided code and can't be changed, so the only way to avoid
  the crash is to not leave empty groups in the tree.
- `getGroupHeight` returns `height(nullptr) = -1` for an empty group, which
  looks the same as "group not found".

#aside(title: "Ambiguity")[
  The spec doesn't say what should happen here. A low-risk change is to have
  `Show::insert` delete the new group and return `false` when `newGroup->m_root`
  is `nullptr`, since "no valid drones" is a reasonable error case. What to do
  when a group loses its last drone (remove the group, or leave it) is a
  design choice. Ask on the discussion board if you want to handle it. This is
  optional; the two return-value fixes above matter more.
]

== Note: splaying on a miss

`count`, `setState`, `removeDrone` and `getGroupHeight` call `splay` before
checking whether the group exists, so a lookup for a missing group still
restructures the tree:

```text
before count(45000):  (((20000)30000)40000(50000(70000)))
after:                ((((20000)30000)40000)50000(70000))
```

That's textbook splay-tree behaviour (the last node visited is brought to the
root), and the reference implementation does the same. The spec line "If a node
is accessed the splay operation happens otherwise there is no splay" could be
read either way. I'd leave it.

= Group class (AVL)

No bugs found. Every insert and remove matched the reference tree, leaves have
height 0, and heights are correct at every node. Notes for cleanup only:

- `rebalanceTree` calls itself on the child (lines 103, 118). The child is
  already balanced, so this does nothing except update its height. It's
  harmless, but the comment "recursively balances the avl tree" is misleading.
- The double-rotation checks aren't written the same way: `< 0` on the left
  (line 106) and `== 1` on the right (line 121). Both are correct for a valid
  AVL tree. Writing `> 0` on the right would make them match.
- In the `insert` helper, the "NO CHILDREN, INSERT HERE" branches repeat the
  `root == nullptr` case. `root->setLeft(insert(root->getLeft(), drone));`
  handles both.
- Unreachable code: `return drone;` at line 134, the final
  `return rebalanceTree(root);` at line 204, and lines 320–321 in `remove`.

= mytest.cpp <sec-tests>

== Bug in the BST checkers

Both BST helpers pass `MAXID` as the upper bound for the right subtree,
instead of the bound they were given:

```cpp
isAVLStillBST(drone->m_right, drone->m_id, MAXID)            // line 279
checkSplayStillBST(group->m_right, group->m_groupID, MAXID)  // line 330
```

So a node in the right part of a *left* subtree can be bigger than its
ancestor and the check still passes:

```text
probe: 50000 -> left 30000 -> right 60000   (60000 is in the LEFT subtree of 50000)
       isAVLStillBST(root, MINID, MAXID)  ->  true   (expected false)
```

*Fix:* pass `maxId` in both places. This affects four of the required tests
(the BST-property ones), which currently can't catch most ordering mistakes.

== The spec's required tests

#table(
  columns: (1fr, auto, 1.4fr),
  stroke: 0.5pt + rule-line,
  inset: 6pt,
  table.header([*Required test*], [*Status*], [*Notes*]),
  [AVL balanced after insertions (checks heights)], pass, [`testAVLBalanceAfterInsertions`. Compares stored height against the real height at every node],
  [BST property after insertions], partial, [`testBSTPropAfterInsertions`. Weakened by the `MAXID` bug],
  [Splay on insert: new node at root, BST holds], partial, [`testSplayTreeInsertMovesRoot`. Root check is good; BST part weakened by the `MAXID` bug],
  [AVL balanced after removals], pass, [`testAVLBalanceAfterRemoval`],
  [BST property after removals], partial, [`testBSTPropAfterRemovals`. Weakened by the `MAXID` bug],
  [`Group::remove`, 50 removals], pass, [`testRemoveMultipleDrones`. Checks found before, not found after, and the final count],
  [`Group::findDrone`, 50 searches], partial, [`testFindMultipleDrones`. Only searches for IDs that are present. Add some that aren't],
  [`Show::count`: correct value and splay], pass, [`testShowCountAndState`],
  [`Show::setState`: correct value and splay], partial, [`testShowSetState` only tries a drone that exists and never checks that the state changed. A missing-drone case would have caught the bug above],
  [BST property after many splays], partial, [`testBSTPropAfterMultipleSplayOperations`. Weakened by the `MAXID` bug],
)

== Other gaps

- Nothing tests `Show::removeDrone` or `Show::getGroupHeight`. A
  missing-drone test for `removeDrone` would have caught the second bug.
- Nothing tests `Group::count(LIGHTCOLOR)` or `listDrones`.
- `testBSTAfterRemoveDead` is declared in `Tester` but never defined or
  called. Either write it or delete the declaration.
- `testAVLBalanceAfterInsertions` and `testAVLBalanceAfterRemoveDead` use raw
  `idGen` draws and then expect exactly 100 nodes, so they assume no repeated
  IDs. I checked: the first 100 draws are distinct with both the new and the
  older libstdc++ generator, so they pass on GL too. Using `fillIDArray` (as the
  other tests do) would make them safe on any machine.
- The labels in `main` say "rejects invalid IDs", but those two tests also
  check duplicates. Clear labels help the TA.

= Housekeeping

- *Provided files.* `driver.cpp` was changed (the show loop is 50 × 50
  instead of 10 × 1) and `driver.txt` has a stray `b` at the start. Neither is
  submitted, but restore both from the course site so the comparison is honest.
- *Coding standards.* The spec grades comments and indentation. Remove the
  leftover comment `//im lost not sure what to do here` (line 139), the
  commented-out `cout`s and `else` (lines 36–38), the old insert attempt in
  `/* … */` (lines 187–196), and the stale `// updateHeight(root);` /
  `// return rebalanceTree(drone);` lines. Fix the typos in comments
  ("handel", "recurisve", "obhect"). The `remove` helper's pseudocode comment
  (lines 246–261) can go too.
- *Repo.* The binaries `driver`, `test` and `tester` are committed, plus a
  stray binary that is deleted in the working tree but not yet committed.
  Remove them from the repo. `recursionExample.cpp` is fine to keep locally
  but isn't part of the submission.
- Submit only `show.h`, `show.cpp` and `mytest.cpp`:
  `cp show.h show.cpp mytest.cpp ~/cs341proj/proj2/`

= Fix checklist

Ordered by how much each one is worth tonight.

+ `Show::setState`: `return m_root->setState(drone, state);`
+ `Show::removeDrone`: return `false` if `!m_root->findDrone(drone)`.
+ `mytest.cpp`: change `MAXID` to `maxId` in `isAVLStillBST` and
  `checkSplayStillBST`.
+ `mytest.cpp`: add missing-drone cases for `Show::setState` and
  `Show::removeDrone`, and check that the state really changed.
+ Remove the leftover comments and dead code listed above.
+ Optional: reject groups with no valid drones in `Show::insert`.
+ Compile on GL, run `mytest` under valgrind, and submit the three files.
