---
name: literate-driven-development
description: >
  Literate Driven Development — a methodology for building software WITH AI
  where the narrative is the specification. Three phases (Frame the domain →
  Scaffold the project → Write the book chapter by chapter) and one rule:
  change the code, update the prose. Self-contained: it carries the whole
  literate-programming paradigm and the complete weft reference, so no other
  skill is needed. Runs on one command, `weft` — no scripts, no Python, no
  harness. Triggers: "LDD", "literate driven development", "desarrollo
  dirigido por literatura", "programacion literaria con IA", "literate
  development with AI", "build a project with weft".
---


# Literate Driven Development (LDD)

A methodology for building software **with an AI** on top of literate
programming. Its thesis, stated once:

> **TDD says: the test is the specification, the code is derived.**
> **LDD says: the narrative is the specification, and BOTH the code and
> the tests are artifacts derived from it.**

In literate programming the `.weft` file is, at the same time, the **spec**,
the **context window**, and the **deliverable**. Prompt, code and
documentation are not three files — they are one object. LDD is the
discipline of writing that object as a book, chapter by chapter, with the AI
as co-author.

This is **pure LDD**: no spec-driven development, no harness engineering, no
agentic loops, no task runner, no generated helper scripts. You may add them,
but none are indispensable — and this skill will not ask for any.


## What it takes

**The `weft` binary on `PATH`. That is all.**

This skill is **self-contained**: it carries the literate-programming paradigm
and the complete weft reference inside its own `references/`, so it needs no
other skill loaded and no step 0. If the `literate-programming` skill is also
installed, it holds the same paradigm material *without* this methodology —
for someone who wants literate programming alone. You never need both.

There is nothing to scaffold into the user's project either: no helper script,
no hook, no CI file, no lock file. If a project wants CI, that is the
project's decision, not a requirement of the method.

### The metalanguage, in brief

Everything below is expanded in this skill's own references — start here, go
there when you need depth.

```
@o path/file.c @{ ... @}    output file: tangle writes this chunk to that path
@d Concept: thing @{ ... @}  fragment: a named, reusable chunk
@<Concept: thing@>           invocation: expands that fragment here
@i other.weft                include another source file
@% ... @}                    a comment inside a chunk (tangles to nothing)
@@                           a literal @ (escape it in decorators, emails, …)
```

Chunks compose hierarchically and may be **defined in several places**; weft
concatenates them in order. A chunk name is `Domain: what it does` — the
domain prefix is what makes the book, the map and the lint output legible.

The navigation flags — the AI's IDE, and the reason it never greps:

```
weft file.weft                 tangle: .weft → the output files
weft -w tex|md|html file.weft  weave: the book
weft -m --prose file.weft      JSON map: every fragment, its prose, the graph
weft -e NAME --callers f.weft  one fragment, its dependencies, who calls it
weft --lint file.weft          undefined refs (with suggestions), orphans
weft --diff file.weft          what re-tangling would change (writes nothing)
make 2>&1 | weft --errors      compiler errors → .weft source lines
weft -R src/file.c:47          one reverse lookup, no .weft files needed
weft --help                    everything, when context is lost
```

Full detail: [references/weft-syntax.md](references/weft-syntax.md) for the
complete syntax, [references/naming-conventions.md](references/naming-conventions.md)
for chunk naming and narrative rules,
[references/literate-craft.md](references/literate-craft.md) for writing the
prose, [references/concept-architecture.md](references/concept-architecture.md)
for organizing by concept,
[references/debugging-tangled-code.md](references/debugging-tangled-code.md)
for tracing errors back to source, and
[references/build-pipeline.md](references/build-pipeline.md) for the tangle →
build pipeline per language.
## The workflow

Three phases. Each one produces a real artifact before the next begins.

```
  PHASE 1 · Frame the domain       (in prose, before any code)
     01 Determine the domain
     02 Analyze the domain
     03 Determine the architecture
     04 Plan incremental stages
        → what you get: a simple domain document, in any format

  PHASE 2 · Scaffold the project   (turn the thinking into a book)
     05 Scaffold the first chapters
     06 Generate the first document
        → what you get: the first woven book

  PHASE 3 · Write the book         (chapter by chapter)
     07 Write the chapter
     08 Review, then advance
        → what you get: the complete literate code

        write → review → advance — chapter after chapter
```


## Phase 1 — Frame the domain

In prose, before any code. The output is **one plain document in any format**
(Markdown is fine — it is not yet the book). Cheap to write, cheap to throw
away, and the whole design lives here before a single token is spent on code.

**01 · Determine the domain.** Name exactly what is being built, and its
purpose and scope. Write it in the reader's language, not the machine's.
Say also what is *out* of scope — a domain with no edges cannot be framed.

**02 · Analyze the domain.** Map the core concepts, entities and invariants
the system turns on. The invariant is the part worth hunting for: the law
that, if violated, means you no longer have the thing at all. This step also
fixes the **vocabulary** — every term the book will use, and later the
`Domain:` prefix of every chunk name.

**03 · Determine the architecture.** Decide the system's shape — the
concepts, their boundaries, the technology — and record each decision **with
its rationale**: what was chosen, what was rejected, and why. A decision
without its alternatives is not a decision, it is a preference.

**04 · Plan incremental stages.** Break the build into stages that each run
and are **proven** before the next starts. Stage 1 is the thinnest slice that
tangles to a running system and touches every Stage-1 concept, however
thinly — a stage that skips a concept proves nothing. Software is
evolutionary: each stage is built on ground already solid.

Phase 1 is a **dialog**, not a form to fill. The human frames the domain and
the problem; the AI proposes concepts, boundaries, decisions, stages, and
pushes back on what is vague. Iterate here until the shape is sound: this is
the cheapest level to change, and every hour spent here is paid back in code
that does not have to be rewritten.

See [references/ldd-design.md](references/ldd-design.md) for Phases 1–2
walked through on a real example.


## Phase 2 — Scaffold the project

Turn the thinking into a book.

**05 · Scaffold the first chapters.** With the literate-programming
discipline, write the opening chapters as `.weft` — the same source that will
later hold the code:

| Chapter | From | What it is |
|---|---|---|
| The project domain | step 01 | what this is, its purpose and scope |
| General domain analysis | step 02 | concepts, entities, invariants, vocabulary |
| Architecture | step 03 | the shape, and the decision records |
| Development stages | step 04 | the stages, and what each one proves |

These chapters are **prose that does not tangle**: no `@o`, no code-bearing
`@d`. weft weaves them into the book and they produce zero output files. This
is the LDD promise applied to design — the design is not a doc that rots on
the side, it is Chapter 1 of the same book, versioned with the code.

You also need the two structural files of any weft project: a **master**
`project.weft` (the formatting preamble plus `@i` includes) and an
**assembly** file (the only file carrying `@o`, which routes chunks to output
paths). Concepts never name their own output paths, so the book can be
reorganized without touching a single narrative.

Start the chapter skeletons from `templates/`:

```
templates/00-domain.weft           the project domain
templates/01-domain-analysis.weft  concepts, entities, invariants, vocabulary
templates/02-architecture.weft     the shape + decision records
templates/03-stages.weft           the stages
```

**06 · Generate the first document.** Weave the sources into a real book — the
single source of truth, readable by a human before any code exists:

```bash
weft -w tex project.weft && pdflatex project.tex   # → project.pdf, the book
weft -w md  project.weft                            # → Markdown
weft -w html project.weft                           # → HTML
```

Read it. A design you cannot read straight through is not yet a design.


## Phase 3 — Write the book, chapter by chapter

One stage at a time, and within a stage one substage at a time.

**07 · Write the chapter.** Draft the stage as prose first, then derive its
code and its tests from that prose — through every substage. Concretely, for
each unit of work:

1. **Narrate.** Write what this part does and *why*, in the reader's order,
   and **name the chunks** you will need (`@d Ledger: post entry @{ … @}`) —
   even before filling them. The prose plus the chunk names are the spec.
   Naming the chunks at the seams between concepts first is what stops
   interfaces from being improvised mid-flight.
2. **Orient.** Never start by reading files. Query the map:
   `weft -m --prose project.weft` for structure plus the prose around each
   fragment, `weft -e "<chunk>" --callers project.weft` for one fragment and
   its call chain. Load bodies only for the chunks you will actually touch.
3. **Fill.** Implement the chunk bodies in the `.weft` source, referencing
   existing chunks by name (`@<name@>`) instead of re-deriving them. Stable,
   tested code is a named black box.
4. **Verify.** Tangle, then build and run. Map every failure back to the
   source: `make 2>&1 | weft --errors`, or `weft -R output.c:47` for a single
   lookup. Never open the tangled file to debug it.
5. **Reconcile.** Re-read the prose around every chunk you touched and make
   it tell the truth about the new code. This is not optional — see the Prime
   Directive.

Tests are derived from the narrative like everything else: the prose says what
must hold, and the test chunk proves it. They live in the same concept file as
the code they test, because that is where the reader needs them.

**08 · Review, then advance.** Read the chapter back, find the gaps, and close
them **before** starting the next one — no technical debt, no "we'll fix it in
Stage 3". A stage boundary is an edition of the book: everything tangles,
builds green, and reads complete for what exists. Then, and only then, begin
the next chapter.

The review is the whole quality mechanism of LDD, and it is the one place
where rushing costs the most.

See [references/ldd-cycle.md](references/ldd-cycle.md) for one unit of work
walked through end to end, and [references/ldd-review.md](references/ldd-review.md)
for the review checklist in full.


## The Prime Directive

> **A task is NOT complete until its prose has been updated. No exceptions.**

This is the single rule the whole method exists to protect. It is why the
literate document can be trusted as living context for humans and AIs alike:
because it is *never* allowed to fall behind the code.

The prose says the **why** and the **what**; the code says the **how**. That
pairing is what stays valuable — today, and for whoever inherits the project.

Stated in its strongest form: in LP the code is the *derived* artifact and the
prose is the *source*. So **changing code without changing its prose is the
same category of error as editing the tangled output** — you edited the wrong
layer. Both are forbidden; they are the same sin.

For the AI this is operational, not aspirational:

- You may **not** report a task complete without stating explicitly that the
  prose was updated, or affirming it was reviewed and still holds. "It
  compiles and tests pass" is not done.
- In step 05 of the review you re-read the prose around every chunk you
  touched. If the code changed and the prose did not, say so and fix it.
- Every pass is also a chance to *enrich* the prose: with LaTeX you have
  equations, tables, cross-references, diagrams and the full typographic
  richness of publishing — all in service of explaining the code better.

This directive outranks convenience, token budget, and speed.


## The review checklist

Five checks, cheapest first, all of them plain `weft`. There is no runner and
nothing to install — you run them and you **report the result**.

| Check | Command | What it catches |
|---|---|---|
| **Refs** | `weft --lint project.weft` | Invented chunk names (with fuzzy suggestions), orphan chunks. The anti-hallucination check: anything *invented* rather than *referenced* fails here. |
| **Sync** | `weft --diff project.weft` | Claiming done while the tangled output is stale. Empty diff = in sync. |
| **Build** | `make 2>&1 \| weft --errors` | Real compile/test failures, annotated with the `.weft` line that produced them. |
| **Prose** | re-read the prose of every touched chunk | Code changed, prose stale. The Prime Directive. |
| **Book** | read the chapter straight through | Gaps, contradictions, a concept that no longer reads as an essay. |

Cheap-and-structural before expensive-and-behavioural: failing `--lint` means
you never pay for the build. That ordering is what makes LDD cheap to keep
correct.

The last two checks are judgement, not machinery — and that is deliberate. A
hash cannot tell you whether prose tells the truth. The author and the AI can.


## You and the AI are co-authors

Writing in LDD is like writing a book together. It is a continuous dialog, not
a queue of tickets.

| Phase | You | The AI |
|---|---|---|
| 1 · Frame | frame the domain and the problem | propose concepts, invariants, decisions, stages; challenge what is vague |
| 2 · Scaffold | approve the shape of the book | write the chapters, weave the first document |
| 3 · Write | narrate intent, name chunks, review | orient, fill, verify, reconcile |
| Prime Directive | demand it | obey it — never close without prose |

You define *what* and *why*; the AI produces *how* and verifies it. At every
moment the author knows and understands the code that was generated — from
day one, from the first line.

This workflow is a **proposal**, deliberately simple, not the definitive one.
There are as many ways to write as there are authors: adapt it to yours.


## Managing complexity

The unit of comprehension is the **concept**, not the file. The AI navigates a
queryable graph; it never loads the whole territory.

- **One concept crosses every layer and language.** A `login.weft` can hold
  fragments of Go, SQL, TypeScript, HTML, Terraform, shell — with their
  tests — plus the REST and GraphQL calls: login, expressed conceptually. The
  cross-language *contract* is explained in one place, exactly where
  multi-language bugs hide. Improving login means reading one file.
- **`weft -m` is the system map.** For a system too large for context, query
  the graph instead of reading it.
- **`@o` lives only in the assembly file.** Physical layout changes without
  touching any narrative.
- **`--callers` before touching a shared chunk** — impact analysis across
  every language at once.
- **Concepts grow into subconcepts.** When a concept stops reading straight
  through in one sitting, split it into a folder of `.weft` files; chunk names
  carry the hierarchy (`Payments: refunds — process`) so `-m` and `--lint`
  show the tree. Because `@o` lives elsewhere, the book reorganizes and the
  code tangles the same.


## Token economy

> The token cost of a change scales with the size of the **change**, not the
> size of the **system** — because names let you reference without loading.

- **Index before content.** `weft -m --prose` to orient, then `-e` to pull
  only the fragments you touch. Not "read the repo."
- **Stable code is a named black box.** Reference `@<name@>`; never re-read
  tested, stable chunks.
- **`--diff` previews without rebuilding or rewriting.**
- **Debugging reads only section markers.** `-R` and `--errors` need no
  `.weft` files at all.
- A typical task = map + extract 1–3 fragments + edit 1 chunk + diff.
- Designing in prose is token-optimal: the outline is ~free to change, prose
  is cheap, code is expensive. Settle the cheap levels first.


## Two outputs, one source

`.weft` files are organized around mental concepts, in a metalanguage designed
to be at once **explained to humans** and **understood by computers**. *Weave*
them and you get a book; *tangle* them and you get the source the compilers
build. Same file.

That makes the artifact a typeset book by construction, so LDD treats the
book-grade elements as part of the work, not decoration:

| Book element | How |
|---|---|
| Chapters / sections | master `.weft`, `\documentclass{book}`, concept = chapter |
| Code cross-references | automatic "used in / uses" + chunk index (native to LP) |
| Prose cross-references | LaTeX `\label` / `\ref`, `hyperref` |
| Tables, figures, diagrams | `booktabs`, `tikz`, in the prose |
| Bibliography | `\cite` + in-repo `bib/*.bib` |
| Glossary | a `glossary.weft` concept + `glossaries` |
| Index | chunk index (native) + `makeidx` for prose terms |
| Typography | `microtype`, a chosen font, a prose style guide |
| Three audiences | `-w tex` (book) · tangle (machine) · `-w md`/`-w html` (web) |

**The standard is publishable quality.** A chapter is not done if it does not
read straight through, lacks its glossary entry, or fails to cross-reference
what it names.


## What the AI must and must not do


**Must:**
- Edit only `.weft` source.
- Orient with `-m --prose` / `-e` / `--callers` before reading anything.
- Run the review checklist and **report** `--lint` and `--diff` before
  claiming done.
- Update the prose in the same change as the code.
- Reference stable chunks by name instead of re-reading them.
- Close the gaps of a chapter before starting the next one.
- Consult this skill's own `references/` for syntax, naming and craft — they
  are here; no other skill needs to be loaded.

**Must not:**
- Read or edit tangled output.
- Invent a chunk name (check with `-m` first; `--lint` will catch it).
- Mark a change complete with stale prose or a dirty `--diff`.
- Load whole files when `-e` returns the exact fragment.
- Introduce a script, runner, hook, lock file or CI harness in order to
  "enforce" the method. The method is the discipline, plus `weft`.


## Relationship to the other skills

This skill stands alone. The others are alternatives or complements, never
prerequisites:

- **`literate-programming`** — the same paradigm and the same weft reference,
  *without* the methodology. It is for someone who wants literate programming
  on its own: the language, not the process. Everything it contains is already
  inside this skill, so **do not load both** — if the task is "build a project
  this way", you are in the right one.
- **`111`** (Mojolicious), **`333`** (Go microservices), **`444`** (Flutter) —
  per-stack tangle pipelines and conventions. Use LDD for the *process* and a
  stack skill for the *pipeline* of a given target language.


## Where to look next

**The method:**

- [QUICKSTART.md](QUICKSTART.md) — the hands-on walk: activation, the three
  phases, and the commands you actually type.
- [references/ldd-design.md](references/ldd-design.md) — Phases 1–2 worked on
  a real example, from blank page to first woven book.
- [references/ldd-cycle.md](references/ldd-cycle.md) — one unit of work
  through narrate → orient → fill → verify → reconcile.
- [references/ldd-review.md](references/ldd-review.md) — the review checklist,
  with real failure output for each check.
- [references/ldd-notes.md](references/ldd-notes.md) — notes for the author:
  the mindset and the habits that keep prose and code moving together.
- [MANIFESTO.md](MANIFESTO.md) — why the method exists.

**The paradigm and the tool** (carried here in full, so this skill needs no
other):

- [references/weft-syntax.md](references/weft-syntax.md) — the complete weft
  syntax: every directive, flag, scrap mode and CLI option.
- [references/naming-conventions.md](references/naming-conventions.md) — chunk
  naming, narrative rules, anti-patterns.
- [references/literate-craft.md](references/literate-craft.md) — how to write
  the prose: exposition, order, and the standard of publishable quality.
- [references/literate-philosophy.md](references/literate-philosophy.md) — the
  philosophy, Knuth and Childs' requirements.
- [references/concept-architecture.md](references/concept-architecture.md) —
  organizing a system by concept, and the assembly pattern.
- [references/debugging-tangled-code.md](references/debugging-tangled-code.md)
  — section markers, and tracing an error back to its `.weft` line.
- [references/build-pipeline.md](references/build-pipeline.md) — the tangle →
  format → build pipeline, per language.
- [references/worked-example.md](references/worked-example.md) — one concept
  spanning Go + Flutter + SQL + Protobuf, end to end.
