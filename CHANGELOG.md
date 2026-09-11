# Changelog

All notable changes to weft are documented in this file.

## [2.0.0] - 2026-09-11

### Added

- **Native HTML weave (`-w html`)** — weave a web to a single, self-contained
  `.html` page: doctype, an embedded style sheet (fragment names in an accent
  colour, soft code blocks, light/dark aware), and body, with no \TeX\ and no
  Markdown processor. Fragment references in the prose become clickable links
  to their definitions; scrap footers and the `@f`/`@m`/`@u` indices link via
  `#weftN` anchors. As with every weave, prose is copied through verbatim, so
  an HTML web is authored in HTML prose. Selectable with `-w html` or by
  declaring `@W html` in the source.
- **`--mkdirs`** — an opt-in, recursive `mkdir -p` that creates the parent
  directories of each output file before writing (both the `-p` prefix and any
  subdirectory in an `@o` name). Off by default, so the safe behaviour is
  unchanged. Portable (`mkdir` on POSIX, `_mkdir` on Windows).
- **Single-source version** — the version lives once, in the `@<version@>`
  fragment, which `WEFT_VERSION` splices into the binary. As a
  result `make doc`, `make user-guide`, and `make dist` no longer require a
  `VERSION=` argument; `-V` still overrides `@v` when needed.
- **Rewritten user guide** — restructured into eleven navigable chapters
  (what weft is, quickstart, cheatsheet, the language, section markers,
  running weft, navigating a web, the AI toolkit, debugging, examples, and a
  reference), plus a woven HTML edition. A new `examples/wordcount.weft`
  ships a small literate HTML example.

### Changed

- **Modernized LaTeX weave** — a cleaner, contemporary code presentation:
  uniform chunk-name typography, the scrap number set on the baseline (no
  longer a superscript), no code indentation, no rules or coloured
  background, tighter code leading, and no footer hairlines. Chunk headers
  now carry a definition marker — `≡` for a definition, `+≡` for an additive
  continuation — and the fragment name and its number are set with a wider
  separation. Because the weave output changes, documents will re-render
  differently — hence the major version bump.
- **Prose fragment references are rendered, not expanded** — a `@<name@>`
  written in the prose (outside a scrap) is now typeset as a reference to the
  fragment rather than having its body spliced in.

### Fixed

- **Mid-line fragment splice** — a `@<name@>` reference appearing after
  visible characters on a line (e.g. `#define WEFT_VERSION "@<version@>"`) no
  longer emits a spurious `#line`/section-marker resync; the resync is kept
  only at the start of a line, so the reference splices seamlessly.
- **Output rename failure hygiene** — when the final `rename` of an output
  file fails, weft now removes the stray temporary file, prints a hint
  (`try --mkdirs`), and exits non-zero, instead of leaving the temp behind
  and — in the missing-subdirectory case — exiting silently with status 0.

## [1.1.0] - 2026-07-31

### Changed

- **LaTeX code output now uses the `listings` package** — every code scrap
  (`@{...@}`) is woven as an `lstlisting` environment instead of the old
  `minipage`/`\verb` machinery. This is now the default and only code
  format. The gains: **syntax highlighting** (driven by the scrap's
  language, see below), automatic **line wrapping** (`breaklines`, so long
  lines no longer run off the right margin), and **page breaking** (tall
  scraps split across pages instead of overrunning the bottom). Because
  `listings` reads the body verbatim, special characters (`\ { } $ & # % _
  ^ ~`) need no escaping.

  **Requirement:** the document preamble must load `\usepackage{listings}`
  (weft cannot inject it, as the limbo is written before `\documentclass`).
  Fragment references that appear in the middle of code are typeset by
  briefly escaping out of the listing via `escapeinside={(*<}{>*)}`, which
  weft writes on every `lstlisting`; the delimiter is chosen to never occur
  in real source, and weft warns if a scrap body happens to contain it.

- **Per-scrap syntax highlighting** — the language weft already tracks
  (`@l tag`, `@L`, or file-extension inference) is now passed to
  `lstlisting` as `language=`, for the languages `listings` ships. Untagged
  fragments are typeset without highlighting (still verbatim).

- **`\WEFTlstsetup`** — a convenience macro written to the limbo. Call it
  once, after `\usepackage{listings}`, to apply weft's recommended
  `\lstset` (monospace, `breaklines`, keyword/comment styles). Optional:
  the `escapeinside`/`language` options that make the output correct are
  written per-listing, so references and highlighting work without it.

- **Macros renamed `\NW...` → `\WEFT...`** — every default definition weft
  writes into the document limbo (`\WEFTtarget`, `\WEFTlink`, the
  `\WEFTtxt...` labels, `\WEFTsep`, `\WEFTnotglobal`, `\WEFTuseHyperlinks`,
  plus `\WEFTbegin`/`\WEFTend`) now uses the `WEFT` prefix. Documents that
  redefined the old `\NW...` commands (for hyperref or localisation) must
  rename their overrides. The hypertarget anchor prefix (`weftN`) is
  unchanged.

- The `-l` flag is now a no-op (accepted but ignored): `listings` is always
  used, so the old opt-in and its `\lstinline` substitution are gone.

## [1.0.5] - 2026-07-26

### Added

- **`end_line` in JSON map** — every `defs` entry in `-m` JSON output now
  includes `"end_line"`, giving AI assistants and tools the full line span
  of each scrap definition. Always on — no flag needed.

- **`--bodies` flag** — when combined with `-m`, includes the raw scrap
  source code as a `"body"` field in each `defs` entry. JSON-escaped,
  ready for programmatic consumption.

- **`--prose` flag** — when combined with `-m`, includes the documentation
  paragraph preceding each `@d`/`@o` directive as a `"prose"` field.
  Gives AI assistants the narrative context for every fragment.

- **`--diff` flag** — dry-run tangle that shows a unified diff of what
  would change in each output file without writing anything. Useful for
  previewing changes before committing.

- **`--callers` flag** — when combined with `-e name`, shows the upward
  call chain from `@o` roots down to the named fragment as an indented
  tree. Answers "who uses this fragment?" transitively.

- **`--lint` flag** — static analysis of the literate source: reports
  unused fragments, undefined references, and fuzzy-match suggestions
  for likely typos (Levenshtein distance ≤ 3).

- **`--errors` flag** — reads compiler/linter output from stdin and
  annotates `file:line:` patterns with `.weft` source locations by
  parsing section markers in the tangled output. No `.weft` files needed.
  Example: `make 2>&1 | weft --errors`

## [1.0.3] - 2026-03-07

### Added

- **`--help` flag**: comprehensive help message covering usage, all options,
  examples, directives quick reference, and an AI-specific section.  The
  help text is structured so that both human users and AI assistants can
  orient themselves when they lose context during a session.  Includes:
  - **WHAT IS WEFT?** — explains literate programming and the tangle/weave
    model from scratch.
  - **HOW IT WORKS** — shows `@o`, `@d`, `@<Name@>` with inline examples.
  - **RULES FOR AI ASSISTANTS** — six numbered imperatives (never edit
    tangled output, always edit `.weft` source, how to use `-R`, `-m`,
    `-e`).
  - **STEP-BY-STEP AI WORKFLOW** — the map→extract→edit→tangle→build→debug
    loop.
  - **DIRECTIVES QUICK REFERENCE** — all directives with syntax.

### Changed

- **Error messages reference `--help`**: when weft is invoked without files
  or with an unrecognised flag, the error now says
  `Try 'weft --help' for more information.` instead of repeating a
  cryptic usage line.

- **Man page updated**: `--help` documented in the `weft(1)` man page.

## [1.0.2] - 2026-03-05

### Added

- **Reverse map (`-R file[:line]`)**: new flag that reads section markers
  from a tangled output file and emits JSON mapping each region back to
  its `.weft` source file and line.  With a line number, returns the
  single innermost scrap covering that line; without one, returns all
  scrap regions.  No `.weft` source files are needed — operates entirely
  on the tangled output.  Enables CI/CD error translation and AI agent
  workflows.

### Fixed

- **Inline fragment expansion with `@l txt`**: fragments defined with
  `@l txt` (or any language with no comment style) now expand inline
  without section markers or extra newlines.  Three issues were resolved:
  - `lang_to_flags()` set `location_flag = TRUE` even when the language
    has no comment delimiters (`comment == 0`); now sets it to `FALSE`.
  - "Apply pending language to macro" stored the `@l` tag but never
    called `lang_to_flags()`, so `@d` fragments never got their own
    flags; now mirrors the `@o` behaviour.
  - "Copy macro into..." always used the parent's flags; now temporarily
    overrides with the child's flags when the child has an explicit `@l`
    tag, then restores them.

### Changed

- **Version in document titles via `@v`**: the hardcoded "weft 1.0" in
  the titles of `weft.weft` and `weft-user-guide.weft` is now `weft @v`,
  replaced at weave time by the `-V` flag.

- **`make dist` requires `VERSION=`**: `make dist VERSION=1.0.2` passes
  `-V` to weft when generating PDFs.  Running `make dist`, `make doc`,
  or `make user-guide` without `VERSION=` now produces an error.

## [1.0.1] - 2026-03-04

### Fixed

- **POSIX portability**: added `_POSIX_C_SOURCE 200809L` so that `strdup`
  is declared when compiling under strict C11 mode (`-std=c11`).

- **Out-of-bounds array access** in `prev_char` (search-labels.weft):
  when indexing into the previous slab with a negative offset `k`,
  the expression `SLAB_SIZE - k` produced an index beyond the array
  bounds (e.g. 1025 for `chars[1024]`).  Corrected to `SLAB_SIZE + k`.

- **Uninitialized field** `reader.prev` in the scrap-copy loop
  (scraps.weft): the `Manager` struct was used without initialising
  its `prev` pointer, which could lead to undefined behaviour in
  `backup()`.  Now explicitly set to `NULL`.

- **Implicit fallthrough warnings**: introduced a portable `FALLTHROUGH`
  macro (`__attribute__((fallthrough))` on GCC/Clang, `((void)0)`
  elsewhere) and applied it to every intentional switch-case
  fallthrough across the parser, LaTeX output, Markdown output,
  source I/O, and name-handling modules.

- **`strncpy` truncation warning** in `collect_lang_def` (names.weft):
  replaced `strncpy` + manual null termination with `snprintf`, which
  always null-terminates and does not trigger `-Wstringop-truncation`.

### Changed

- `make bootstrap` now compiles with zero warnings under
  GCC (`-Wall -Wextra -pedantic`) and Clang.

## [1.0] - 2025-12-14

Initial release of weft as a modern fork of nuweb 1.64.
