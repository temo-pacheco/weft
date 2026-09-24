# Phases 1 and 2 — from a blank page to the first woven book

Phases 1 and 2 write the spine of the book before any code tangles. Steps
**01 Determine the domain → 02 Analyze it → 03 Determine the architecture →
04 Plan the stages**, then **05 Scaffold the chapters → 06 Generate the first
document**.

Worked here on a small but real example: a **double-entry ledger service** —
a domain where correctness and vocabulary matter and the concept graph is not
trivial. (The complete version is `examples/ldd-ledger/` in the weft
repository.)

Total code tangled by the end of this walk-through: **zero**. Total tokens:
**low** — it is all prose and names. The expensive level (code) is entered
only once the cheap levels are sound, and that is the whole point.


## Phase 1 — Frame the domain

One plain document, any format. Markdown is fine: this is not the book yet,
it is the thinking. Write it as a dialog with the AI — you frame, it proposes
and challenges.

### 01 · Determine the domain

> A ledger records movements of value between accounts. Its purpose is to
> answer, at any instant and without reconciliation, *what does each account
> hold, and why*. In scope: accounts, transactions, postings, balances,
> statements. Out of scope: currency conversion, tax, and anything resembling
> a general-purpose accounting package — we are building the ledger those
> things would sit on top of.

Note the exclusions. They are not modesty; they are what makes the rest
decidable.

### 02 · Analyze the domain

Concepts, entities, and — the part worth hunting for — the **invariant**:

> **Concepts.** *accounts* owns the chart of accounts and the balances.
> *ledger* owns transactions and postings, and the invariant. *reporting*
> owns read-only projections. *api* owns the outside surface.
>
> **Invariant (the defining one).** Every transaction posts equal and
> opposite entries, so the sum of all balances is always zero. A system that
> can violate this is not a ledger with a bug — it is not a ledger. Enforced
> in *ledger*, at write time, because an invariant checked anywhere else is
> an invariant that can be bypassed.
>
> **Vocabulary.** account · entry · transaction · posting · balance.

Those five words are now the language of the whole book. They will become the
glossary, and the `Domain:` prefixes of the chunk names (`Ledger: post
entry`, `Accounts: balance of`). Get them right here and the book speaks one
language; invent vocabulary later and the glossary grows contradictory terms.

### 03 · Determine the architecture

The shape, the dependencies, and the decisions with their reasons:

> **Dependencies.** `ledger → accounts`, `reporting → ledger, accounts`,
> `api → everything`. Forbidden, deliberately: `accounts → ledger` (accounts
> must not know about transactions) and `ledger → api` (the domain must not
> depend on its surface).
>
> **Decision: enforce the invariant in the domain, not the database.**
> Chosen: the *ledger* concept refuses to construct an unbalanced
> transaction. Considered: a database `CHECK` constraint — rejected, it
> cannot express a cross-row sum cheaply. Consequence we accept: the
> invariant is only as strong as the single write path, so every write goes
> through *ledger*.

State the forbidden edges explicitly and in prose. **If nothing is forbidden,
no architectural decision has been made** — and a later reader (or AI) has no
way to know whether a reference crosses a line.

### 04 · Plan incremental stages

> **Stage 1 — walking skeleton.** Post a single balanced transaction between
> two accounts and read back both balances. Touches every Stage-1 concept
> thinly: accounts (create), ledger (post), api (one mutation, one query). No
> reporting. *Proven when* the binary runs, posts, and the two balances sum
> to zero.
>
> **Stage 2 — reporting.** Trial balance and account statements.
> *Proven when* a statement over a seeded month reconciles against the
> balances.

The skeleton must touch every Stage-1 concept. One that skips a concept does
not prove the architecture — it proves the concepts it happened to include.

**Phase 1 output:** one Markdown document. Cheap to write, cheap to rewrite,
and the entire design is settled in it.


## Phase 2 — Scaffold the project

### 05 · Scaffold the first chapters

The same four movements, now as `.weft` prose — the same source that will
later hold the code. Start from `templates/`. Prose only: no `@o`, no
code-bearing `@d`. They weave into the book and tangle to nothing.

```
literate/00-domain.weft           the project domain        (from step 01)
literate/01-domain-analysis.weft  concepts, invariants, vocabulary (step 02)
literate/02-architecture.weft     shape + decision records  (from step 03)
literate/03-stages.weft           the stages                (from step 04)
```

The domain chapter, in the book's own voice:

```latex
\chapter{The domain: money that must always balance}

A ledger records movements of value between accounts. Its one inviolable law
is \emph{double entry}: every transaction posts equal and opposite entries, so
the sum of all balances is always zero. A system that can violate this law is
not a ledger with a bug --- it is not a ledger.

\section{The problem}

Teams today reconcile balances nightly against a spreadsheet. We want the
invariant enforced \emph{at write time}, so that an unbalanced transaction
cannot be committed at all.
```

This is the LDD promise applied to design: the design is not a document that
rots beside the code, it is Chapter 1 of the same book, versioned with it.

You also write the two structural files every weft project has:

- **`project.weft`** — the master: the LaTeX preamble (`\documentclass{book}`,
  packages, title) and the `@i` includes, in reading order.
- **`literate/assembly.weft`** — the only file carrying `@o`. It routes chunks
  into output files (`@o src/ledger.c @{ @<Ledger: …@> @}`). Because output
  paths live only here, no concept knows where its code lands, and the book
  can be reorganized without touching a single narrative.

### Naming the seams

Before filling any body, name the chunks at the boundaries between concepts —
empty:

```
@d Ledger: post entry @{ @% TODO stage 1 --- posts one balanced transaction @}
@d Accounts: balance of @{ @% TODO stage 1 --- returns an account balance @}
@d Accounts: open account @{ @% TODO stage 1 @}
@d Api: mutation postTransaction @{ @% TODO stage 1 @}
```

This is design-by-contract made of chunk names: the AI then fills each concept
against seams that **already have names**, and never invents an interface
mid-flight. `weft --lint` will report them as unused until the assembly wires
them — during Phase 2 that is the expected signal of *designed, not yet
built*.

### 06 · Generate the first document

```bash
weft -w tex project.weft && pdflatex project.tex   # → project.pdf
```

Then **read it**. Front to back, as a reader. A design you cannot read
straight through is not yet a design — and this is the cheapest moment in the
project to discover that.


## What Phases 1–2 produced

- Four prose chapters that weave into the book's front matter and Part I.
- A stated architecture, including what is deliberately forbidden.
- A plan of stages, each with the fact that proves it.
- Named, empty seams: the contracts Phase 3 will honour.
- A PDF you can hand to a colleague before a line of code exists.


## Two rules that keep this from becoming Big Design Up Front

Designing first flirts with waterfall. Two rules prevent it, and neither is
optional:

1. **The design is living, not frozen.** The design chapters are revised with
   the same discipline as code — and under the same Prime Directive. They are
   the book's first pages, not a stone tablet.
2. **Thin first, not complete.** Design enough for Stage 1 plus a sense of the
   whole graph. Do not specify Stage 3 in detail before Stage 1 has taught you
   what it has to teach.


## Where this goes wrong (and how to tell)

- **Vocabulary invented later instead of framed in step 02** → the glossary
  grows contradictory terms. Test: every domain word a chunk name uses traces
  back to a definition in the analysis chapter.
- **An architecture that forbids nothing** → it checks nothing, and no reader
  can tell a violation from a design.
- **A walking skeleton that skips a concept** → Stage 1 does not prove the
  architecture.
- **Design chapters written after the code** → then they are documentation,
  not design, and the method has been inverted.
