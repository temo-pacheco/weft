---
name: ldd
description: >
  Literate Driven Development — a methodology for building software WITH AI
  where the narrative is the specification. Wraps the weft toolkit into a
  disciplined cycle (Narrate → Orient → Fill → Verify → Reconcile) with
  hard gates against hallucination and prose/code drift. Builds on the
  literate-programming skill.
  Triggers: "LDD", "literate driven development", "desarrollo dirigido por
  literatura", "programacion literaria con IA", "literate development with AI".
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
discipline that turns that fact into engineering guarantees.

This skill assumes the `literate-programming` skill (the paradigm and the
`weft` toolkit). LDD adds the **process**, the **hard gates**, and the
**editorial layer**.

> **Step 0 — before anything else, load the `literate-programming` skill**
> (via the Skill tool). It carries the weft syntax, the `@d`/`@o`/`@<@>`
> directives, and the paradigm this skill builds on. Do this at the start of
> every LDD session; LDD is the process, `literate-programming` is the
> language it operates in. Skills are not auto-imported — you must load it.


## The four principles

1. **The narrative is the prompt.** The human writes *intent* as prose and
   names the chunks (empty or stubbed). That naming IS the task. The AI does
   not guess a design; it fills a specification that stays versioned in the
   source.

2. **The chunk is the atomic unit of everything.** A `@d` chunk is at once
   the cognitive unit (one screenful, one idea), the AI's unit of work, the
   human's unit of review, and the unit of commit. LDD collapses
   "task = chunk = review = transaction." No agent edits 2000 lines; it
   edits one chunk with its prose.

3. **The weft toolkit is the AI's IDE — never the grep.** Hard rule: the AI
   **never** navigates or edits tangled output. It navigates the `.weft`
   source via `-m --prose`, extracts with `-e`, traces with `--callers`, and
   translates errors with `--errors`/`-R`. This eliminates the classic
   failure mode ("it edited the generated file") by construction.

4. **Prose↔code coherence is a *verifiable* invariant, not a good
   intention.** The definition of done (prose updated) is a CI gate, not a
   habit. A chunk changed without its prose updated is a failing build.


## The Prime Directive

> **A task is NOT complete until its prose has been updated. No exceptions.**

This is the single rule the whole methodology exists to protect. It is the
reason the literate document can be trusted as living context for both humans
and AI: because it is *never* allowed to fall behind the code.

Stated in its strongest form: in LP the code is the *derived* artifact and the
prose is the *source*. So **changing code without changing its prose is the
same category of error as editing the tangled output** — you edited the wrong
layer. Both are forbidden; they are the same sin.

For the AI, this is operational, not aspirational:

- You may **not** report a task complete without stating, explicitly, that the
  prose was updated or affirmed still-correct. "It compiles and tests pass" is
  not done.
- The **Prose gate** enforces this mechanically (see Gates): a fragment whose
  code changed but whose prose did not is flagged, and the only way past is a
  logged, auditable affirmation that the prose was reviewed and still holds.
- In the RECONCILE step you must re-read the prose around every chunk you
  touched and make it tell the truth about the new code.

This directive outranks convenience, token budget, and speed. If updating the
prose is skipped, the task is not done — full stop.


## Phase 0 — Design (the spine of the book)

Before any code is tangled there is a design phase. A literate program **is**
a book; the great ones open with chapters of domain and architecture before a
single line of code. Phase 0 is that front matter — and it lives in the same
source as everything else.

The design chapters are **prose that does not tangle**: concepts with no `@o`
and no output-bound `@d`. weft weaves them into the book but they produce zero
code. This is the LDD promise (documentation *is* source) applied to design:
the design is not a doc that rots on the side — it is Chapter 1 of the same
book, versioned with the code, and it *becomes* the spec the code chapters
fill.

```
literate/
├── 00-domain.weft         front matter: the domain and the problem to solve
├── 01-architecture.weft   Part I: technology, architecture, decisions (ADRs)
├── 02-roadmap.weft        the logical stages + the concept ledger
└── concepts/*.weft        Parts II+: one chapter per concept
```

### Three levels of commitment

Design **top-down, revise bottom-up**, across three levels of rising cost.
This is why designing in prose is token-optimal: you iterate the cheap levels
until the concept graph is sound, then spend the expensive tokens on code.

| Level | Artifact | Cost to change | When |
|---|---|---|---|
| **1 · Outline** | table of contents = concept ledger + stages | ~nil | first |
| **2 · Prose** | design chapters + concept narratives | medium | next |
| **3 · Code** | chunk bodies | high (build, tokens) | last |

### The Phase 0 cycle

```
  FRAME       domain + problem              → 00-domain.weft
  DECOMPOSE   concepts + declared graph     → 01-architecture.weft
  SEQUENCE    stages; pick the walking skeleton → 02-roadmap.weft
  CONTRACT    name the seam chunks between concepts (empty stubs)
```

**CONTRACT is the step that prevents improvised interfaces.** Before filling
anything, name the chunks at the seams between concepts
(`@<Payments: charge@>`, `@<Ledger: post entry@>`) even while empty. The AI
then fills each concept against seams that already have names — it never
invents an interface mid-flight. Design-by-contract, made of chunk names.

### The Concept Ledger

`02-roadmap.weft` holds the single source of truth for what exists and what is
planned — the literate backlog. The AI reads it to orient before touching code
(it is the map of the *design*, complementing `weft -m`, the map of the
*code*).

| Concept | Status | Depends on | Stage | Chapter |
|---|---|---|---|---|
| auth | `stable` | session-store | 1 | §3 |
| payments | `implemented` | auth, ledger | 2 | §7 |
| fraud | `stubbed` | payments | 3 | §7.4 |
| reporting | `planned` | payments, ledger | 3 | — |

Lifecycle: `planned → stubbed → implemented → stable`.

### Concepts grow into subconcepts

A concept is born as one `.weft` with a few chunks. When it stops reading
straight through in one sitting, it **splits**: `payments.weft` → a `payments/`
folder with `core.weft`, `refunds.weft`, `fraud.weft`. The chapter gains
sections. Chunk names encode the hierarchy (`Payments: refunds — process`), so
`-m` and `--lint` show the tree. Because `@o` lives only in `assembly.weft`,
you split the file **without touching output routing** — the book reorganizes,
the code tangles the same.

### Two rules that keep Phase 0 incremental (not waterfall)

Design-first flirts with Big Design Up Front. Two rules prevent it, and they
are not optional:

1. **The design is living, not frozen.** Chapter 1 is revised with the same
   `--diff` discipline as code. It is the book's first page, not a stone tablet.
2. **Thin first (walking skeleton), not full spec.** Design just enough for
   Stage 1 plus a sense of the global graph. Do not specify Stage 3 in detail
   before learning from Stage 1.

Stages are **editions of the book**: each stage boundary is a git tag where the
whole system tangles, builds green, and reads complete for what exists.

See [references/ldd-design.md](references/ldd-design.md) for the full Phase 0
walk-through, and the `templates/` chapter skeletons
(`00-domain.weft`, `01-architecture.weft`, `02-roadmap.weft`).


## The LDD cycle

After Phase 0, every unit of work walks this loop — the analogue of
red-green-refactor:

```
  1. NARRATE   human: prose + chunk names            ← the "spec"
  2. ORIENT    AI: weft -m --prose / -e / --callers   ← directed context
  3. FILL      AI: implement chunk bodies             ← tangle-fill
  4. VERIFY    tangle → build/test; weft --errors      ← "green"
  5. RECONCILE AI: --lint + prose/code drift check     ← the "refactor"
  6. COMMIT    at chunk / concept granularity
```

The two steps no other paradigm has are **2 (Orient)** and **5 (Reconcile)**.
Orient replaces "read 15 files" with one directed JSON map. Reconcile is the
step that checks the prose *still describes* what the code does — the
equivalent of refactor, but over narrative coherence.

### Step 1 — NARRATE (human-led)
Write the concept as prose first: what it does and *why*, in the reader's
order. Name the chunks you will need (`@<Domain: description@>`) even before
implementing them. Leave bodies empty or as one-line stubs. The prose plus
the chunk names are the specification the AI will fill.

### Step 2 — ORIENT (AI, cheap tokens)
Never start by reading files. Start by querying the map:

```bash
ldd context "<Concept>"                    # structure + prose + usage, one call
weft -e "<chunk>" --callers project.weft   # what it is + who uses it
weft --lint project.weft                    # graph health first
```

`ldd context` is the one-command orient pack (no jq needed); for ad-hoc
exploration `weft -m --prose project.weft | jq …` also works. Load bodies
(`--bodies` / `-e`) only for the chunks you will actually touch.

### Step 3 — FILL (AI)
Implement chunk bodies from the narrative. Reference existing chunks by name
(`@<name@>`) instead of re-reading or re-deriving them — treat stable,
tested chunks as named black boxes. Edit the `.weft` source only.

### Step 4 — VERIFY (mechanical)
Tangle, then build/test. Translate every failure back to source:

```bash
make 2>&1 | weft --errors        # compiler line → literate/xyz.weft:NN
weft -R output.file:47           # single reverse lookup
```

### Step 5 — RECONCILE (AI)
Close the loop before claiming done:

```bash
weft --lint project.weft         # no undefined refs, no orphan chunks
weft --diff project.weft         # tangled output is in sync
```

Then re-read the prose around every chunk you changed: does it still explain
the code? Update stale prose. This step is not optional — see Gates.

### Step 6 — COMMIT
Commit at chunk or concept granularity. The commit message references the
concept, not the tangled files.


## Hard gates (the anti-hallucination layer)

A hallucination is *unverified generation*. LDD bounds uncertainty by making
every level independently checkable. A change is **NOT done** until all four
gates pass. All five are run by `ldd verify` (the `templates/ldd.py` CLI —
Python stdlib only, no bash/jq/just) and by CI.

| Gate | Command | What it catches |
|---|---|---|
| **Refs** | `weft --lint` clean | Invented chunk names, orphan chunks. Any hallucinated reference fails here. |
| **Sync** | `weft --diff` clean | Claiming "done" while tangled output is stale or uncommitted. |
| **Build** | `make 2>&1 \| weft --errors` green | Real compile/test failures, mapped to `.weft` source. |
| **Prose** | `ldd prose check` + reviewer | Code changed but its prose stayed stale (mechanical), or prose no longer tells the truth (reviewer). The Prime Directive. |
| **Conformance** | declared graph vs `weft -m` | A chunk crossing a concept boundary the architecture chapter forbids. |

> The AI must run `--lint`, `--diff` and the conformance check and report them
> before saying a task is complete. "It compiles" is one gate of five.

The **Conformance** gate is what closes the loop with Phase 0: the architecture
chapter declares the allowed dependency edges between concepts (as
`% ARCH-ALLOW: payments -> auth` comments, invisible in the woven book), and
`weft -m` yields the *real* edge set (a chunk in concept A referencing a chunk
in concept B is an edge A→B). Any real edge not permitted fails the gate. The
architecture chapter becomes machine-checkable against the actual code graph —
architecture-as-code derived from the narrative.

### The `ldd` CLI

All gates and orchestration live in one dependency-free Python file,
`templates/ldd.py` (stdlib only — no bash, jq, shasum, openssl, or just). Its
only requirements are `python3` and the `weft` binary, so it runs identically
on macOS, Linux, Windows, and CI.

```
ldd verify [--build "CMD"]     run the five gates in order
ldd conformance                gate 3 standalone
ldd prose check                gate 5: the Prime Directive (mechanical)
ldd prose lock [--affirm]      record the reviewed prose/code state
ldd context CONCEPT            orient: directed context pack for a concept

# globals:  --main FILE  (default $LDD_MAIN or project.weft)
#           --weft PATH  (default $WEFT or weft on PATH)
# invoke as `python3 ldd.py …`, or chmod +x and alias `ldd`.
```

The `templates/pre-commit` hook (also pure Python) runs `ldd prose check`
before every commit, so the Prime Directive cannot be violated even by
accident. `templates/ci.yml` runs `ldd verify` on every PR.


## Managing complexity (many components, many languages)

The unit of comprehension is the **concept**, not the file. The AI navigates
a queryable graph; it never loads the whole territory.

- **One concept spans all layers and languages.** `payments.weft` narrates
  the Go entity, GraphQL schema, Flutter model, SQL migration and gRPC proto
  in one story. The cross-language *contract* is explained in one place —
  exactly where multi-language bugs hide.
- **`weft -m` is the system map.** For a system too large for context, query
  the graph (`jq`) instead of reading it.
- **`@o` lives only in `assembly.weft`.** Concepts don't know their output
  paths, so physical layout can change without touching any narrative.
- **`--callers` before touching shared chunks** — transitive impact analysis
  across every language.
- **Chunks nest ("web, not tree").** Complexity is handled by hierarchical
  abstraction with names, the way humans handle it.


## Token economy (cheap, fast, incremental)

> The token cost of a change scales with the size of the **change**, not the
> size of the **system** — because names let you reference without loading.

- **Index before content.** `weft -m --prose` (cheap) to orient, then `-e`
  to pull only the fragments you touch. Not "read the repo."
- **Stable code is a named black box.** Reference `@<name@>`; never re-read
  tested, stable chunks.
- **`--diff` previews without rebuilding or rewriting.**
- **Debugging reads only section markers.** `-R`/`--errors` never open the
  `.weft` sources.
- A typical task = map + extract 1–3 fragments + edit 1 chunk + diff.
  Development is **incremental by construction**: you work at the frontier,
  on top of what is already tested and stable.


## The editorial layer (works of art, not just programs)

The weave to LaTeX makes the artifact a **typeset book by construction**,
from the same source as the program. LDD makes the book-grade elements
mandatory, not optional.

| Book element | How | Source |
|---|---|---|
| Chapters / sections | Master `.weft`, `\documentclass{book}`, concept = chapter | weft weave |
| Code cross-references | Auto "used in §X / uses §Y" + native chunk index | native to LP |
| Prose cross-references | LaTeX `\label`/`\ref`, `hyperref` | standard |
| Tables, figures, diagrams | `booktabs`, `tikz` in the prose | standard |
| Bibliography | `\cite` + `bib/*.bib` | in-repo |
| Glossary | a `glossary.weft` concept + `glossaries` package | LDD convention |
| Index | chunk index (native) + `makeidx` for prose terms | native + standard |
| Typography & style | `microtype`, chosen font, a prose style guide | `literate-craft.md` |
| Two audiences, one source | `-w tex` (book) / tangle (machine) / `-w md` (GitHub) | weft weave |

**Publishable-quality gate.** A concept is not done if it does not read
straight through, lacks its glossary entry, or fails to cross-reference what
it names. This is the same standard the `literate-programming` skill states:
a reader should understand the system by reading it as a book.


## What the AI must and must not do

**Must:**
- Load the `literate-programming` skill first (Step 0) — it is the
  prerequisite paradigm and weft syntax; it is not auto-imported.
- Start every task with `weft --help` if context is lost, then `-m --prose`.
- Edit only `.weft` source.
- Run all four gates and report `--lint` + `--diff` before claiming done.
- Update prose in the same change as code.
- Reference stable chunks by name instead of re-reading them.

**Must not:**
- Read or edit tangled output.
- Invent a chunk name (the linter will catch it; check first with `-m`).
- Mark a change complete with stale prose or a dirty `--diff`.
- Load whole files when `-e` would return the exact fragment.


## Relationship to the stack skills

LDD is language-agnostic and orchestrates the stack skills:
- `literate-programming` — the paradigm and weft toolkit (prerequisite).
- `111` (Mojolicious), `333` (Go microservices), `444` (Flutter) — the
  per-stack tangle pipelines and conventions.

Use LDD for the *process*; use the stack skill for the *pipeline* of a given
target language.


See [QUICKSTART.md](QUICKSTART.md) for the hands-on workflow (activation,
scaffolding a new project, the daily cycle, and the commands).

See [references/ldd-design.md](references/ldd-design.md) for the Phase 0
walk-through (Frame → Decompose → Sequence → Contract),
[references/ldd-cycle.md](references/ldd-cycle.md) for a worked walk-through
of one unit of work through all six steps, and
[references/ldd-gates.md](references/ldd-gates.md) for the CI gate
definitions and failure examples (including the conformance gate).
