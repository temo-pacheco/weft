# Changelog

All notable changes to weft are documented in this file.

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
