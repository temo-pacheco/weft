# Phase 0 — Design, walked through

Phase 0 writes the spine of the book before any code tangles. Its four steps —
**Frame → Decompose → Sequence → Contract** — are worked here on a small but
real example: a **double-entry ledger service** (a financial domain, chosen
because correctness and vocabulary matter and the concept graph is non-trivial).

The output of Phase 0 is three prose-only `.weft` chapters and a set of named,
empty seam chunks. No code is implemented. The design *is* the deliverable of
this phase, and it becomes the spec the LDD cycle later fills.


## Step 1 — FRAME (`00-domain.weft`)

Write the domain and the problem in the reader's language, not the machine's.
This chapter has no chunks at all — pure exposition.

```
\chapter{The domain: money that must always balance}

A ledger records movements of value between accounts. Its one inviolable law
is \emph{double entry}: every transaction posts equal and opposite entries, so
the sum of all balances is always zero. A system that can violate this law is
not a ledger with a bug --- it is not a ledger.

The problem we solve: teams today reconcile balances nightly against a
spreadsheet. We want the invariant enforced \emph{at write time}, so an
unbalanced transaction cannot be committed at all...
```

The FRAME chapter defines the vocabulary the whole book will use: *account*,
*entry*, *transaction*, *balance*, *posting*. These terms become the glossary
and the chunk-name domains later. Get them right here and the rest of the book
speaks one language.


## Step 2 — DECOMPOSE (`01-architecture.weft`)

Name the concepts, choose the technology, record the decisions, and — the part
that makes the architecture machine-checkable — **declare the allowed
dependency graph**.

```
\chapter{Architecture}

\section{Concepts}
We decompose the system into four concepts:
\begin{itemize}
  \item \textbf{accounts} --- the chart of accounts and balances
  \item \textbf{ledger} --- transactions and postings; owns the invariant
  \item \textbf{reporting} --- read-only projections over the ledger
  \item \textbf{api} --- the GraphQL surface
\end{itemize}

\section{Decision: enforce the invariant in the domain, not the database}
We considered a database CHECK constraint (rejected: cannot express a
cross-row sum cheaply) and application-level validation (chosen: the ledger
concept refuses to build an unbalanced transaction) ...
```

Then declare the permitted edges as LaTeX comments — invisible in the woven
book, but the machine-readable contract the conformance gate enforces:

```
% ARCH-ALLOW: ledger -> accounts
% ARCH-ALLOW: reporting -> ledger
% ARCH-ALLOW: reporting -> accounts
% ARCH-ALLOW: api -> ledger
% ARCH-ALLOW: api -> accounts
% ARCH-ALLOW: api -> reporting
```

Note what is **not** allowed: `accounts -> ledger` (accounts must not depend on
transactions), `ledger -> api` (the domain must not depend on its surface).
If a chunk ever creates one of those edges, the conformance gate fails. The
architecture is now a fitness function, not a diagram that lies.

Record ADRs here as prose. The "why" lives with the code, versioned, not in a
separate `docs/adr/` folder that drifts.


## Step 3 — SEQUENCE (`02-roadmap.weft`)

Order the work into stages and pick the **walking skeleton** — the thinnest
vertical slice that tangles to a running system. Hold the concept ledger here.

```
\chapter{Roadmap}

\section{Stage 1 --- walking skeleton}
Post a single balanced transaction between two accounts and read back both
balances. Touches every concept thinly: accounts (create), ledger (post),
api (one mutation, one query). No reporting yet. When this tangles and runs,
the architecture is proven end to end.

\section{Stage 2 --- reporting}
Trial balance and account statements over the ledger.

\section{Stage 3 --- fraud / limits}
...
```

The ledger table (the literate backlog):

| Concept | Status | Depends on | Stage | Chapter |
|---|---|---|---|---|
| accounts | `stubbed` | — | 1 | §4 |
| ledger | `stubbed` | accounts | 1 | §5 |
| api | `stubbed` | ledger, accounts | 1 | §6 |
| reporting | `planned` | ledger, accounts | 2 | — |

Everything is `planned` or `stubbed` at the end of Phase 0. Nothing is
`implemented` yet — that is the LDD cycle's job.


## Step 4 — CONTRACT (seam chunks, still empty)

Name the chunks at the boundaries between concepts, empty. This fixes the
interfaces before anyone fills a body, so the AI never improvises a seam.

```
@d Ledger: post entry @{ @% TODO stage 1 --- posts one balanced transaction @}
@d Accounts: balance of @{ @% TODO stage 1 --- returns an account balance @}
@d Accounts: open account @{ @% TODO stage 1 @}
@d Api: mutation postTransaction @{ @% TODO stage 1 @}
@d Api: query accountBalance @{ @% TODO stage 1 @}
```

These are the only chunks that exist so far. `weft --lint` will report them as
unused (they are not yet wired into an `@o`) — that is expected during Phase 0
and is the signal of "designed, not yet built." As the LDD cycle fills each
concept and the assembly wires them, the orphans disappear.


## What Phase 0 produced

- Three prose chapters that weave into the book's front matter and Part I.
- A declared architecture graph the conformance gate can enforce.
- A concept ledger: the map of intent.
- Named, empty seams: the contracts the cycle will honour.

Total code tangled: **zero**. Total tokens: **low** — it is all prose and
names. The expensive level (code) is entered only once the cheap levels
(outline, prose) are sound. That is the token-optimal path, and it is the
whole point of designing in the book before designing in the compiler.


## Where FRAME can go wrong (and how to tell)

- **Vocabulary invented later instead of framed here** → the glossary grows
  contradictory terms. Fix: every domain word a chunk name uses must trace to
  a definition in `00-domain.weft`.
- **Architecture with no forbidden edges** → an ARCH-ALLOW list that permits
  everything checks nothing. If nothing is forbidden, you have not made an
  architectural decision.
- **A "walking skeleton" that skips a concept** → then Stage 1 does not prove
  the architecture. The skeleton must touch every Stage-1 concept, however
  thinly.
