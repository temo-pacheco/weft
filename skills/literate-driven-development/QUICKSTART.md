# LDD Quickstart

How to run a Literate Driven Development project day to day. Two authors are at
the keyboard: **you** (intent) and the **AI** (drafting and verification). The
skill is the protocol that coordinates them.

## Prerequisites

- The `weft` binary on `PATH` (or `$WEFT`). That is the only tool.
- This skill. **Nothing else** — it carries the whole literate-programming
  paradigm and the complete weft reference in its own `references/`, so there
  is no second skill to load and no step 0.

Nothing is copied into your project either: no helper script, no hook, no lock
file, no CI harness. Pure LDD.

## Activation

The skill triggers on "LDD", "literate driven development", "programación
literaria con IA", or an explicit `/literate-driven-development`. Once it is
active, everything needed is in hand: the workflow below, and the syntax and
craft references beside it.

## Phase 1 — Frame the domain (no code, no tools)

Just you, the AI, and one plain document — Markdown is fine. Nothing is woven
yet; this is the cheapest level, where changing your mind costs nothing.

```
01  Determine the domain        what this is, its purpose, its scope
02  Analyze the domain          concepts, entities, invariants, vocabulary
03  Determine the architecture  the shape + every decision with its rationale
04  Plan incremental stages     stages that each run and are proven
```

Iterate as a dialog until the concept graph is sound. Ask the AI to argue with
it: which invariant is unstated, which concept owns what, which stage does not
actually prove anything.

**You get:** a simple domain document, in any format.

## Phase 2 — Scaffold the project

```bash
mkdir my-project && cd my-project && git init
mkdir -p literate/concepts src

# chapter skeletons (adapt, then delete the @c guidance comments)
SKILLS=~/.claude/skills/literate-driven-development
cp $SKILLS/templates/00-domain.weft          literate/
cp $SKILLS/templates/01-domain-analysis.weft literate/
cp $SKILLS/templates/02-architecture.weft    literate/
cp $SKILLS/templates/03-stages.weft          literate/
```

**05 · Scaffold the first chapters** — carry Phase 1 into `.weft` prose: the
project domain, the domain analysis, the architecture, the stages. Prose only:
no `@o`, no code-bearing `@d`. They weave into the book and tangle to nothing.

You also write the two structural files (copy and adapt the ones in
`examples/ldd-ledger/`):

- `project.weft` — the master: formatting preamble + `@i` includes.
- `literate/assembly.weft` — the only file with `@o`; it routes chunks to
  output paths, so no concept knows where its code lands.

**06 · Generate the first document** — weave the book and read it:

```bash
weft -w tex project.weft && pdflatex project.tex   # → project.pdf
weft -w md  project.weft                            # → Markdown
weft -w html project.weft                           # → HTML
```

**You get:** the first woven book — your design, in one readable document,
before a line of code exists.

## Phase 3 — Write the book, chapter by chapter

One stage per chapter, one substage at a time.

```bash
# 07 · WRITE THE CHAPTER
#
# a) NARRATE (you): prose first, then name the chunk, e.g. in
#    literate/concepts/ledger.weft →  @d Ledger: post @{ @% TODO @}

# b) ORIENT (AI): directed context — never read whole files
weft -m --prose project.weft                     # structure + prose
weft -e "Accounts: apply delta" --callers project.weft

# c) FILL (AI): write the chunk body in the .weft — never in src/

# d) VERIFY: tangle, then build and run
weft project.weft                                # .weft → src/
cc -Wall src/*.c -o app && ./app
make 2>&1 | weft --errors                        # errors → exact .weft line

# e) RECONCILE (AI): re-read the prose of every chunk touched; update it

# 08 · REVIEW, THEN ADVANCE
weft --lint project.weft          # refs: invented names, orphans
weft --diff project.weft          # sync: empty = tangled output current
make 2>&1 | weft --errors         # build: green
#   + re-read the prose (Prime Directive)
#   + read the chapter straight through (does it read as a book?)
git commit                        # one concept, one story, one commit

# close every gap now — no technical debt — then start the next chapter
```

**You get:** the complete literate code — book and program, growing together.

## Definition of done

```
refs clean · output in sync · build green · prose updated · chapter reads whole
```

All five are plain `weft` commands or your own reading. The AI must **report**
the first three, not merely assert them. The Prime Directive is
non-negotiable: **a task is not complete until its prose is updated.**

## The one operating rule

> Edit only `.weft` files, never the tangled output. A task does not close
> without updated prose.

## Worked references

- `examples/ldd-ledger/` in this repository — a small double-entry ledger in C:
  design chapters, a walking skeleton, and a typeset book from one source.
- **A Sophisticated Audio Editor** — a non-destructive, modular audio engine
  built this way, with its domain document, its first woven book and the full
  repository published stage by stage:
  <https://gitlab.polinesso.org/literate-programming/the-ldd-audio-project>
- The `kcc` compiler — the same workflow, on a larger system.

## The commands you actually use

```
weft project.weft                    tangle: .weft → output files
weft -w tex|md|html project.weft     weave: the book
weft -m --prose project.weft         the map: structure + prose (orient)
weft -e NAME --callers project.weft  one fragment + who calls it
weft --lint project.weft             invented refs, orphan chunks
weft --diff project.weft             is the tangled output current?
make 2>&1 | weft --errors            compiler errors → .weft source lines
weft -R src/file.c:47                one reverse lookup
weft --help                          everything, if context is lost
```
