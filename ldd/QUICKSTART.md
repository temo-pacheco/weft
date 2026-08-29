# LDD Quickstart

How to actually use Literate Driven Development day to day. Two people are at
the keyboard: **you** (write intent) and the **AI** (fill and verify). The
skill is the protocol that coordinates them.

## Prerequisites

- The `weft` binary on `PATH` (or set `$WEFT`).
- `python3` (for the `ldd` CLI — stdlib only, nothing to install).
- `templates/ldd.py` copied into your project.

## Activation

The skill triggers on "LDD", "literate driven development", "programación
literaria con IA", or an explicit `/ldd`. Once active the AI follows the
protocol below. **Step 0 for the AI: load the `literate-programming` skill
first** — it carries the weft syntax and the paradigm this one builds on.

## The two roles

| Phase | You (human) | AI |
|---|---|---|
| Phase 0 | frame the domain & problem | propose concepts, graph, seams |
| NARRATE | write prose + **name empty chunks** | — |
| ORIENT / FILL / VERIFY / RECONCILE | review | execute |
| Prime Directive | demand it | obey it (never closes without prose) |

You define *what* and *why* in prose; the AI produces *how* and verifies it.

## Flow A — start a new project

```bash
mkdir my-project && cd my-project && git init
mkdir -p literate/concepts src

# tooling + design-chapter skeletons
cp ~/.claude/skills/ldd/templates/ldd.py .
cp ~/.claude/skills/ldd/templates/00-domain.weft       literate/
cp ~/.claude/skills/ldd/templates/01-architecture.weft literate/
cp ~/.claude/skills/ldd/templates/02-roadmap.weft      literate/

# enforce the Prime Directive on every commit
cp ~/.claude/skills/ldd/templates/pre-commit .git/hooks/ && chmod +x .git/hooks/pre-commit
```

Then do **Phase 0** — fill the three design chapters in prose, no code yet:

1. `00-domain.weft` — the domain and the problem, and the vocabulary.
2. `01-architecture.weft` — concepts, decision records, and the
   `% ARCH-ALLOW:` dependency policy.
3. `02-roadmap.weft` — the stages, the concept ledger, and the **names of the
   seam chunks**.

Iterate these (you + AI) until the concept graph is sound. This is the cheap
level: prose and names, no build, few tokens.

You also need a **master** `project.weft` (LaTeX preamble + `@i` includes) and
an **assembly** `literate/assembly.weft` (the only file with `@o`). Copy the
ones from `examples/ldd-ledger/` and adapt.

## Flow B — the per-change cycle (what you run daily)

```bash
# 1. NARRATE (you): write the concept prose and name the empty chunk, e.g.
#    literate/concepts/ledger.weft →  @d Ledger: post @{ @% TODO @}

# 2. ORIENT (AI): directed context, never read whole files
python3 ldd.py context "Ledger"
weft -e "Accounts: apply delta" --callers project.weft

# 3. FILL (AI): implement the chunk body in the .weft (never in src/)

# 4. VERIFY: tangle, then build/run
mkdir -p src && weft project.weft           # .weft → src/
cc -Wall src/*.c -o app && ./app            # build + run
make 2>&1 | weft --errors                   # compiler errors → exact .weft line

# 5. RECONCILE (AI): re-read the prose around every touched chunk; update it

# 6. CLOSE: the five gates + record the Prime Directive baseline
python3 ldd.py verify --build "cc -Wall src/*.c -o app"
python3 ldd.py prose lock       # or: prose lock --affirm  (code changed but
                                # the prose is genuinely still correct)
git commit                      # pre-commit blocks if prose fell behind
```

## Definition of done — five green gates

```
ldd verify  →  1 Refs · 2 Sync · 3 Conformance · 4 Build · 5 Prose
```

A task is **not done** until `ldd verify` is green. Gate 5 (the Prime
Directive) is non-negotiable: if you changed code without touching prose it
fails, and it only passes with an explicit, logged `prose lock --affirm`.

## The book (same source)

```bash
weft -w tex project.weft && pdflatex project.tex   # → project.pdf (the book)
weft -w md  project.weft                            # → Markdown for GitHub
```

## The one operating rule

> Edit only `.weft` files, never `src/`. A task does not close without updated
> prose and a green `ldd verify`.

## A worked reference

`examples/ldd-ledger/` is a complete project built this way: Phase 0 design
chapters, a walking-skeleton double-entry ledger in C, all five gates green,
and a typeset book — from one source. Read it end to end as a model.

## The `ldd` commands

```
ldd verify [--build "CMD"]   run the five gates in order
ldd conformance              gate 3 standalone
ldd prose check              gate 5: the Prime Directive (mechanical)
ldd prose lock [--affirm]    record the reviewed prose/code state
ldd context CONCEPT          orient: directed context pack for a concept

# globals:  --main FILE (default $LDD_MAIN or project.weft)
#           --weft PATH (default $WEFT or weft on PATH)
```
