# The weft skills

Two Claude Code skills ship with weft. They overlap on purpose, and the rule
that governs them is one line:

> **`literate-programming` is the paradigm alone.
> `literate-driven-development` is the paradigm *plus* the method — and it
> carries the paradigm inside it, so it stands alone too.**

Load **one**. Never both.

```
skills/
├── literate-programming/   the paradigm, the metalanguage, the tool
│   ├── SKILL.md
│   └── references/         weft-syntax · naming-conventions · literate-craft
│                           literate-philosophy · concept-architecture
│                           debugging-tangled-code · build-pipeline
│                           worked-example
└── literate-driven-development/   the method (LDD) — self-contained
    ├── SKILL.md            the three phases, the review, the Prime Directive
    ├── QUICKSTART.md
    ├── MANIFESTO.md
    ├── templates/          the four design-chapter skeletons
    └── references/
        ├── ldd-design · ldd-cycle · ldd-review · ldd-notes     ← the method
        └── the eight files above, carried verbatim             ← the paradigm
```

## Which one

| You want | Skill | Because |
|---|---|---|
| To write `.weft`: syntax, chunk names, tangle, weave, debugging | `literate-programming` | it is the language, with no process attached |
| To build a project this way with an AI, start to finish | `literate-driven-development` | it adds the three phases and the review — and already contains everything the other one has |

`literate-driven-development` has **no prerequisite**. There is no "load the
other skill first" step: if it asked for one, a single missed call would leave
the AI writing weft it had never read the rules for.

## The duplication, and how it is kept honest

The eight paradigm references live in `literate-programming/references/` and
are **copied verbatim** into `literate-driven-development/references/`.
`literate-programming/` is the source of truth; the copies are derived.

After editing any of them, re-sync:

```sh
cp skills/literate-programming/references/*.md \
   skills/literate-driven-development/references/
```

To check that nothing drifted:

```sh
for f in skills/literate-programming/references/*.md; do
  diff -q "$f" "skills/literate-driven-development/references/$(basename $f)"
done
```

Silence means they agree. The `ldd-*.md` files exist only on the method side
and are never copied back — the method does not leak into the pure paradigm.

## Installing

```sh
cp -r skills/literate-programming        ~/.claude/skills/
cp -r skills/literate-driven-development ~/.claude/skills/
```

Both directories keep their own names, which is what each `SKILL.md` declares
in its `name:` field — a skill's directory name is its identity, and the two
must agree. Re-copy after every change: the installed copies are snapshots,
and a stale one is how you end up running last month's method.

## Author them the way weft is authored

These are prose files about writing prose files. The standard they describe
applies to them: if a section no longer tells the truth about the tool, it is
not documentation, it is a lie with syntax highlighting.
