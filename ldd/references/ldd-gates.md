# LDD gates — definitions and failure examples

A change is **not done** until all five gates pass. They are run by
`ldd verify` (the `templates/ldd.py` CLI — Python stdlib only) and by CI. Each
maps to a real weft flag or a mechanical check, not a matter of opinion.

## Gate 1 — Refs (`weft --lint`)

**Passes when** every `@<name@>` resolves to a defined chunk and every
defined chunk is reachable from an `@o` root.

**Catches:**
- *Hallucinated references* — the AI invents `@<Auth: verify tokn@>`. Lint
  reports the undefined ref and fuzzy-suggests `Auth: verify token`.
- *Orphan chunks* — a chunk defined but never used (dead literate code).

```
$ weft --lint project.weft
undefined reference: "Auth: verify tokn"  (did you mean "Auth: verify token"?)
unused fragment:     "Auth: legacy hash"
```

This is the primary anti-hallucination gate: anything the AI *invented*
rather than *referenced* fails here.

## Gate 2 — Sync (`weft --diff`)

**Passes when** re-tangling would change nothing on disk — i.e. the committed
output matches the current `.weft` source.

**Catches:**
- Claiming "done" after editing `.weft` but forgetting to re-tangle.
- A committed generated file that drifted from its source.

```
$ weft --diff project.weft
--- a/src/auth.go
+++ b/src/auth.go
@@ ...
   (nonempty diff → gate FAILS: output is stale)
```

In CI this is the check that forbids merging source and generated output that
disagree.

## Gate 3 — Build (`make 2>&1 | weft --errors`)

**Passes when** the tangled output compiles and tests pass. On failure,
`--errors` rewrites every `file:line:` into the `.weft` source location by
reading the section markers embedded in the output — no `.weft` files needed.

```
$ make 2>&1 | weft --errors
src/auth.go:42: error: undeclared identifier 'retryAfter'
  [weft: literate/concepts/auth.weft:118, scrap 204]
```

The gate is green/red; the annotation makes red *actionable at the source*.

## Gate — Conformance (`weft -m` vs declared graph)

**Passes when** every cross-concept edge in the real code graph is permitted
by the architecture chapter. Run by `ldd conformance` (ordered as Gate 3 in
`ldd verify`, before the expensive build).

**How it works:** the architecture chapter declares allowed edges as LaTeX
comments, invisible in the woven book:

```
% ARCH-ALLOW: ledger -> accounts
% ARCH-ALLOW: reporting -> ledger
```

The real graph comes from `weft -m`: a chunk's `used_by` list plus a
scrap→concept map (built from every chunk's `defs`) yields, for each edge, the
*using* concept → the *used* concept. The concept is the text before a chunk
name's first `:`. Any real edge not in the allowed set is a violation.

```
$ ldd conformance
  ✗ forbidden edge: accounts -> ledger
✗ GATE FAILED: 1 architecture violation(s).
```

When it fails you have exactly two honest choices: the code crossed a boundary
it shouldn't (fix the code), or the architecture genuinely changed (add the
`% ARCH-ALLOW:` and say why in prose). Either way the decision is explicit and
versioned. If no `% ARCH-ALLOW:` policy exists yet, the gate passes with a
notice — Phase 0 has not declared the graph.

## Gate — Prose (the Prime Directive, `ldd prose`)

**A task is not done until its prose is updated.** This gate makes that rule
mechanical for the one failure a machine can detect, and delegates the rest to
the reviewer.

**Mechanical half — `ldd prose check`.** A `.ldd-prose-lock` manifest
stores, per fragment, a hash of its code and a hash of its prose (like a
package-lock). The gate **fails** when:
- a fragment's code hash changed but its prose hash did **not** — you edited
  the derived layer and left the source stale; or
- a **new code chunk ships with no prose**.

```
$ ldd prose check
✗ PRIME DIRECTIVE VIOLATION — prose is stale:
    • Net: port  (code changed, prose unchanged)
```

There is no silent skip. The only way past a genuine no-prose-change is an
explicit, logged affirmation:

```
$ ldd prose lock --affirm    # appends to .ldd-prose-affirmations
```

`ldd prose lock` without `--affirm` **refuses** to bless a
code-changed-prose-unchanged fragment. Skipping the prose is therefore always a
conscious, auditable act that shows up in the diff and the affirmation log.

**Semantic half — the reviewer (human or AI).** The lock cannot judge whether
prose that *did* change now tells the *truth*. Feed the changed fragments to a
reviewer:

```
weft -m --prose --bodies project.weft | jq '.fragments'
```

Ask: does the prose still describe the code? Does the concept read straight
through? Does it have its glossary entry and cross-reference what it names?

A chunk changed with stale prose is a failing build in LDD, even though it
compiles. Enforce locally with the `pre-commit` hook so the sin never reaches
history.

---

## The gate order matters

Run cheap-and-structural before expensive-and-behavioural:

```
1. weft --lint         (refs        — catches invented names instantly, ~free)
2. weft --diff         (sync        — catches stale output, ~free)
3. ldd conformance    (conformance — catches boundary violations, ~free)
4. make | --errors     (build       — the expensive one)
5. prose review        (meaning     — human/AI judgement)
```

Failing an early gate means you never pay for the build. This ordering is what
makes LDD cheap to keep correct.
