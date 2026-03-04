# weft 1.0

**A Modern Literate Programming Tool**

weft is a language-agnostic literate programming tool derived from
[nuweb](http://nuweb.sourceforge.net/) 1.64 by Preston Briggs. You write
a single `.weft` file that combines documentation with code in any programming
language. weft **tangles** it into executable source files and, when asked,
**weaves** it into typeset documentation (LaTeX or Markdown).

## Features

### Core (inherited from nuweb)

- **Any programming language** — C, Python, JavaScript, Go, Rust, Haskell,
  or anything else. One tool, no language-specific plugins.
- **Single source of truth** — one `.weft` file produces both runnable code
  and typeset documentation. No synchronization problems.
- **Named fragments** — define code in the order that makes sense for
  human understanding, not the order the compiler demands. Fragments
  can be defined, extended, and composed freely.
- **Fragment parameters** — fragments accept arguments (`@1`..`@9`),
  enabling reusable code templates without preprocessor tricks.
- **Automatic cross-references** — the woven documentation includes
  indices of file definitions, fragment names, and user-specified
  identifiers, with clickable hyperlinks.
- **Sections** — `@s`/`@S` partition large documents into independent
  namespaces, keeping fragment names and identifiers from colliding
  across different parts of a project.
- **Global fragments** — `@d+` makes a fragment accessible across all
  sections; `@+` and `@-` export and import identifiers between sections.
- **Multiple output files** — a single `.weft` file can produce any number
  of output files via `@o` directives.
- **Include files** — `@i` splits large webs into manageable pieces,
  with up to 10 levels of nesting and configurable search paths (`-I`).
- **Per-file flags** — `#line` directives (`-d`), comment styles
  (`-cc`, `-c+`, `-cp`), tab control (`-t`), and indentation
  suppression (`-i`) on each `@o` directive.
- **Quoted fragments** — `@q` defines fragments whose contents are not
  expanded, useful for embedding literal `@` sequences.
- **Version stamping** — `-V string` sets a version that `@v` inserts
  into output files.
- **Hyperref support** — `-r` and `-h options` enable clickable
  cross-references in the PDF.
- **Listings integration** — `-l` uses the LaTeX `listings` package
  for pretty-printed scraps.
- **Portable C** — builds on any POSIX system with a C compiler.

### New in weft

- **Tangle by default** — `weft myfile.weft` tangles only. Weave is
  requested explicitly with `-w`.
- **`@@W` weave format declaration** — `@@W md` or `@@W tex` in the
  source file declares the weave format. `weft -w` uses this; `-w md`
  or `-w tex` on the command line overrides it.
- **Markdown weave output** — `weft -w md` produces a `.md` file
  that renders on GitHub, GitLab, and any Markdown viewer, without
  requiring a TeX installation.
- **Knuth-style section markers** — every scrap in the tangled output
  is wrapped with opening and closing comments that reference the
  source file and line number, making it easy to trace errors back
  to the literate source.
- **Language auto-detection** — `@l` tags and file extensions
  automatically select the right comment style and `#line` directives
  for over 250 built-in languages.
- **Custom language definitions** — the `@L` command lets you define
  comment styles for languages not in the built-in table, or override
  defaults.
- **Unified `.weft` extension** — one extension for all literate
  source files, regardless of weave format.
- **Modular literate source** — weft's own source is organized by
  concept across 14 `.weft` files, not one monolithic file.
- **Clean C11 build** — compiles with zero warnings under
  `-std=c11 -Wall -Wextra -pedantic`.
- **`@l` language tag** — `@l python` before `@o` or `@d` sets the
  language explicitly. Used for section markers in the tangle and
  `class="language-X"` in the Markdown weave.
- **`-s` per-file flag** — suppresses section markers and `#line`
  directives while preserving the language tag for Markdown highlighting.
- **Syntax highlighting in Markdown** — the Markdown weave emits
  `<code class="language-X">` automatically, enabling highlight.js,
  Prism, and GitHub syntax colouring.
- **57 automated tests**.

## Quick Start

### Build from a fresh clone

```sh
git clone <repo-url> weft
cd weft
make bootstrap        # src/*.c → build/*.o → ./weft
```

The repository includes pre-generated `.c` files in `src/` so you can
build weft with nothing more than a C compiler — no prior installation
needed. Object files go to `build/`, the binary lands in the project root.

### Development workflow

```sh
vim literate/*.weft   # 1. edit source of truth
make weft             # 2. tangle → src/*.c → build/*.o → ./weft
make check            # 3. run all 57 tests
```

### Documentation

```sh
make doc              # → weft.pdf        (developer reference with code)
make user-guide       # → weft-user-guide.pdf
```

### Release

```sh
make dist             # bootstrap (if needed) → tangle → compile → test → docs
```

Produces `weft`, `src/*.c` (bootstrap), `weft.pdf`, and
`weft-user-guide.pdf`. No intermediate files are left behind.

### Cleanup

```sh
make clean            # remove build/ (object files)
make veryclean        # remove build/ + weft binary + PDFs + LaTeX intermediates
```

**Rule of thumb:** never edit `src/*.c` or `build/`. All changes go
through `.weft` files.

### Basic usage

```sh
# Tangle only (default behavior)
./weft myprogram.weft

# Weave using format declared by @@W in the source
./weft -w myprogram.weft

# Weave to Markdown (override, ignores @@W)
./weft -w md myprogram.weft

# Weave to LaTeX (override, ignores @@W)
./weft -w tex myprogram.weft

# Tangle + weave
./weft -w md myprogram.weft   # tangles AND weaves

# Suppress tangle (weave only)
./weft -o -w md myprogram.weft
```

### A minimal example

```latex
\documentclass{article}
\begin{document}

Here is a complete C program:

@o hello.c
@{#include <stdio.h>

int main() {
    @<Print greeting@>
    return 0;
}
@}

The greeting is straightforward:

@d Print greeting
@{printf("Hello, literate world!\n");
@}

\end{document}
```

Running `weft example.weft` produces `hello.c`:

```c
#include <stdio.h>

int main() {
    printf("Hello, literate world!\n");
    return 0;
}
```

### Markdown input

weft also accepts Markdown-based literate files. The same example in Markdown style:

````markdown
# Hello

Here is a complete C program:

@l c
@o hello.c
@{#include <stdio.h>

int main() {
    @<Print greeting@>
    return 0;
}
@}

The greeting is straightforward:

@d Print greeting
@{printf("Hello, literate world!\n");
@}
````

The `@l c` tag automatically enables `/* */` comments and
`#line` directives in the tangled output.

## Section Markers

When a language is detected (from `@l` tags, `@L` definitions,
or file extensions), weft wraps each scrap in the tangled output
with opening and closing markers:

```js
// {3: routes.weft:42}
app.get("/api/users", async (req, res) => {
  // {7: database.weft:18}
  const users = await db.query("SELECT * FROM users");
  // {:7}
  res.json(users);
});
// {:3}
```

For C/C++, `#line` directives are also generated so the compiler
reports errors against the `.weft` source directly.

The marker format adapts to each language's comment syntax:

| Style | Languages (examples) | Opening | Closing |
|-------|---------------------|---------|---------|
| `/* */` | C, CSS | `/* {1: f.weft:5} */` | `/* {:1} */` |
| `//` | JS, Go, Rust, Java | `// {1: f.weft:5}` | `// {:1}` |
| `#` | Python, Shell, Ruby | `# {1: f.weft:5}` | `# {:1}` |
| `--` | Lua, SQL, Haskell | `-- {1: f.weft:5}` | `-- {:1}` |
| `<!-- -->` | HTML, XML | `<!-- {1: f.weft:5} -->` | `<!-- {:1} -->` |

## Custom Languages (`@L`)

For languages not in the built-in table, define them in the preamble:

```
@L nim #          % Nim uses # comments
@L glsl // +d     % GLSL uses // and supports #line
@L c //           % override built-in: use // for C
```

Syntax: `@L name style [+d]` where *style* is `//`, `#`, `--`,
`/*`, or `<!--`, and `+d` enables `#line` directives.

## Command-Line Flags

| Flag | Effect |
|------|--------|
| `-t` | Suppress weave output (even if `-w` is given) |
| `-o` | Suppress tangled output files |
| `-c` | Overwrite output files without comparing |
| `-v` | Verbose progress reporting |
| `-n` | Sequential scrap numbering (instead of page numbers) |
| `-s` | Suppress scrap lists after each scrap |
| `-d` | List dangling identifier references |
| `-x` | Include cross-references in output file comments |
| `-w [fmt]` | Weave: `-w` alone uses `@@W`; `-w md` or `-w tex` overrides |
| `-m` | Emit JSON map of document structure to stdout (no tangle/weave) |
| `-e name` | Extract fragment and dependencies as Markdown to stdout |
| `-r` | Enable hyperlinks (hyperref) in LaTeX output |
| `-h opts` | Provide hyperref package options |
| `-l` | Use `listings` package for scrap formatting |
| `-p path` | Prepend path to output file names |
| `-I path` | Add directory to include search path |
| `-V str` | Set version string for `@v` substitution |

## Output File Flags

Flags on `@o` directives control per-file behavior:

| Flag | Effect |
|------|--------|
| `-d` | Generate `#line` directives |
| `-i` | Suppress fragment indentation |
| `-t` | Suppress tab expansion |
| `-cc` | C comments (`/* */`) |
| `-c+` | C++ comments (`//`) |
| `-cp` | Shell/Perl comments (`#`) |
| `-s` | Suppress section markers and `#line` directives |

Explicit flags override auto-detection.

## Project Structure

weft is itself a literate program. The source is organized by concept:

```
weft.weft                 <- master file (includes all others)
literate/                 <- source of truth (.weft files)
├── introduction.weft     <- user-facing documentation (Chapter 1)
├── architecture.weft     <- architecture and limits
├── main.weft             <- main(), command-line parsing
├── parser.weft           <- pass 1: scanning and parsing
├── names.weft            <- name table, language table, @L
├── scraps.weft           <- scrap storage and tangle output
├── output-files.weft     <- output file generation
├── latex-output.weft     <- LaTeX weave pass
├── markdown-output.weft  <- Markdown weave pass
├── source-io.weft        <- source file I/O
├── indexes.weft          <- index generation
├── search-labels.weft    <- label cross-references
├── arena.weft            <- arena memory allocator
└── man-page.weft         <- UNIX man page
src/                      <- generated C (bootstrap for fresh clones)
├── main.c, pass1.c, ...
└── global.h
build/                    <- object files (created by make, gitignored)
skill/                    <- Claude Code skill for literate programming
├── SKILL.md              <- main skill reference
└── references/           <- detailed guides
bib/                      <- bibliography for make doc
test/
└── 00/                   <- 57 automated tests
```

## Literate Programming Guide

The `skill/` directory is a self-contained reference for literate
programming with weft. It is useful both as documentation for humans
and as a skill for AI assistants.

### Contents

- **Complete weft syntax** — all commands, flags, scrap modes, and CLI options
- **Architecture guide** — concept-oriented organization, assembly pattern
- **Build pipeline** — justfile templates for Go, Rust, Python, Flutter, etc.
- **Debugging workflow** — tracing errors from tangled output back to `.weft` source
- **AI navigation** — using `-m` (JSON map) and `-e` (fragment extraction)
- **Naming conventions** — chunk naming, narrative rules, anti-patterns
- **Worked example** — a single concept spanning Go + Flutter + SQL + Protobuf

### As documentation

Read `skill/SKILL.md` for an overview, then dive into `skill/references/`
for detailed guides:

```
skill/
├── SKILL.md                          <- overview and quick start
└── references/
    ├── weft-syntax.md                <- complete syntax reference
    ├── concept-architecture.md       <- organizing code by concept
    ├── build-pipeline.md             <- build automation templates
    ├── debugging-tangled-code.md     <- tracing errors to .weft source
    ├── literate-philosophy.md        <- LP philosophy and principles
    ├── naming-conventions.md         <- chunk naming and narrative rules
    └── worked-example.md             <- cross-system example
```

### As a Claude Code skill

Install it so [Claude Code](https://claude.com/claude-code) can create,
navigate, and maintain literate projects autonomously:

```sh
cp -r skill/ ~/.claude/skills/literate-programming/
```

Example prompts:

```
> Create a literate project for a REST API in Go with auth and payments

> I have a bug at server.js:47, help me trace it back to the .weft source

> Add a notifications concept to my literate project

> Show me the dependency graph of my literate project

> Refactor this monolithic .weft into separate concept files
```

## License

BSD 3-Clause. See the license headers in `weft.weft` for details.

Original nuweb code copyright (c) 1996, Preston Briggs.
weft extensions copyright (c) 2024-2026, Cuauhtemoc Pacheco.
