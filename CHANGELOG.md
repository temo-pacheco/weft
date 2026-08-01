# Changelog

All notable changes to weft are documented in this file.

## [1.1.0] - 2026-07-31

### Changed

- **LaTeX output rewritten** — the woven `.tex` no longer wraps each scrap
  in a `minipage` typeset with per-line `\verb`. Scraps are now plain
  `\ttfamily` blocks that **break across pages** (no more scraps overrunning
  the bottom margin) and whose long lines **wrap with a hanging indent**
  (no more code running off the right margin). Special LaTeX characters in
  code are escaped as the body is copied out; spaces become `\WEFTsp`
  (breakable, fixed-width) and a `\WEFTbrk` break opportunity is emitted
  after underscores and common punctuation so long tokens can still wrap.

- **Macros renamed `\NW...` → `\WEFT...`** — every default definition weft
  writes into the document limbo (`\WEFTtarget`, `\WEFTlink`, the
  `\WEFTtxt...` labels, `\WEFTsep`, `\WEFTnotglobal`, `\WEFTuseHyperlinks`)
  now uses the `WEFT` prefix. Documents that redefined the old `\NW...`
  commands (for hyperref or localisation) must rename their overrides.
  The hypertarget anchor prefix (`weftN`) is unchanged.

- **New layout macros** are written to the limbo and may be redefined to
  tune the appearance: `\WEFTbegin`/`\WEFTend`, `\WEFTcode`/`\WEFTendcode`,
  `\WEFTeol`, `\WEFTsp`, `\WEFTbrk`, the lengths `\WEFTindent` and
  `\WEFThang`, and the penalty `\WEFTbreakpenalty` (default 500, controlling
  how strongly a scrap resists being split across a page).

- The `-l` (listings) flag is now a no-op: the block layout no longer uses
  `\verb`, so the old `\lstinline` substitution it performed is gone.

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
