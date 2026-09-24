# The review — step 08, in full

A chapter is not done when it compiles. It is done when five things hold, and
closing a chapter with any of them open is technical debt by another name.

All five are either a plain `weft` command or your own reading. **Nothing runs
them for you**: no runner, no hook, no CI harness, no lock file. The AI runs
them and *reports the output* — "it works" is not a report.

Run them cheapest first. Failing `--lint` means you never pay for the build,
and that ordering is what makes LDD cheap to keep correct.

```
1. weft --lint          refs   — invented names, orphans        (~free)
2. weft --diff          sync   — is the tangled output current? (~free)
3. make | weft --errors build  — the expensive one
4. re-read the prose    prose  — the Prime Directive
5. read the chapter     book   — does it still read as an essay?
```


## 1 · Refs — `weft --lint`

**Passes when** every `@<name@>` resolves to a defined chunk, and every
defined chunk is reachable from an `@o` root.

```
$ weft --lint project.weft
undefined reference: "Auth: verify tokn"  (did you mean "Auth: verify token"?)
unused fragment:     "Auth: legacy hash"
```

This is the primary anti-hallucination check: **anything the AI *invented*
rather than *referenced* fails here**, and the fuzzy suggestion usually names
the chunk that was meant. Orphans are the other half — a chunk defined and
never used is dead literate code, or a seam the assembly forgot to wire.

During Phase 2 (seams named, nothing built yet) orphans are expected. After a
stage closes, they are not.


## 2 · Sync — `weft --diff`

**Passes when** re-tangling would change nothing on disk: the committed output
matches the current `.weft` source.

```
$ weft --diff project.weft
--- a/src/auth.c
+++ b/src/auth.c
@@ ...
   (non-empty → the output is stale: re-tangle and commit)
```

This is the check that catches "done" declared after editing the `.weft` but
before tangling — and generated files that drifted from their source.


## 3 · Build — `make 2>&1 | weft --errors`

**Passes when** the tangled output compiles and the tests pass. On failure,
`--errors` rewrites every `file:line:` into the `.weft` location that produced
it, by reading the section markers embedded in the output — it needs no
`.weft` files at all:

```
$ make 2>&1 | weft --errors
src/auth.c:42: error: undeclared identifier 'retryAfter'
  [weft: literate/concepts/auth.weft:118, scrap 204]
```

The check is green or red; the annotation is what makes red *actionable at the
source*. For a single lookup: `weft -R src/auth.c:42`.

You never open the tangled file to debug it. That is not a style preference —
the generated file is the wrong layer, and fixing it there is a change that
the next tangle silently destroys.


## 4 · Prose — the Prime Directive

> **A task is not complete until its prose has been updated.**

Re-read the prose around **every** chunk you touched and ask the question
plainly: *does it still describe this code?*

- The threshold moved from five to ten — does the paragraph still say five?
- The function now returns early on a case the prose never mentions.
- A new chunk shipped with no prose at all — the most common failure, and the
  worst, because it is invisible until someone needs it.

Two things make this check work in practice:

1. **Report it explicitly.** The AI states, in words, that the prose was
   updated or that it was reviewed and still holds. Silence counts as a
   violation. The reason the report matters is that nothing else can catch
   this one: a hash can tell you prose *changed*, never that it is *true*.
2. **Use `git diff` as the honest evidence.** In the diff of a closed chunk,
   changed lines outside `@{ … @}` are the prose keeping pace. A diff that
   touches only chunk bodies is the shape of the violation.

And treat it as an opportunity, not a tax: every pass is a chance to *enrich*
the prose — an equation, a table, a cross-reference, a figure — so the chapter
explains the code better than it did before.


## 5 · Book — read the chapter straight through

The standard is **publishable quality**. Read the chapter as a reader, not as
its author:

- Does it read straight through, in an order that teaches?
- Does every domain term it uses trace back to the vocabulary chapter, and
  does it have its glossary entry?
- Does it cross-reference what it names, instead of leaving the reader to
  search?
- Is any chunk too big to hold in one screenful, or too small to deserve a
  name?
- Does a reference cross a concept boundary the architecture chapter said was
  forbidden? (`weft -e NAME --callers` shows who reaches into what.)
- Are the gaps closed — or is there a `TODO` that is really a promise to a
  future self?

A chapter that fails this check compiles perfectly and is still not done.
Close the gaps **now**: the next chapter will be written on top of this one,
and debt compounds through a book faster than through a codebase.


## Definition of done

```
refs clean · output in sync · build green · prose updated · chapter reads whole
```

Then commit, at concept granularity, with a message that names the concept
rather than the tangled files. One concept, one story, one commit — and the
next chapter begins on solid ground.
