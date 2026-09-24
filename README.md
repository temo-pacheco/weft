# weft

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
- **`@W` weave format declaration** — `@W md` or `@W tex` in the
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
- **Reverse map (`-R`)** — given a tangled output file and
  optionally a line number, emits JSON mapping back to the `.weft`
  source. No `.weft` files needed — operates on section markers
  already in the tangled output. Enables CI/CD error translation
  and AI agent workflows.
- **Mid-line inline expansion** — when a fragment reference `@<name@>`
  appears mid-line (after visible characters on the same line), section
  markers, `#line` directives, and fragment-name comments are
  automatically suppressed. The fragment content is spliced in
  seamlessly. At start-of-line references (including indented ones),
  markers are emitted normally.
- **`--help` for humans and AI** — comprehensive help output covering
  all options, usage examples, directive quick reference, and an
  AI-specific section with rules and step-by-step workflow.
- **`end_line` in JSON map** — each `defs` entry in `-m` JSON now
  includes `"end_line"` for the full line span of every scrap.
- **`--bodies`** — include raw scrap code in `-m` JSON output.
- **`--prose`** — include preceding documentation in `-m` JSON output.
- **`--diff`** — dry-run tangle showing unified diff of changes (no
  files written).
- **`--lint`** — static analysis: unused fragments, undefined refs,
  fuzzy-match suggestions for typos.
- **`--callers`** — with `-e`, show call chain from `@o` roots to
  the named fragment.
- **`--errors`** — annotate compiler errors from stdin with `.weft`
  source locations using section markers.
- **62 automated tests**.

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
make check            # 3. run all 62 tests
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

### Getting help

```sh
./weft --help             # comprehensive reference (options, examples, AI workflow)
```

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

### Mid-line references

When a fragment reference `@<name@>` appears **mid-line** (after at
least one visible character on the same line), markers are automatically
suppressed. This lets you define small constant fragments and splice
them inline without breaking the surrounding expression:

```latex
@d port @{4000@}
@d host @{"localhost"@}

@l c
@o config.c -s
@{const char *host = @<host@>;
int port = @<port@>;
@}
```

Tangling produces clean inline expansion:

```c
const char *host = "localhost";
int port = 4000;
```

If the same reference appears at the **start of a line** (even indented),
it is treated as a block expansion and markers are emitted normally.
No special flags or directives are needed — the position on the line
is the only thing that matters.

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
| `-R file[:line]` | Reverse map: emit JSON tracing tangled output back to `.weft` source |
| `-r` | Enable hyperlinks (hyperref) in LaTeX output |
| `-h opts` | Provide hyperref package options |
| `-l` | Use `listings` package for scrap formatting |
| `-p path` | Prepend path to output file names |
| `-I path` | Add directory to include search path |
| `-V str` | Set version string for `@v` substitution |
| `--bodies` | Include raw scrap body text in `-m` JSON output |
| `--prose` | Include preceding documentation in `-m` JSON output |
| `--diff` | Show unified diff of what tangle would change (no files written) |
| `--lint` | Static analysis: unused fragments, undefined refs, typo suggestions |
| `--callers` | With `-e`, show call chain from `@o` roots to fragment |
| `--errors` | Annotate compiler errors from stdin with `.weft` source locations |
| `--help` | Comprehensive help: options, examples, directives, AI workflow |
| `--version` | Show version number |

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
skills/                   <- Claude Code skills (see skills/README.md)
├── literate-programming/        <- the paradigm alone
└── literate-driven-development/ <- the method, self-contained
bib/                      <- bibliography for make doc
test/
└── 00/                   <- 62 automated tests
```

## The skills

The `skills/` directory ships two Claude Code skills. They overlap on
purpose, and one line governs them:

> **`literate-programming` is the paradigm alone.
> `literate-driven-development` is the paradigm *plus* the method — and it
> carries the paradigm inside it, so it stands alone too.**

Load one. Never both.

### `literate-programming` — pure literate programming

The language and the tool, with no process attached: how to write `.weft`,
how to name chunks, how to organize a system by concept, how to build, and how
to debug. Use it when you want literate programming itself.

- **Complete weft syntax** — every directive, flag, scrap mode and CLI option
- **Architecture guide** — concept-oriented organization, the assembly pattern
- **Build pipeline** — tangle → format → build, per language
- **Debugging workflow** — tracing errors from tangled output back to `.weft`
- **AI navigation** — `-m` (JSON map), `-e` (extraction), `-R` (reverse map)
- **Naming conventions** — chunk naming, narrative rules, anti-patterns
- **Literate craft** — writing the prose to publishable quality
- **Worked example** — one concept spanning Go + Flutter + SQL + Protobuf

### `literate-driven-development` — the LDD method

The method for building a whole project this way **with an AI**, in three
phases — *Frame the domain → Scaffold the project → Write the book, chapter by
chapter* — under one rule: **a task is not complete until its prose is
updated**.

It is **self-contained**: it carries all eight paradigm references above,
verbatim, alongside its own. There is no prerequisite skill and no "load this
first" step, and its only tool is the `weft` binary — no scripts, no task
runner, no hooks, no CI harness.

- `SKILL.md` — the three phases, the review checklist, the Prime Directive
- `QUICKSTART.md` — the commands you actually type, day to day
- `MANIFESTO.md` — why the method exists
- `templates/` — the four design-chapter skeletons
- `references/` — the method (`ldd-design`, `ldd-cycle`, `ldd-review`,
  `ldd-notes`) plus the paradigm, carried whole

A complete project built this way is in `examples/ldd-ledger/`: a double-entry
ledger in C, its design chapters, and its typeset book — from one source.

### Installing

```sh
cp -r skills/literate-programming        ~/.claude/skills/
cp -r skills/literate-driven-development ~/.claude/skills/
```

Both keep their own directory names — a skill's directory name is its
identity, and it must match the `name:` its SKILL.md declares. Re-copy after
every change: the installed copies are snapshots.

See [skills/README.md](skills/README.md) for how the shared references are kept
in sync between the two.

### Example prompts

```
> Create a literate project for a REST API in Go with auth and payments

> I have a bug at server.js:47, help me trace it back to the .weft source
>   (uses: weft -R server.js:47)

> Add a notifications concept to my literate project

> Show me the dependency graph of my literate project

> Let's build this with LDD: frame the domain first, then scaffold the book

> I have an existing Express app in src/. Reorganize it into .weft concept files so I can benefit from literate programming
```

## License

BSD 3-Clause. See the license headers in `weft.weft` for details.

Original nuweb code copyright (c) 1996, Preston Briggs.
weft extensions copyright (c) 2024-2026, Cuauhtemoc Pacheco.
