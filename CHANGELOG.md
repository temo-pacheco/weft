# Changelog

All notable changes to weft are documented in this file.

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
