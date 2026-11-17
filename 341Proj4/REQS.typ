#import "@local/folio:0.1.0": *

#show: notes.with(
  title: "CarDB: Requirements, Clarified",
  subtitle: "Project 4: Car Sales App",
  course: "CMSC 341 · Fall 2026",
  author: "Aren Vista",
  date: "Due Tue Dec 1, 2026, before 9:00 pm",
)

// =========================================================================
//  Local drawing helpers (canvas units are points; y grows downward)
// =========================================================================

#let c-live = c-ops
#let c-dead = c-solve
#let c-probe = c-core

#let state-fill = (
  empty: surface,
  live: c-live.lighten(80%),
  dead: c-dead.lighten(84%),
  free: c-live.lighten(52%),
  scan: c-probe.lighten(70%),
  moved: surface-2,
)

// Label centred on p. Shadows folio's `flab`, whose zero-width box wraps
// multi-word labels one word per line.
#let flab(p, body, col: ink, sz: 9.5pt, w: 240) = place(
  dx: (p.at(0) - w / 2) * 1pt,
  dy: p.at(1) * 1pt - 20pt,
  box(width: w * 1pt, height: 40pt, align(center + horizon, text(size: sz, fill: col, font: font-sans, body))),
)

// Left-aligned text whose top-left corner sits at p.
#let ftxt(p, body, col: ink, sz: 8pt, font: font-sans, weight: "regular") = place(
  dx: p.at(0) * 1pt,
  dy: p.at(1) * 1pt,
  text(size: sz, fill: col, font: font, weight: weight, body),
)

// A rectangle at (x, y), w × h, with centred content.
#let fbox(
  x, y, w, h, body,
  fill: surface, stroke: rule-line, radius: 0pt, dash: none, col: ink-soft, sz: 7.5pt,
) = place(dx: x * 1pt, dy: y * 1pt, rect(
  width: w * 1pt,
  height: h * 1pt,
  inset: 3pt,
  radius: radius,
  fill: fill,
  stroke: (paint: stroke, thickness: 0.6pt, dash: dash),
  align(center + horizon, text(size: sz, fill: col, font: font-sans, body)),
))

// A rounded, tinted box for flow and state diagrams.
#let fnode(x, y, w, h, body, hue: c-define, dash: none) = fbox(
  x, y, w, h, body,
  fill: hue.lighten(88%), stroke: hue.transparentize(35%), radius: 3pt,
  dash: dash, col: ink,
)

// A row of buckets. `cells` holds (label, state) pairs; indices print below.
#let fstrip(x, y, cells, w: 26, h: 20, first: 0, idx: true) = {
  for (j, c) in cells.enumerate() {
    let (lab, st) = c
    fbox(x + j * w, y, w, h, lab, fill: state-fill.at(st), stroke: muted.lighten(30%))
    if idx {
      flab(
        (x + j * w + w / 2, y + h + 7),
        text(font: font-mono, size: 6.3pt, fill: muted)[#(first + j)],
      )
    }
  }
}

// Centre of bucket j in a strip drawn at (x, y).
#let bc(x, y, j, w: 26) = (x + j * w + w / 2, y)

// A quadratic arc from p to q, bowed `lift` points upward (negative: downward).
#let farc(p, q, lift: 16, col: c-probe, th: 0.8pt, dash: none) = {
  let c = ((p.at(0) + q.at(0)) / 2, (p.at(1) + q.at(1)) / 2 - lift)
  place(curve(
    stroke: (paint: col, thickness: th, dash: dash),
    curve.move((p.at(0) * 1pt, p.at(1) * 1pt)),
    curve.quad((c.at(0) * 1pt, c.at(1) * 1pt), (q.at(0) * 1pt, q.at(1) * 1pt)),
  ))
  fhead(q.at(0), q.at(1), calc.atan2(q.at(0) - c.at(0), q.at(1) - c.at(1)), col: col, s: 5.5)
}

// A labelled colour swatch, for figure keys.
#let fkey(p, st, body) = {
  fbox(p.at(0), p.at(1), 12, 9, none, fill: state-fill.at(st), stroke: muted.lighten(30%))
  ftxt((p.at(0) + 16, p.at(1)), body, sz: 7pt, col: ink-soft)
}

// One method's contract: signature, one-line intent, then labelled rows.
#let method(sig, intent, ..rows) = block(
  width: 100%,
  fill: c-define.lighten(93%),
  stroke: 0.6pt + c-define.transparentize(55%),
  radius: 3pt,
  inset: (x: 12pt, y: 10pt),
  above: 1.3em,
  below: 1.3em,
  breakable: false,
  {
    set par(justify: false)
    text(font: font-mono, size: 8.6pt, weight: "bold", fill: c-define.darken(30%), sig)
    v(0.35em)
    text(weight: "bold", fill: ink, intent)
    v(0.2em)
    grid(
      columns: (6.4em, 1fr),
      column-gutter: 10pt,
      row-gutter: 0.75em,
      ..rows.pos().map(((k, v)) => (pad(top: 2.2pt, eyebrow(k, col: c-define.darken(20%))), v)).flatten(),
    )
  },
)

#let decide(body) = notebox("Decide & document", body, hue: c-unsure, breakable: true)

// =========================================================================

#block(width: 100%, below: 1.6em, {
  set text(size: 9.5pt, fill: ink-soft)
  set par(justify: false)
  [A restatement of `REQS.txt`. The method specifications are rewritten as intent,
  scope, and contract, and the design goals behind them are drawn as diagrams. Where
  the original is silent or contradicts itself, the gap is collected in a yellow
  _Decide & document_ box rather than resolved silently.]
})

= What you are building, and why

A car manufacturer's delivery agents carry a mobile app whose local database is
kept in sync with the sales department. Agents use it to *insert*, *remove*,
*update* and *search* delivery records, and every one of those operations must run
in $O(1)$ time. Your job is the database: a hash table class named `CarDB`, plus a
test suite that proves it works.

A record is a `Car`. Its *model* string (`"stratos"`, `"gt500"`, …) is the hash
key; its *dealer* number is an ID. The model and the dealer *together* identify a
record, so the table can hold many `stratos` records as long as their dealers
differ.

#key[
  Every rule in this project serves one of five design goals:
  + *Constant-time operations.* Keep the table less than half full, so probe chains stay short.
  + *Deletion without breaking searches.* Removing a record must not hide the records stored after it in a probe chain. This is why deletion is lazy.
  + *Cleanup of dead buckets.* Lazily deleted buckets accumulate; when they dominate, rebuild the table without them.
  + *No long pauses.* Rebuilding a table is $O(n)$. Spread that cost over four operations so no single insert or remove stalls the app.
  + *Pluggable behaviour.* The hash function is a function pointer, and each table carries its own collision policy, which can change at run time.
]

*Objectives.* Implement a hash table; resolve collisions with linear, quadratic and
double-hash probing; implement incremental rehashing; use function pointers; write
unit tests; work in Linux; read and analyse a requirements document.

*Files provided.* `dealer.h` (the `Car` and `CarDB` declarations), `dealer.cpp`
(complete `CarDB` here), `driver.cpp` (a sample driver, which also holds the
`Random` class), `driver.txt` (that driver's output).

*Files you submit.* `dealer.h`, `dealer.cpp`, `mytest.cpp`.

= Background: hashing and probing

A hash table implements the map ADT. A hash function turns the key into a number
$H$; reducing it modulo the table size $T$ gives the key's *home bucket*
$h = H mod T$. When the home bucket is taken, a *collision policy* generates a
sequence of further buckets to try. The $i$-th probe, for $i = 0, 1, 2, dots$, is

#callout("Formula", hue: c-formula, glyph: [∮], title: "The three probe sequences", breakable: false)[
  $
    "linear:"      && "index"_i & = (h + i) mod T \
    "quadratic:"   && "index"_i & = (h + i^2) mod T \
    "double hash:" && "index"_i & = (h + i dot (11 - (H mod 11))) mod T
  $
  Probe $i = 0$ is always the home bucket. Each table uses the policy stored with
  it: `m_currProbing` for the current table, `m_oldProbing` for the old one.
]

#figframe[Each row assumes the first three buckets probed are taken (green) and
  shows where the fourth probe lands (bright green). The cell label is the probe
  number $i$. $T = 13$, home bucket $h = 3$; for double hashing $H = 29$, so the step is
  $11 - (29 mod 11) = 4$ and the sequence wraps past the end of the table.][
  #fcanvas(460, 250, {
    let x0 = 45
    let w = 30
    let row(y, label, formula, probes, wrap: false) = {
      ftxt((0, y), label, weight: "bold", sz: 8pt)
      ftxt((x0, y), formula, sz: 8pt, col: ink-soft)
      let cells = range(13).map(k => {
        let j = probes.position(p => p == k)
        if j == none { (none, "empty") } else if j < 3 { ([#j], "live") } else { ([#j], "free") }
      })
      fstrip(x0, y + 34, cells, w: w)
      for n in range(3) {
        let a = bc(x0, y + 34, probes.at(n), w: w)
        let b = bc(x0, y + 34, probes.at(n + 1), w: w)
        let backward = b.at(0) < a.at(0)
        farc(a, b, lift: if backward { 24 } else { 8 + 1.2 * calc.abs(probes.at(n + 1) - probes.at(n)) },
          dash: if backward { "dashed" } else { none })
      }
    }
    row(0, [linear], $(h + i) mod T$, (3, 4, 5, 6))
    row(78, [quadratic], $(h + i^2) mod T$, (3, 4, 7, 12))
    row(156, [double], $(h + i dot 4) mod T$ + [ (dashed arc: wraps from 11 to 2)], (3, 7, 11, 2))
  })
]

#idea[
  *Prime table sizes* and *$lambda <= 0.5$* work together. With a prime $T$, the
  first $ceil(T slash 2)$ quadratic probes all land in different buckets, so a table
  that is at most half full always has a free bucket within reach. Linear probing
  forms long runs of occupied buckets (_primary clustering_), and quadratic and
  double-hash probing are there to break them up. The double-hash step
  $11 - (H mod 11)$ lies between 1 and 11 and is never 0, so the sequence always
  moves.
]

#remark(title: "The key is the model alone, so collisions are the norm")[
  Only `m_model` is hashed, so every `Car` with the same model has the same home
  bucket. The sample driver draws from five models, so its 49 records fall into just five probe
  chains. Under double hashing with $T = 101$, `hashCode("stratos")` $= 2873477648$,
  giving $h = 75$ and step $3$. That is why `driver.txt` shows `stratos` at buckets
  75, 78, 81, 84, 87, 90, 96, then wrapping to 1, 7, 13, … (93 and 99 were already
  taken by `x101`).
  *For testing:* "non-colliding keys" means different model strings whose
  hashes reduce to different buckets. The five-model array is a built-in source of
  colliding keys.
]

= Data model

== The two tables

`CarDB` owns a hash-function pointer and up to two tables. Each table is a
dynamically allocated array of `Car*`, so a bucket is either `nullptr` or points at a
heap-allocated `Car`. Each table has its own capacity, its own occupied and deleted
counts, and its own probing policy. The old table exists only while a rehash is
moving records out of it.

#figframe[Memory layout of `CarDB`. Bucket colours: white is `nullptr`, green is a
  live `Car`, red is a lazily deleted `Car`. The policy fields travel with their
  table, so the old table keeps probing the way it was built while it is drained.][
  #fcanvas(460, 230, {
    fbox(0, 0, 175, 208, none, fill: c-define.lighten(93%), stroke: c-define.transparentize(45%), radius: 3pt)
    ftxt((10, 8), [`CarDB`], sz: 9pt, weight: "bold", col: c-define.darken(30%))
    let f(y, body) = ftxt((10, y), body, sz: 7.5pt, font: font-mono, col: ink)
    f(30, [m_hash])
    f(44, [m_newPolicy])
    ftxt((10, 64), eyebrow[current table], sz: 7pt)
    f(78, [m_currentTable])
    f(92, [m_currentCap, m_currentSize])
    f(106, [m_currNumDeleted, m_currProbing])
    ftxt((10, 128), eyebrow[old table], sz: 7pt)
    f(142, [m_oldTable])
    f(156, [m_oldCap, m_oldSize])
    f(170, [m_oldNumDeleted, m_oldProbing])
    f(190, [m_transferIndex])

    fnode(235, 22, 205, 22, [`unsigned hashCode(string)` (any function of this type)], hue: c-decomp)
    fvec((75, 34), (235, 33), col: c-decomp)
    fseg((72, 49), (180, 55), col: muted, th: 0.4pt, dash: "dotted")
    ftxt((182, 50), [pending policy: applied at the next rehash], sz: 6.8pt, col: muted)

    let cur = (("", "empty"), ("L", "live"), ("", "empty"), ("L", "live"), ("D", "dead"), ("", "empty"), ("L", "live"), ("…", "empty"))
    ftxt((235, 64), [`Car*[m_currentCap]`: inserts always go here], sz: 7pt, col: ink-soft)
    fstrip(235, 76, cur, w: 25, idx: false)
    fvec((100, 82), (235, 86), col: c-define)
    fvec((272, 96), (272, 110), col: ink-soft, s: 5)
    fnode(235, 110, 205, 20, [`Car { m_model, m_quantity, m_dealer, m_used }`], hue: c-aside)

    let old = (("L", "live"), ("", "empty"), ("D", "dead"), ("L", "live"), ("", "empty"), ("L", "live"), ("", "empty"), ("…", "empty"))
    ftxt((235, 138), [`Car*[m_oldCap]`: `nullptr` unless a rehash is in progress], sz: 7pt, col: ink-soft)
    fstrip(235, 150, old, w: 25, idx: false)
    fvec((85, 146), (235, 160), col: c-define)
    ftxt((235, 178), [drained ¼ per operation, then deleted], sz: 6.8pt, col: muted)
    fseg((90, 195), (180, 199), col: muted, th: 0.4pt, dash: "dotted")
    ftxt((182, 195), [tracks how far the drain has reached], sz: 6.8pt, col: muted)
  })
]

The `Car` class is given. `operator==` compares *model and dealer only*, so two
`Car`s with different quantities but the same model and dealer count as the same record.
`m_used` is the lazy-deletion flag: `true` means the bucket holds live data and must
not be overwritten; `false` means the record was removed and the bucket may be reused.

== Bucket states and lazy deletion

A bucket is in one of three states. Searching and inserting read those states
differently, and that asymmetry is what lazy deletion means:

- *When inserting, treat a deleted bucket as empty.* It can be reused.
- *When searching, treat a deleted bucket as occupied.* Keep probing past it.

#figframe[Life cycle of one bucket. In `driver.txt` a removed record still prints its
  data with flag `0` (for example `x101 (980680, 0)`): removal clears `m_used` and
  leaves the record in place.][
  #fcanvas(460, 190, {
    let bw = 112
    let st(x, hue, name, rep, srch) = {
      fnode(x, 50, bw, 62, none, hue: hue)
      flab((x + bw / 2, 62), text(weight: "bold", size: 8.5pt, name))
      flab((x + bw / 2, 79), text(size: 7pt, font: font-mono, fill: ink-soft, rep))
      flab((x + bw / 2, 98), text(size: 6.8pt, fill: muted, srch))
    }
    st(0, c-aside, [EMPTY], [nullptr], [search: stop, not found])
    st(164, c-live, [LIVE], [m_used == true], [search: compare record])
    st(328, c-dead, [DELETED], [m_used == false], [search: skip, keep probing])

    fvec((112, 74), (164, 74), col: c-live)
    flab((138, 66), text(size: 7pt)[insert])
    fvec((276, 74), (328, 74), col: c-dead)
    flab((302, 66), text(size: 7pt)[remove])
    farc((384, 50), (220, 50), lift: 34, col: c-live)
    flab((302, 18), text(size: 7pt)[insert may reuse the bucket])

    fvec((220, 112), (220, 146), col: c-probe)
    flab((220, 160), text(size: 7pt, fill: c-probe.darken(10%))[rehash: copied to the new table])
    fvec((384, 112), (384, 146), col: muted, s: 5)
    flab((384, 160), text(size: 7pt, fill: muted)[rehash: dropped, never copied])
  })
]

#figframe[Why a removed bucket cannot simply be emptied. Records A, B and C all hash
  to bucket 2 and sit at 2, 3, 4 under linear probing. Then B is removed.][
  #fcanvas(460, 190, {
    let x0 = 140
    let w = 34
    ftxt((0, 30), [*Lazy delete* (correct)], sz: 8pt)
    ftxt((0, 42), [bucket 3 is tagged], sz: 7pt, col: muted)
    let top = ((none, "empty"), (none, "empty"), ([A], "live"), ([B], "dead"),
      ([C], "live"), (none, "empty"), (none, "empty"), (none, "empty"))
    fstrip(x0, 30, top, w: w)
    farc(bc(x0, 30, 2, w: w), bc(x0, 30, 3, w: w), lift: 10)
    farc(bc(x0, 30, 3, w: w), bc(x0, 30, 4, w: w), lift: 10)
    flab((bc(x0, 30, 4, w: w).at(0) + 22, 22), text(fill: c-live.darken(20%), weight: "bold")[✓])
    ftxt((x0, 66), [`getCar(C)`: 2 is not C, 3 is deleted so keep going, 4 is C. Found.], sz: 7pt, col: ink-soft)

    ftxt((0, 112), [*Emptying* (wrong)], sz: 8pt)
    ftxt((0, 124), [bucket 3 set to `nullptr`], sz: 7pt, col: muted)
    let bot = ((none, "empty"), (none, "empty"), ([A], "live"), (none, "empty"),
      ([C], "live"), (none, "empty"), (none, "empty"), (none, "empty"))
    fstrip(x0, 112, bot, w: w)
    farc(bc(x0, 112, 2, w: w), bc(x0, 112, 3, w: w), lift: 10, col: c-dead)
    flab((bc(x0, 112, 3, w: w).at(0), 100), text(fill: c-dead, weight: "bold")[✗])
    ftxt((x0, 148), [`getCar(C)`: 2 is not C, 3 is empty so stop. C is still stored but can no longer be found.], sz: 7pt, col: c-dead.darken(10%))
  })
]

= Rehashing

== When a rehash starts

#definition(title: "Load factor and deleted ratio (current table)")[
  $
    lambda = "m_currentSize" / "m_currentCap"
    quad quad
    "deletedRatio" = "m_currNumDeleted" / "m_currentSize"
  $
  `m_currentSize` counts _occupied_ buckets, live *and* deleted. The number of live
  records is $"m_currentSize" - "m_currNumDeleted"$.
]

- *After an insertion*, if $lambda > 0.5$, start a rehash. _(Goal 1: short chains.)_
- *After a removal*, if $"deletedRatio" > 0.8$, start a rehash. _(Goal 3: purge dead buckets.)_
- The new table's capacity is
  $"findNextPrime"(4 dot ("m_currentSize" - "m_currNumDeleted"))$:
  the smallest prime greater than four times the live count, clamped to
  $["MINPRIME", "MAXPRIME"]$. Right after a rehash, the new table is therefore
  roughly a quarter full.
- *Starting* a rehash means: allocate the new table with policy `m_newPolicy`; the
  current table, with its counters and policy, becomes the old table; the new table
  becomes the current table; then scan the first quarter of the old table.

#figframe[What every `insert` and `remove` does after its own work. This ordering is
  the recommended reading; the dashed box is the step the spec leaves open (see
  the _Decide & document_ list at the end of this section).][
  #fcanvas(460, 300, {
    fnode(20, 0, 150, 26, [`insert(car)`: place in current table], hue: c-aside)
    fnode(270, 0, 150, 26, [`remove(car)`: tag in either table], hue: c-aside)
    fvec((95, 26), (190, 52), col: ink-soft, s: 5)
    fvec((345, 26), (250, 52), col: ink-soft, s: 5)

    fnode(140, 52, 160, 34, [Is a rehash already in progress? \ (`m_oldTable != nullptr`)], hue: c-prompt)
    fvec((140, 69), (80, 106), col: ink-soft, s: 5)
    flab((96, 80), text(size: 7pt, weight: "bold")[yes])
    fvec((300, 69), (360, 106), col: ink-soft, s: 5)
    flab((344, 80), text(size: 7pt, weight: "bold")[no])

    fnode(0, 106, 175, 66, [*Advance the transfer.* \ Scan the next ¼ of the old table; move each live `Car` into the current table. If the scan reached the end, delete the old table and set it to `nullptr`.], hue: c-probe)

    fnode(245, 106, 195, 52, [*Check the trigger.* \ after insert: $lambda > 0.5$? \ after remove: deletedRatio $> 0.8$?], hue: c-prompt, dash: "dashed")
    fvec((342, 158), (342, 190), col: ink-soft, s: 5)
    flab((354, 174), text(size: 7pt, weight: "bold")[yes])
    flab((300, 174), text(size: 7pt, fill: muted)[no: done])

    fnode(225, 190, 215, 92, [*Start the rehash.* \ cap′ = findNextPrime(4 · (size − deleted)) \ current table → old table \ new table(cap′, `m_newPolicy`) → current table \ scan the first ¼ of the old table], hue: c-solve)
  })
]

== Incremental transfer

Rebuilding the whole table in one call would make that one insert or remove
$O(n)$. Instead the work is spread over consecutive operations, a quarter of the old
table at a time:

#figframe[A drain of an old table with capacity 101. Each operation scans
  $floor(101 slash 4) = 25$ buckets of the old table (blue) and moves the live
  records it finds into the current table. Bucket 100 is left over after four
  quarters; see the _Decide & document_ list at the end of this section.][
  #fcanvas(460, 210, {
    let heads = ([op $k$ \ triggers], [op $k+1$], [op $k+2$], [op $k+3$], [after op $k+3$])
    let spans = ([0–24], [25–49], [50–74], [75–100])
    for j in range(5) {
      let x = j * 90
      flab((x + 40, 10), text(size: 7.5pt, weight: "bold", heads.at(j)))
      if j < 4 {
        for q in range(4) {
          let st = if q < j { "moved" } else if q == j { "scan" } else { "live" }
          fbox(x + 4, 32 + q * 25, 26, 25, if q == j [Q#(q + 1)] else [], fill: state-fill.at(st), stroke: muted.lighten(30%))
        }
        fvec((x + 30, 32 + j * 25 + 12), (x + 50, 32 + j * 25 + 12), col: c-probe, s: 5)
        flab((x + 17, 140), text(size: 6.8pt, fill: muted)[old])
        flab((x + 17, 152), text(size: 6.5pt, font: font-mono, fill: muted, spans.at(j)))
      } else {
        fbox(x + 4, 32, 26, 100, [freed], dash: "dashed", stroke: muted, col: muted, sz: 6.5pt)
        flab((x + 17, 140), text(size: 6.8pt, fill: muted)[`nullptr`])
      }
      fbox(x + 50, 32, 26, 100, none, stroke: muted.lighten(30%))
      let lvl = calc.min(j + 1, 4) * 10
      fbox(x + 50, 132 - lvl, 26, lvl, none, fill: state-fill.live, stroke: muted.lighten(30%))
      flab((x + 63, 140), text(size: 6.8pt, fill: muted)[current])
    }
    fkey((0, 172), "scan", [scanned by this operation])
    fkey((150, 172), "moved", [already scanned (vacated)])
    fkey((310, 172), "live", [live data])
    ftxt((0, 190), [The trigger operation does Q1 itself, so the old table is gone after four operations in total. Deleted buckets are never copied.], sz: 7pt, col: ink-soft)
  })
]

The rules that follow from this picture:

- *Inserts go only into the current table.* Searches (`getCar`, `updateDealer`) and
  `remove` look in the current table *and* in the old table if it exists.
- *Deleted buckets are not transferred.* A rehash is the moment they leave the system
  for good.
- *A quarter is* $floor("m_oldCap" slash 4)$ *buckets.* `m_transferIndex` records where
  the next scan starts.
- *When the scan finishes*, the old table has no live data left. Free each `Car`, free
  the array, and set `m_oldTable` to `nullptr`.
- *A moved record's old bucket must still look occupied.* Probe chains run across
  quarter boundaries, so a bucket that has been scanned can lie in the middle of the
  chain of a record that has not been moved yet. If the bucket were emptied, searches
  in the old table would stop there, which is the same failure as in the emptying
  figure above:

#figframe[Old table, capacity 12, linear probing, after the first quarter (buckets
  0–2) has been moved. A and B both hash to 2; B sits at 3, in the second quarter, and
  has not been moved yet. Because bucket 2 still reads as occupied, the probe continues
  to bucket 3 and finds B.][
  #fcanvas(460, 110, {
    let x0 = 60
    let w = 30
    let cells = ((none, "empty"), (none, "empty"), ([A], "dead"), ([B], "live"), (none, "empty"), ([E], "live"),
      (none, "empty"), ([F], "live"), (none, "empty"), (none, "empty"), ([G], "live"), (none, "empty"))
    fstrip(x0, 40, cells, w: w)
    for (q, lab) in ((0, [Q1 · moved]), (1, [Q2]), (2, [Q3]), (3, [Q4])) {
      let a = x0 + q * 3 * w + 3
      let b = x0 + (q + 1) * 3 * w - 3
      fseg((a, 22), (b, 22), col: if q == 0 { muted } else { c-probe }, th: 0.6pt)
      fseg((a, 22), (a, 27), col: muted, th: 0.6pt)
      fseg((b, 22), (b, 27), col: muted, th: 0.6pt)
      flab(((a + b) / 2, 13), text(size: 7pt, fill: ink-soft, lab))
    }
    farc(bc(x0, 40, 2, w: w), bc(x0, 40, 3, w: w), lift: 9)
    ftxt((x0, 76), [`getCar(B)` in the old table: bucket 2 is A's vacated bucket, still occupied, so keep probing; bucket 3 is B. Found.], sz: 7pt, col: ink-soft)
    ftxt((x0, 90), [If bucket 2 had been set to `nullptr`, the search would stop at 2 and miss B.], sz: 7pt, col: c-dead.darken(10%))
  })
]

== Changing the collision policy at run time

`changeProbPolicy` only *records* a request in `m_newPolicy`. Nothing about the
existing tables changes. The request is honoured the next time a rehash *starts*,
when the new table is built with it. While the old table is drained it keeps probing
with its own policy, because its records were placed using that policy and can only
be found by retracing it.

#figframe[The policy is a property of a table, not of the database. Table 1 keeps
  QUADRATIC for its whole life, including the drain; only Table 2 uses LINEAR.][
  #fcanvas(460, 150, {
    fseg((0, 112), (440, 112), col: ink-soft)
    fhead(440, 112, 0deg, col: ink-soft, s: 6)
    flab((430, 124), text(size: 7pt, fill: muted)[time])
    for (x, lab) in ((120, [`changeProbPolicy(LINEAR)` \ only `m_newPolicy` changes]), (235, [rehash starts]), (355, [drain done, \ old table freed])) {
      fseg((x, 14), (x, 112), col: muted, dash: "dashed", th: 0.5pt)
      flab((x, 132), text(size: 6.8pt, fill: ink-soft, lab))
    }
    fbox(0, 28, 235, 24, [Table 1 · QUADRATIC · current], fill: c-core.lighten(80%), stroke: c-core.transparentize(40%))
    fbox(235, 28, 120, 24, [Table 1 · QUADRATIC · old], fill: c-core.lighten(92%), stroke: c-core.transparentize(40%), dash: "dashed")
    fbox(235, 66, 205, 24, [Table 2 · LINEAR · current], fill: c-ops.lighten(80%), stroke: c-ops.transparentize(40%))
  })
]

== Questions the spec leaves open

#decide[
  + *The leftover bucket.* Four floor-quarters of 101 cover buckets 0–99, leaving
    bucket 100. _Suggested:_ the fourth pass runs to the end of the table, so a drain
    always takes exactly four operations. The alternative is a fifth, short pass.
  + *A trigger during a drain.* With only two table pointers there is no room for a
    third table. _Suggested:_ skip the trigger check while a drain is in progress, as
    in the flow figure. If you do check, finish draining the old table completely
    before starting the new rehash.
  + *Failed operations.* Does an `insert` or `remove` that returns `false` advance
    the drain? The spec counts "operations", not successes. Pick one behaviour and
    state it in a comment.
  + *Which table's ratio after removing from the old table?* `deletedRatio()` is
    defined on the current table, so the trigger looks at the current table even when
    the removed record was in the old one.
  + *Tombstone representation.* `dealer.h` defines a `DELETED` macro, but
    `driver.txt` shows removed records keeping their data with `m_used = 0`. Either
    is acceptable as long as searches skip the bucket and inserts may reuse it; your
    `dump()` output will differ accordingly.
]

= Method specifications

Each block gives the method's intent in one sentence, the tables it reads and writes,
its contract, and what else it has to do. "Equal" always means `operator==`: same
model *and* same dealer.

#method(
  "CarDB(int size, hash_fn hash, prob_t probing = DEFPOLCY)",
  [Create an empty database whose current table has a legal prime capacity.],
  ("Capacity", [
    #table(
      columns: (1fr, 1fr),
      [`size` is …], [capacity becomes …],
      [below `MINPRIME` (101)], [`MINPRIME`],
      [above `MAXPRIME` (99991)], [`MAXPRIME`],
      [in range and prime], [`size` itself],
      [in range, not prime], [the smallest prime greater than `size`],
    )
  ]),
  ("Afterwards", [The current table is `new Car*[cap]` with every bucket `nullptr`; its
    size and deleted count are 0; `m_currProbing = probing`; `m_hash = hash`. No change
    has been requested yet, so `m_newPolicy = probing`. There is no old table
    (`m_oldTable = nullptr`, its counters 0), and `m_transferIndex = 0`.]),
  ("Watch for", [The default `= DEFPOLCY` is written on the *definition* in
    `dealer.cpp`, not on the declaration in `dealer.h`. `mytest.cpp` sees only the
    header, so pass all three arguments there. Also, the provided `findNextPrime(101)`
    returns 103, so check `isPrime` before calling it, or a prime `size` gets
    bumped.]),
)

#figframe[How the constructor turns a requested size into a capacity (not to scale).
  Only the green band is legal.][
  #fcanvas(460, 120, {
    let yl = 70
    fbox(110, yl - 6, 240, 12, none, fill: state-fill.live, stroke: c-live.transparentize(50%))
    fseg((10, yl), (440, yl), col: ink-soft)
    fhead(440, yl, 0deg, col: ink-soft, s: 6)
    for (x, lab) in ((110, [`MINPRIME` 101]), (350, [`MAXPRIME` 99991])) {
      fseg((x, yl - 9), (x, yl + 9), col: ink, th: 0.9pt)
      flab((x, yl + 20), text(size: 7pt, lab))
    }
    let move(a, b, lab, txt, lift: 26) = {
      fdot((a, yl), col: muted, r: 2.4)
      fdot((b, yl), col: c-live.darken(20%), r: 2.6)
      if a != b { farc((a, yl - 3), (b, yl - 3), lift: lift, col: c-live.darken(10%)) }
      flab((a, yl + 20), text(size: 7pt, font: font-mono, fill: muted, lab))
      flab(((a + b) / 2, yl - lift - 12), text(size: 7pt, fill: ink-soft, txt))
    }
    move(45, 110, [50], [clamp up])
    move(180, 192, [150], [next prime: 151], lift: 18)
    move(260, 260, [211], [], lift: 0)
    flab((260, yl - 16), text(size: 7pt, fill: ink-soft)[prime: kept])
    move(415, 350, [200000], [clamp down])
  })
]

#method(
  "~CarDB()",
  [Free every `Car` and both bucket arrays, even if a drain is in progress.],
  ("Acts on", [Both tables. Each bucket is a `Car*`, so delete every non-null
    pointer and then the array itself. The old table may still exist if the object
    is destroyed mid-drain.]),
  ("Checked by", [`valgrind`: no leaks, no invalid reads or writes.]),
)

#method(
  "bool insert(Car car)",
  [Add one new record to the current table, then let the rehash machinery take its step.],
  ("Rejects", [Returns `false`, changing nothing, when the dealer ID is outside
    $["MINID", "MAXID"] = [100000, 999999]$, or when an equal `Car` is already live in
    *either* table. While a drain is in progress the duplicate may still be in the old
    table.]),
  ("Placement", [Home bucket `m_hash(model) % m_currentCap`, then follow
    `m_currProbing`. The record goes into the first bucket that is empty *or deleted*.
    The duplicate search, however, must keep probing past deleted buckets until it
    reaches a truly empty one. Store a heap copy with `m_used = true`.]),
  ("Counters", [Filling an empty bucket: `m_currentSize + 1`. Reusing a deleted
    bucket: `m_currNumDeleted − 1`, and the size is unchanged because the bucket was
    already counted as occupied.]),
  ("Afterwards", [Advance an in-progress drain, or check $lambda > 0.5$ and start
    a rehash (see the flow figure).]),
  ("Returns", [`true` if and only if the record was stored.]),
)

#method(
  "bool remove(Car car)",
  [Lazily delete the live record equal to `car`, from whichever table holds it.],
  ("Acts on", [Search the current table with `m_currProbing`; if the record is not
    there and an old table exists, search the old table with `m_oldProbing`.]),
  ("Effect", [Set the bucket's `m_used` to `false` and keep the bucket occupied. Add 1
    to *that table's* deleted count; its size is unchanged. Never set the bucket to
    `nullptr` (see the emptying figure).]),
  ("Afterwards", [Advance an in-progress drain, or check $"deletedRatio" > 0.8$ and
    start a rehash.]),
  ("Returns", [`true` if a live equal record was found and tagged; otherwise `false`.]),
)

#method(
  "const Car getCar(string model, int dealer) const",
  [Read-only lookup of the record identified by (`model`, `dealer`).],
  ("Acts on", [The current table, then the old table if it exists, each with its own
    policy. Stop a probe at an empty bucket, skip deleted buckets, compare live ones.]),
  ("Returns", [A copy of the matching `Car`, or an empty `Car()` (model `""`) if there
    is no live match. A removed record counts as not found.]),
  ("Side effects", [None. The method is `const`, so it cannot advance a drain or
    trigger a rehash.]),
)

#method(
  "bool updateDealer(Car car, int dealer)",
  [Find the live record equal to `car` and change its dealer ID in place.],
  ("Why in place", [Only the model is hashed. Changing the dealer changes the
    record's identity but not its key, so the record stays in the correct bucket and
    nothing needs to move.]),
  ("Acts on", [Either table, found the same way as in `getCar`.]),
  ("Returns", [`true` if the record was found and updated; otherwise `false`.]),
  ("Side effects", [This is not an insert or a remove, so it neither advances a drain
    nor checks a trigger.]),
  ("Decide", [Should the new `dealer` be range-checked against
    $["MINID", "MAXID"]$? Should the update be refused when (model, `dealer`) already
    exists, since it would create a duplicate? Both seem wise; document your
    choice.]),
)

#method(
  "float lambda() const",
  [Report how full the current table is.],
  ("Formula", [$"m_currentSize" slash "m_currentCap"$, computed as a floating-point
    division. Deleted buckets count as full.]),
)

#method(
  "float deletedRatio() const",
  [Report what fraction of the current table's occupied buckets are dead.],
  ("Formula", [$"m_currNumDeleted" slash "m_currentSize"$, as a float. Return 0 when
    the size is 0 rather than dividing by zero.]),
)

#method(
  "void changeProbPolicy(prob_t policy)",
  [Record a policy change for future tables; existing tables are not affected.],
  ("Effect", [`m_newPolicy = policy`, and nothing else. At the next rehash, the new
    current table takes `m_newPolicy`, and the old table keeps the policy it had as
    the current table.]),
)

#method(
  "void dump() const",
  [Print both tables in index order, for debugging only. Provided.],
  ("Note", [Each line shows `model (dealer, used)`. Never call it from a test, since
    printed output does not count as a test result.]),
)

#method(
  "int findNextPrime(int current)",
  [Return the next prime capacity above `current`, kept inside the legal range. Provided.],
  ("Behaviour", [Returns the smallest prime *strictly greater* than `current`, clamped
    to $["MINPRIME", "MAXPRIME"]$. The spec says an input $<= "MINPRIME"$ returns
    `MINPRIME`, but the provided code returns 103 for an input of exactly 101.]),
  ("Used by", [Rehash: `findNextPrime(4 * (m_currentSize - m_currNumDeleted))`. Also
    the constructor, for a non-prime `size`.]),
)

#method(
  "bool isPrime(int number)",
  [Trial-division primality test. Provided.],
  ("Note", [Returns `true` for 0 and 1. This never matters, because sizes are clamped
    to at least 101 first.]),
)

= Recommended helper functions

`insert`, `remove`, `getCar` and `updateDealer` share a handful of steps: walk a
probe sequence, find a record in one table, store a record in the current table, and
move a quarter of the old table. `dealer.h` declares a private helper for each step,
and `dealer.cpp` has a stub for each that throws `logic_error` until it is written.
None of them is required, and you may add your own, as long as every helper is
declared in the `private` section of `CarDB`.

One idea runs through them: *a helper that probes a table takes that table's capacity
and policy as arguments*, so the same code serves the current table and the old one.
A helper that read `m_currentCap` or `m_currProbing` itself could not search the old
table, whose records were placed with its own capacity and policy. Inserts only ever
go into the current table, so `placeInCurrent` is the exception.

`Tester` is a friend of `CarDB`, so `mytest.cpp` can call the helpers directly and
test each one before the public methods are built on it. The blocks below use the
same layout as the method specifications, in the order `dealer.h` declares them.

#figframe[How each public method, and each rehash helper, is assembled, in order.
  Blue: a call to a private helper. White: work the method does itself. Violet: the
  rehash step from the flow figure under _When a rehash starts_. "×2" means once for
  the current table and once for the old table, if it exists; in `insert` that pair
  is the duplicate check. Not shown: `findIndex` and `placeInCurrent` compute every
  probe with `probeIndex`.][
  #fcanvas(460, 230, {
    let x0 = 84
    let h = 20
    let gap = 12
    let row(y, sig, steps) = {
      ftxt((0, y + 6), sig, sz: 7.3pt, font: font-mono, weight: "bold")
      let x = x0
      for (j, s) in steps.enumerate() {
        let (w, kind, body) = s
        if j > 0 {
          fvec((x - gap + 1.5, y + h / 2), (x - 1.5, y + h / 2), col: muted, th: 0.6pt, s: 4.5)
        }
        if kind == "call" {
          fnode(x, y, w, h, text(font: font-mono, size: 7pt, body), hue: c-define)
        } else if kind == "step" {
          fnode(x, y, w, h, text(size: 7pt, body), hue: c-prompt)
        } else {
          fbox(x, y, w, h, body, radius: 3pt, sz: 7pt)
        }
        x += w + gap
      }
    }
    row(0, [CarDB(…)], ((84, "call", [normalizeCapacity]), (110, "own", [allocate, all `nullptr`])))
    row(27, [\~CarDB()], ((64, "call", [freeTable ×2]),))
    row(54, [insert(car)], ((60, "own", [ID in range?]), (64, "call", [findIndex ×2]),
      (70, "call", [placeInCurrent]), (60, "step", [rehash step])))
    row(81, [remove(car)], ((64, "call", [findIndex ×2]), (56, "own", [tag deleted]),
      (60, "step", [rehash step])))
    row(108, [getCar(…)], ((64, "call", [findIndex ×2]), (84, "own", [copy, or `Car()`])))
    row(135, [updateDealer(…)], ((64, "call", [findIndex ×2]), (96, "own", [set dealer in place])))

    ftxt((0, 163), eyebrow(col: c-prompt, size: 6.8pt)[inside the rehash step])
    fseg((112, 167), (456, 167), col: c-prompt.transparentize(55%), th: 0.5pt, dash: "dashed")
    row(180, [startRehash()], ((64, "call", [findNextPrime]), (78, "own", [current → old]),
      (78, "own", [new current table]), (64, "call", [transferChunk])))
    row(207, [transferChunk()], ((76, "own", [next ¼, live only]), (70, "call", [placeInCurrent]),
      (66, "own", [tag old bucket]), (86, "call", [freeTable #text(font: font-sans, fill: muted)[if drained]])))
  })
]

#method(
  "int probeIndex(unsigned int hash, int i, int cap, prob_t policy) const",
  [Return the bucket that probe $i$ visits in a table of capacity `cap` that uses `policy`.],
  ("Formula", [With $H =$ `hash`, $T =$ `cap` and $h = H mod T$, this is the $i$-th
    probe from the background section: $(h + i) mod T$, $(h + i^2) mod T$ or
    #box($(h + i dot (11 - (H mod 11))) mod T$). Probe 0 is the home bucket.]),
  ("Arguments", [`hash` is the raw `m_hash(model)`. Compute it once per search and pass
    it unreduced, because the double-hash step uses $H mod 11$, not $h mod 11$.
    `policy` is an argument rather than `m_currProbing`, so the old table can be
    probed with `m_oldProbing`.]),
  ("Watch for", [Compute probe $i$ from the home bucket, never from the previous probe.
    Stepping from the previous bucket by the $i = 1$ offset each time turns quadratic
    probing into linear probing: consecutive quadratic probes are $1, 3, 5, dots$
    buckets apart, not 1. The double-hash step is $11 - (H mod 11)$, as `REQS.txt`
    specifies, not another second hash such as $1 + (H mod thin (T - 1))$.]),
  ("Used by", [`findIndex` and `placeInCurrent`, once per probe.]),
)

#pitfall[
  For quadratic probing, $i^2$ overflows a 32-bit `int` once $i > 46340$, which a
  table near `MAXPRIME` can reach on a long probe. Do the arithmetic in a wider or
  unsigned type, and stop a probe loop after `capacity` tries so a full chain cannot
  loop forever.
]

#method(
  "int findIndex(Car **table, int cap, prob_t policy, string model, int dealer) const",
  [Return the bucket that holds the live record (`model`, `dealer`) in one table, or −1.],
  ("Probe loop", [Walk the probe sequence of `m_hash(model)` with `probeIndex`. A
    `nullptr` bucket ends the search: not found. A live bucket with the same model and
    dealer is the answer. A deleted bucket, or a live one holding a different record,
    means keep probing. Stop after `cap` probes.]),
  ("Returns", [The bucket index, so that `remove` and `updateDealer` can change the
    record in place and `getCar` can copy it. −1 when there is no live match, and also
    when `table` is `nullptr`, so a caller can pass the old table without checking
    first.]),
  ("Watch for", [Each table has its own home bucket, `hash % cap` with that table's
    capacity, so an index found while probing one table means nothing in the
    other.]),
  ("Used by", [`getCar`, `updateDealer`, `remove`, and the duplicate check in
    `insert`. Each calls it once with `m_currentTable`, `m_currentCap`,
    `m_currProbing` and once with `m_oldTable`, `m_oldCap`, `m_oldProbing`.]),
)

#method(
  "bool placeInCurrent(const Car &car)",
  [Store a copy of `car` in the first free bucket of the current table, with no checks and no rehash step.],
  ("Placement", [Probe `m_currentTable` with `m_currProbing`. The first bucket that is
    `nullptr` *or deleted* takes the record, stored with `m_used = true`. Set the flag
    explicitly: the sample driver passes `true`, but the `Car` constructor defaults it
    to `false`.]),
  ("Counters", [Filling a `nullptr` bucket: `m_currentSize + 1`. Reusing a deleted
    bucket: `m_currNumDeleted − 1`, and the size is unchanged because the bucket was
    already counted as occupied.]),
  ("Returns", [`true` if stored; `false` if no free bucket turned up within
    `m_currentCap` probes, which should not happen while $lambda <= 0.5$.]),
  ("Why separate", [`insert` is checks, then placement, then the rehash step.
    `transferChunk` needs only the placement: the records it moves already passed the
    checks, and calling `insert` would run a rehash step from inside a rehash
    step.]),
  ("Used by", [`insert`, after its checks; `transferChunk`, for each record it moves.]),
)

#method(
  "void startRehash()",
  [Begin an incremental rehash: build the new table, demote the current one, and move the first quarter.],
  ("Requires", [No rehash in progress: `m_oldTable == nullptr`.]),
  ("Steps", [
    + Compute `newCap = findNextPrime(4 * (m_currentSize - m_currNumDeleted))` while
      those counters still describe the current table.
    + Move `m_currentTable`, `m_currentCap`, `m_currentSize`, `m_currNumDeleted` and
      `m_currProbing` into the matching `m_old…` fields.
    + Make the new current table: `new Car*[newCap]()`, size and deleted count 0,
      policy `m_newPolicy`.
    + Set `m_transferIndex = 0` and call `transferChunk()` for the first quarter.
  ]),
  ("Watch for", [`new Car*[n]` leaves the pointers uninitialised; the trailing `()`
    sets them all to `nullptr`. Taking the policy from `m_newPolicy` here is the only
    place a `changeProbPolicy` request takes effect.]),
  ("Used by", [`insert` and `remove`, when their trigger fires and no rehash is in
    progress.]),
)

#method(
  "void transferChunk()",
  [Move the live records in the next quarter of the old table into the current table, and free the old table once it is drained.],
  ("Requires", [A rehash in progress: `m_oldTable != nullptr`.]),
  ("Range", [$floor("m_oldCap" slash 4)$ buckets from `m_transferIndex`, which then
    advances past them. The fourth pass runs to the end of the table, so a leftover
    bucket such as bucket 100 of 101 is included and a drain always takes four passes
    (the suggested answer under _Questions the spec leaves open_).]),
  ("Each bucket", [Live: copy it into the current table with `placeInCurrent`, then
    tag the old bucket deleted. Deleted or `nullptr`: skip it. Skipping is how a
    rehash drops deleted records for good.]),
  ("Why tag", [The tagged bucket still reads as occupied, so probe chains that continue
    into later quarters stay intact (see the figure under _Incremental transfer_).
    Leaving the record live instead would leave two live copies: `remove` would tag
    the one in the current table, and `getCar` would still find the other.]),
  ("Last pass", [When the scan reaches `m_oldCap`, call
    `freeTable(m_oldTable, m_oldCap)` and reset the old table's capacity, counters and
    `m_transferIndex` to 0, as the constructor left them.]),
  ("Used by", [`startRehash` for the first quarter; `insert` and `remove` for each
    later one.]),
)

#method(
  "void freeTable(Car **&table, int cap)",
  [Delete every `Car` in a table, then the array, and set the pointer to `nullptr`.],
  ("Reference", [`table` is a reference to the pointer, so the `nullptr` is written
    into the member itself. `m_oldTable == nullptr` is how the other methods tell that
    no rehash is in progress.]),
  ("Watch for", [Return at once if `table` is already `nullptr`: the destructor frees
    the old table whether or not one exists. Resetting the capacity and counters is
    the caller's job.]),
  ("Used by", [`~CarDB`, once per table; `transferChunk`, after the last pass.]),
)

#method(
  "int normalizeCapacity(int size)",
  [Turn a requested size into a legal prime capacity.],
  ("Rule", [The capacity table under the constructor: below `MINPRIME` gives
    `MINPRIME`, above `MAXPRIME` gives `MAXPRIME`, a prime in range is kept, and
    anything else becomes `findNextPrime(size)`.]),
  ("Watch for", [Test `isPrime(size)` before calling `findNextPrime`:
    `findNextPrime(101)` returns 103, so a prime `size` would be bumped.]),
  ("Used by", [The constructor. A rehash calls `findNextPrime` directly, as the spec
    words it.]),
)

= Implementation constraints

- Add private helpers only in the `private` section of the class declaration.
- Allocate tables dynamically; they are created and destroyed at run time during rehashing.
- Rehashing builds a new table and swaps it in, so the new table becomes the current table.
- *No STL containers or other libraries in `CarDB`.* STL is allowed in `mytest.cpp`.

= Testing

== Rules for `mytest.cpp`

- The file name must be exactly `mytest.cpp`, in lower case. It contains the `Tester`
  class (declaration and implementation), every test function, and `main()`.
- Write one function per test case. Each returns `true` (pass) or `false` (fail).
  Printed output does not count as a result, and `dump()` must not be called in tests.
- Tests must not be interactive: the program compiles and runs to completion.
- Generate data with the provided `Random` class (copy it from `driver.cpp`). With a
  fixed seed it produces the same sequence on a given machine, but different
  machines can produce different numbers than `driver.txt`.
- Use a reasonable amount of data, around 50 records per test.
- Test incrementally: once a method can be tested, test it before moving on.

== Required test cases

#block(breakable: false)[
#set par(justify: false)
#table(
  columns: (auto, 1fr, 1.25fr),
  [\#], [Scenario], [What the test should assert],
  [1], [Insert about 50 records with *non-colliding* keys.], [Each record is at index `hash(model) % cap`; the size goes up by exactly 1 per insert.],
  [2], [Insert about 50 records, *half with colliding* keys.], [Every record can be found afterwards; the size is correct.],
  [3], [`getCar` on a record that is not in the database.], [An empty `Car` is returned.],
  [4], [`getCar` with several non-colliding keys.], [Each returned `Car` equals the one inserted.],
  [5], [`getCar` with several colliding keys, no rehash.], [Each is found. This also checks that colliding inserts are correct.],
  [6], [`remove` a few non-colliding keys.], [Each returns `true`; `getCar` then misses; the deleted count rises.],
  [7], [`remove` several colliding keys, no rehash.], [The same checks, plus records later in the same chain are still found.],
  [8], [Enough inserts to make $lambda > 0.5$.], [A rehash started: the old table exists, and the new capacity matches the formula.],
  [9], [Finish the load-factor rehash.], [After the follow-up operations the old table is `nullptr` and every live record is in the current table.],
  [10], [Enough removes to make $"deletedRatio" > 0.8$.], [A rehash started.],
  [11], [Finish the deleted-ratio rehash.], [The old table is freed, and every record still live before the rehash is in the current table.],
)
]

`Tester` is a friend of both `Car` and `CarDB`, so tests can read private members
such as `m_currentTable`, `m_oldTable` and the counters directly. That is how tests 1, 8,
9 and 11 can check bucket indices and table state without printing anything.

== Memory checking

Run the test program under `valgrind`; it must report no leaks and no errors. If it
finds errors, recompile with `-g` and rerun with `-s --track-origins=yes` to get line
numbers. Do not ignore compiler warnings.

= Submission

```sh
cp dealer.h dealer.cpp mytest.cpp ~/cs341proj/proj4/
```

The grading rubric is on the course's Project Grading Policy page.
