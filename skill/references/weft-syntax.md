
# Complete weft Guide --- Skill Reference

## Table of Contents

1. [What is weft](#1-what-is-weft)
2. [Philosophy and advantages](#2-philosophy-and-advantages)
3. [Major Commands](#3-major-commands)
4. [Scraps: delimiters and modes](#4-scraps-delimiters-and-modes)
5. [Fragment invocation inside scraps](#5-fragment-invocation-inside-scraps)
6. [Fragment parameters (new and old style)](#6-fragment-parameters)
7. [Per-file output flags](#7-per-file-output-flags)
8. [Minor Commands](#8-minor-commands)
9. [Sectioning commands](#9-sectioning-commands)
10. [Indices](#10-indices)
11. [Running weft (CLI)](#11-running-weft)
12. [Section Markers and Language Detection](#12-section-markers-and-language-detection)
13. [Markdown Output](#13-markdown-output)
14. [Restrictions and caveats](#14-restrictions-and-caveats)
15. [Quick reference of all commands](#15-quick-reference)
16. [Common mistakes and how to avoid them](#16-common-mistakes)

---

## 1. What is weft

weft is a **literate programming** tool descended from nuweb (by Preston Briggs). Its core premise is that the programmer writes **a single document** (a `.weft` file) combining prose documentation with source code in **any programming language**. From that document, weft produces:

- **Tangle** -> source code files ready to compile or execute.
- **Weave** (optional, via `-w`) -> documentation in Markdown or LaTeX with automatically generated indices and cross-references.

Unlike WEB/CWEB (which only work with Pascal or C), weft is **language-agnostic**. You can freely mix C, Perl, Python, Fortran, shell, YAML, SQL, and any other language within a single document.

weft extends the original nuweb with: tangle-only by default, Markdown weave output, section markers for debugging, language auto-detection (250+ extensions), JSON map output, and fragment extraction.

---

## 2. Philosophy and advantages

weft prioritizes simplicity and AI-friendly workflows:

- **Simplicity**: only 4 major commands. Everything else is ordinary prose.
- **Full layout control**: the programmer decides exactly how code appears.
- **Speed**: tangle runs in a single pass.
- **Multiple output files**: a single `.weft` can generate many source files.
- **Smart comparison**: weft only overwrites output files that actually changed, integrating well with `make`.
- **Any language**: C, Perl, Fortran, Scheme, Assembly, PostScript, combinations, etc.
- **Section markers**: automatic traceability from tangled output back to `.weft` source.
- **AI navigation**: JSON maps and fragment extraction for directed codebase exploration.



---

## 3. Major Commands

These are the 4 fundamental commands. Anything that is not a weft command is copied verbatim to the documentation file (when weaving). All weft commands begin with `@`.

### 3.1 `@o` --- Output a file

```
@o file-name flags scrap
```

Creates (or appends to) an output file. The file name is terminated by whitespace. The same file may be specified multiple times --- definitions are concatenated in the order they appear.

Variant `@O`: same behavior, but allows long scraps to break across pages.

### 3.2 `@d` --- Define a fragment

```
@d fragment-name scrap
```

Defines a reusable named fragment. The name is terminated by a carriage return or the beginning of a scrap. Multiple `@d` with the same name **append** in order of appearance.

Variant `@D`: same behavior, but allows page breaks for long scraps.

### 3.3 `@q` --- Define a quoted fragment

```
@q fragment-name scrap
```

Identical to `@d`, but inside a quoted fragment **nested fragments are NOT expanded** during tangling. Instead, they are formatted as fragment uses, so that the output file can itself be weft source for further processing.

You can mix `@d` and `@q` fragments with the same name: `@q` ones are quoted and `@d` ones expand normally, all concatenated in order.

Inside quoted fragments, `@f` (the filename command) is also quoted, so that when it is eventually expanded it refers to the finally produced file, not any intermediate one.

Variants: `@Q` (with page break), `@q+` and `@Q+` (global versions, see section 9).

### 3.4 Abbreviated fragment names

Names can be abbreviated with `...` (three dots):

```
@d Check for terminating at-sequence and return name if found
```

Can be invoked as:

```
@<Check for terminating...@>
```

You only need to type enough characters to make the name **unique**. weft always preserves the longest version of a fragment name. Blanks and tabs within names are insignificant (each string of whitespace is replaced by a single blank).

### 3.5 Fragment names with embedded parameters

A fragment name may contain parameters using the `@'value@'` syntax:

```
@d Validate field @'name@' type @'string@' @{
  if (!data.@1) throw new Error("Missing @1");
  if (typeof data.@1 !== '@2') throw new TypeError("@1 must be @2");
@}

% Invocation:
@<Validate field @'email@' type @'string@'@>
```

When invoked, the parameters are replaced by actual arguments (see section 6).



---

## 4. Scraps: delimiters and modes

A scrap is the code/content block that accompanies an `@o` or `@d`. Any amount of whitespace (including carriage returns) may appear between the name and the beginning of a scrap. There are **three typesetting modes**:

### 4.1 Verbatim mode: `@{` ... `@}`

```
@d My fragment @{
everything here is literal code
  including spaces, tabs, and carriage returns
@}
```

Typeset in verbatim mode in the documentation output. With the `-l` option (LaTeX weave), uses the LaTeX `listings` package.

### 4.2 Paragraph mode: `@[` ... `@]`

```
@d Formatted text @[
This text is typeset as a normal \LaTeX{} paragraph,
allowing \textbf{bold} and formulas $E = mc^2$.
@]
```

Useful for scraps that are sections of TeX documents while still being parts of output files.

### 4.3 Math mode: `@(` ... `@)`

```
@d Formula @(
\sum_{i=1}^{n} x_i^2
@)
```

Content is typeset in LaTeX math mode.

### 4.4 Scraps in running text

Scraps can also appear **outside definitions**, in the documentation body. They are formatted visually as if inside a definition, but do not produce any output code:

```
The return value is stored in @{result_buffer@}, which was
declared in the previous section.
```



---

## 5. Fragment invocation inside scraps

### 5.1 Basic invocation

```
@<fragment-name@>
```

Expands the fragment inline. It is an error to specify recursive fragment invocations.

### 5.2 With arguments (old style)

```
@<fragment-name@(arg1@,arg2@)@>
```

Up to 9 arguments. Inside the fragment they are accessed as `@1`, `@2`, ..., `@9`.

### 5.3 With embedded arguments (new style)

Arguments are embedded directly in the fragment name. There are 4 types:

| Syntax | Meaning |
|--------|---------|
| `@'literal text@'` | Literal string (only `@@` -> `@` is interpreted) |
| `@<another-fragment@>` | A fragment expansion passed as argument |
| `@{expanded text@}` | Expanded text (like an anonymous fragment with the same arguments as the caller) |
| `@1`, `@2`, etc. | Forward the calling fragment's argument |

**Important note:** If both embedded and old-style arguments are mixed in a single invocation, the old-style arguments are ignored.

If a fragment is invoked in abbreviated form (with `...`) and parameters are omitted, the default value from the fragment definition is used.

---

## 6. Fragment parameters

### Inside a scrap

| Command | Function |
|---------|----------|
| `@1` to `@9` | Substituted by the Nth argument. If not passed, the empty string is substituted. |
| `@f` | Replaced by the name of the current output file. |
| `@t` | Replaced by the title of the fragment as it appears at the point of use, with all parameters already replaced by actual arguments. |
| `@#` | At the beginning of a line, suppresses the normal indentation for that line. Useful for preprocessor directives inside nested scraps: `@#@#ifdef DEBUG`. |
| `@s` | Suppresses indentation for the following fragment expansion. |
| `@%` | Comment inside scrap (behaves exactly like `%` in normal LaTeX text). |

### Labels inside scraps

```
@xlabel@x
```

Associates a label (any text not containing `@`) to a point inside the scrap. Expands to the scrap's reference number followed by a numeric value. Outside scraps it expands to the same value. This allows documentation text to refer to specific places within code.



---

## 7. Per-file output flags

When defining an output file with `@o`, flags can be placed between the file name and the scrap:

```
@o my_file.c -d -cx @{
  ...
@}
```

| Flag | Effect |
|------|--------|
| `-d` | Generates `#line` directives in the output file. Compiler error messages refer to the `.weft` file rather than the generated file. Useful for C, C++, and sometimes Fortran. |
| `-i` | Suppresses indentation of expanded fragments. When a fragment is expanded within a scrap, it will NOT be indented to match the invocation's indentation. Useful for Fortran. |
| `-t` | Suppresses tab expansion in the output file. Essential for Makefiles. |
| `-cx` | Inserts comments in the generated code documenting which fragment produced it. `x` selects the comment style: `c` for C (`/* */`), `+` for C++ (`//`), `p` for Perl/shell (`#`). With the global `-x` flag, comments include page references. |
| `-s` | Suppresses section markers and `#line` directives in the tangled output for this file. The language is still detected (from the extension or a preceding `@l`), so the Markdown weaver can emit syntax-highlighting classes, but the tangled code is emitted without any annotations. Useful for bootstrap source files or generated output that should remain clean. |

These flags are still available for explicit control, but in most cases the automatic language detection described in section 12 is more convenient.

These flags introduce minimal language dependencies and apply only to the specific file, allowing languages to be freely mixed within a single document.

---

## 8. Minor Commands

These commands may appear anywhere in the `.weft` file.

| Command | Function |
|---------|----------|
| `@@` | Produces a single literal `@` character in the output. **Critical** for languages that use `@` (Perl: `@@array`, emails: `user@@domain.com`). |
| `@_text@_` | Makes `text` bold in the documentation output (for keywords, etc.). |
| `@i file-name` | Includes a file. Includes may be nested up to 10 levels deep. The file name should be complete (no extension appended) and terminated by a carriage return. The current directory is searched by default; additional directories can be added with the `-I` command-line flag. |
| `@rx` | Changes the escape character from `@` to `x`. Must appear before any scrap definitions. |
| `@v` | Replaced by the string established by the `-V` command-line flag, or a default string if the flag isn't given. Intended for marking versions of generated files. |
| `@c` | Outside a scrap: defines a block comment using all text until the next command. Inside a scrap: expands the last block comment using the current indentation, breaking lines between words if 60 characters are exceeded. |
| `@l lang` | Sets the language for the next `@o` or `@d` directive. Placed on the line **before** the directive it applies to (outside the scrap). The tag is looked up in the language table (section 12) to determine the comment style for section markers and the `class` attribute for Markdown syntax highlighting. |
| `@L name style [+d]` | Defines a custom language mapping for section markers in the document preamble. `name` is the tag for `@l` and file extensions, `style` is the comment syntax (`//`, `#`, `--`, `/*`, or `<!--`), `+d` (optional) enables `#line` directive generation. User-defined entries are searched before the built-in table. Up to 32 custom languages. Example: `@L wgsl //`, `@L cedar //`, `@L c // +d`. |
| `@W format` | Set the weave output format for this document. `format` is `md` (Markdown) or `tex` (LaTeX). Equivalent to `-w format` on the command line. |

### Scraps in text (outside definitions)

```
The buffer is declared as @{char buffer[256]@} in the previous section.
```

Typeset as if inside a scrap, but does not generate any output code.

### Fragment expansion in text

```
@<Fragment name@>
```

Outside a scrap, expands the fragment verbatim in the document text. The expansion is NOT interpreted for typesetting, so any special environment must be set up before and after.



---

## 9. Sectioning commands

For larger documents, indices and usage lists become unwieldy. The sectioning system keeps **fragment names and user identifiers separate** between sections.

### 9.1 Local sections vs. base section

- **Base section**: where everything goes by default.
- **Local sections**: created with `@s`, spanning from the `@s` to the next `@s` or `@S`.

Fragments defined in one local section are NOT visible in another. Fragments in the base section are accessible within the base section regardless of any local sections in between. Except for the fact that scraps in successive sections can write to the same output file, this is the same as if the sections came from separate input files.

| Command | Function |
|---------|----------|
| `@s` | Start a new local section. |
| `@S` | Close the current local section and return to the base section (without starting another). |
| `@d+ name scrap` | Define a **global** fragment, accessible from all sections. |
| `@D+` | Same as `@d+` but allows page breaks. |
| `@q+` | Global quoted fragment. |
| `@Q+` | Global quoted fragment with page break. |

### 9.2 Inside scraps (cross-section)

| Command | Function |
|---------|----------|
| `@<+fragment-name@>` | Expand the globally accessible fragment with that name, ignoring any local fragment with the same name. |
| `@+` | Like `@|` but exports the identifiers to the global realm. They are not directly referenced in any scrap in any section (not even the one where they are defined). |
| `@-` | Like `@|` but imports identifiers from the global realm to the local realm. Cross-references show where the global variables are defined and the same names become locally accessible. Uses of the names within the section point to this scrap. |

**Note:** The `+` signs above are part of the commands, NOT part of the fragment names. If you want a fragment whose name begins with a plus sign, leave a space between the command and the name.

---

## 10. Indices

weft generates three types of indices. They are typically placed at the end of the document:

```latex
\appendix
\section{Indices}

@f    % Index of file names
@m    % Index of fragment names
@u    % Index of user-specified identifiers
```

### 10.1 User-specified identifiers (`@u`)

Identifiers must be explicitly marked at the end of a scrap with `@|`:

```
@d A scrap @{
int FOO = 42;
double BAR = 3.14;
@| FOO BAR @}
```

Identifiers can be any string of characters not including whitespace or `@` characters. Therefore it's possible to add index entries for things like `<<=` if desired. An identifier may be declared in more than one scrap.

In the generated index, each identifier appears with a list of all scraps using and defining it, where the defining scraps are distinguished by underlining. Note that the identifier doesn't actually have to appear in the defining scrap; it just needs to be in the list of definitions at the end of the scrap.

### 10.2 Global indices

| Command | Function |
|---------|----------|
| `@m+` | Index of global fragments (defined with `@d+`). |
| `@u+` | Index of globally accessible user identifiers. |



---

## 11. Running weft

### Syntax

```bash
weft [flags] file-name...
```

One or more files may be processed at a time. If a file name has no extension, `.weft` will be appended.

**Default behavior**: weft **tangles only** (no documentation output). This differs from the original nuweb, which produced both tangle and weave by default. Use `-w` to activate weave output.

Example:

```bash
weft project.weft             # tangle only (default)
weft -w project.weft          # tangle + weave per @W directive
weft -w md project.weft       # tangle + weave to Markdown (override)
weft -w tex project.weft      # tangle + weave to LaTeX (override)
```

### Main control flags

| Flag | Effect |
|------|--------|
| `-w [format]` | Activate weave (documentation generation). Without `-w`, only tangle runs. The optional `format` selects the output: `-w md` forces Markdown, `-w tex` forces LaTeX. `-w` alone reads the format from `@W` in the source; if `@W` is absent, weft reports an error. A CLI format overrides any `@W` directive. |
| `-t` | Suppress weaving even if `-w` is given. Useful to override `-w` in scripts. |
| `-o` | Suppress generation of output files (weave only). Requires `-w`. |
| `-c` | Avoid testing output files for change before updating them (always overwrite). |
| `-m` | Emit a JSON map of the document structure to stdout. Neither weaving nor tangling is performed. |
| `-e name` | Extract the named fragment and its transitive dependencies as Markdown to stdout. Uses prefix matching (same as `@<name...@>`). Neither weaving nor tangling is performed. |
| `-R file[:line]` | Reverse map: read section markers from a tangled output file and emit JSON mapping to stdout. If a line number is given, emit the innermost scrap covering that line. Without a line number, emit all scrap regions. No `.weft` source files are needed. |

### Additional flags

| Flag | Effect |
|------|--------|
| `-v` | Verbose: writes progress information to stderr. |
| `-n` | Number scraps sequentially from 1 (instead of using page numbers). More desirable for small webs. |
| `-s` | Don't print the list of scraps making up each file following each scrap. |
| `-d` | Print "dangling" identifiers --- user identifiers which are never referenced. |
| `-p path` | Prepend `path` to the filenames for all output files. |
| `-x` | Include cross-reference numbers in scrap comments. |
| `-h options` | Provide options for the `hyperref` package (LaTeX weave only). |
| `-r` | Turn on hyperlinks (LaTeX weave only). You must include `\usepackage{hyperref}` in the document text. |
| `-l` | Use the `listings` package for formatting scraps (LaTeX weave only). |
| `-V string` | Provide `string` as the replacement for the `@v` operation. |
| `-I directory` | Add a directory to the search path for `@i` includes. Multiple `-I` flags can be used; directories are searched in the order given. |

### Typical build workflow

```bash
weft project.weft                # tangle
# ... build and test with language tools ...

weft -w md project.weft          # optionally generate Markdown docs
```

For LaTeX documentation (when needed):

```bash
weft -w tex project.weft         # weave to LaTeX
pdflatex project.tex             # first pass
weft -w tex project.weft         # update page references
pdflatex project.tex             # final pass with TOC and cross-refs
```

---

## 12. Section Markers and Language Detection

weft automatically generates **section markers** in tangled output to trace
every scrap back to its source `.weft` file and line number.

### Marker Format

```
COMMENT_START {SCRAP_NUMBER: SOURCE_FILE:LINE} COMMENT_END
...code from this scrap...
COMMENT_START {:SCRAP_NUMBER} COMMENT_END
```

Each marker encodes:

| Part | Meaning |
|------|---------|
| `{` or `{:` | Opening or closing delimiter |
| Number | Scrap ID — unique across the entire document |
| `file:line` | Source `.weft` file and line number (opening only) |

### How It Works

The key idea:

> **file extension → language → comment style → section markers in tangled output**

For example, `@o server.js` has extension `.js`. weft looks up `js` in its language table, finds that JavaScript uses `//` comments, and wraps each scrap with `//` markers. This happens automatically — no flags are needed.

When fragments are nested, each expansion gets its own pair of markers, producing a clear hierarchy in the output:

```js
// {3: routes.weft:42}
app.get("/api/users/:id", async (req, res) => {
  // {7: database.weft:18}
  const users = await db.query("SELECT * FROM users");
  // {:7}
  res.json(users);
});
// {:3}
```

Scrap numbers are document-global — scrap 1 is the first `@{...@}` in the entire woven document, scrap 5 is the fifth. They are stable as long as you do not reorder scraps.

### Language Auto-Detection

weft includes a built-in table of over 250 entries organized by comment style. Each entry maps a tag to a comment style and optionally enables `#line` directives. The tag is matched against `@l` tags and file extensions alike.

| Comment Style | Tags (representative) |
|---------------|-----------------------|
| `/* */` + `#line` | c, h, cpp, cc, cxx, hpp, hxx, objc, objcpp, m, mm, ino, pde, cu, cuh, cilk |
| `/* */` | css, scss, sass, less, stylus, cl (OpenCL) |
| `//` | js, ts, jsx, tsx, mjs, cjs, mts, svelte, vue, java, scala, kotlin, kt, groovy, gradle, clj, cs, fs, fsx, vb, go, rust, rs, zig, swift, dart, odin, mojo, carbon, php, graphql, prisma, glsl, hlsl, wgsl, metal, vert, frag, proto, thrift, fbs, capnp, sol, reason, res, ml, sml, fut, apex, bicep, jsonnet, qml, p4 |
| `#` | python, py, pyx, pyi, sh, bash, zsh, fish, ksh, nu, perl, pl, ruby, rb, r, tcl, awk, sed, coffee, makefile, mk, cmake, just, meson, bazel, bzl, julia, jl, nim, crystal, cr, elixir, ex, yaml, yml, toml, dockerfile, terraform, tf, hcl, nix, conf, ini, env, editorconfig, gitignore, powershell, ps1, xdc, sdc |
| `--` | lua, luau, sql, psql, plsql, plpgsql, mysql, sqlite, hive, haskell, hs, elm, purescript, purs, ada, adb, ads, vhdl, vhd, lean, lean4, agda, idris, idr, erlang |
| `<!-- -->` | html, htm, xml, svg, xhtml, xsl, xsd, dtd, rss, xaml, csproj, jsp, aspx, cshtml, erb, ejs, hbs, njk, twig, blade, pug, liquid, mustache, md, mdx, rst, tex, latex |
| (none) | json, csv, tsv, txt, diff, patch |

If a tag is not found in either the user-defined (`@L`) or built-in table, weft defaults to `//` comments with no `#line` directives. Tags mapped to no style (e.g. `json`, `txt`) produce no markers at all.

### `@l` — Override Language

The `@l` command is placed on the line **before** an `@o` or `@d` directive to override the file extension:

```
@l sql
@o queries.txt @{
SELECT * FROM users;
@}
```

Here the file is `queries.txt`, whose extension `.txt` has no comment style. But `@l sql` tells weft to use `--` comments:

```
-- {1: db.weft:5}
SELECT * FROM users;
-- {:1}
```

`@l` also works on `@d` (fragment) definitions. This is useful for fragments that will be included in files of a specific language:

```
@l python
@d Validate email @{
import re
def validate(email):
    return bool(re.match(r'^[^@@]+@@[^@@]+$', email))
@}
```

The language tag is stored on the fragment and used both for section markers in the tangled output and for syntax highlighting in the Markdown weave output.

**Priority rules.** When multiple sources of language information are available:
1. **Explicit per-file flags** (`-d`, `-s`, `-cx`, etc.) on the `@o` line always win for tangle behavior.
2. **`@l` tag** immediately before the directive — overrides the extension.
3. **File extension** of the `@o` name — the default when no `@l` is present.

### `@L` — Define New Languages

The built-in table covers 250+ entries, but it cannot anticipate every language. The `@L` command defines new language mappings — or overrides existing ones — in the document preamble.

```
@L name style [+d]
```

- `name` is the tag recognized in `@l` commands and file extensions.
- `style` is the comment syntax: `//`, `#`, `--`, `/*`, or `<!--`.
- `+d` (optional) enables `#line` directive generation.

Examples:

```
@L wgsl //          % WebGPU shading language: // comments
@L cedar //         % Cedar policy language
@L c // +d          % override built-in: use // instead of /* */
```

User-defined entries are searched **before** the built-in table, so `@L c //` effectively changes the comment style for all subsequent `@o main.c` directives from `/* */` to `//`. Up to 32 user-defined languages may be registered.

`@L` should appear in the document preamble, before any `@o` or `@d` that depends on it.

### C/C++ `#line` Directives

For C-family languages, weft also generates `#line` directives **above** the opening marker, so the compiler reports errors against the `.weft` source directly:

```c
#line 12 "memory.weft"
/* {2: memory.weft:12} */
void *safe_malloc(size_t size) { ... }
/* {:2} */
```

The `#line` directive and the section marker are complementary: `#line` makes the compiler report errors against the `.weft` file directly; the `{N: file:line}` markers are human-readable when browsing the tangled output.

### Syntax Highlighting in Markdown Output

When weaving to Markdown (`-w md`), weft emits the language tag on the `<code>` element:

```html
<pre><code class="language-python">def hello():
    print("hello")
</code></pre>
```

This allows client-side highlighters (highlight.js, Prism) and platforms like GitHub to apply syntax colouring automatically. The language tag comes from the same chain: `@l` tag or file extension.

For `@d` fragments, the tag comes from a preceding `@l` command (there is no file extension to infer from). If no language is known, weft emits a plain `<pre><code>` without a class.

---

## 13. Markdown Output

When invoked with `-w md`, weft generates a Markdown file (`.md`) instead of a LaTeX file (`.tex`). This allows literate programs to be read on any platform that renders Markdown, including GitHub, GitLab, and local editors like VS Code.

### Activating Markdown Weave

Two methods:

1. **Command line**: `weft -w md file.weft` (overrides `@W`)
2. **In-source directive**: place `@W md` in the `.weft` file preamble, then use `weft -w file.weft`

Tangling works identically regardless of the weave format. When `-w` is given, a single invocation produces both the documentation and the tangled output files in one pass.

### Output Format Details

The Markdown output preserves all the essential features of the LaTeX output — scrap definitions, cross-references, and indices — but uses different formatting:

| Feature | Format |
|---------|--------|
| **Scrap headers (files)** | `**"filename"** **N** =` where N is the sequential scrap number |
| **Scrap headers (fragments)** | `**<*Name* N>** =` |
| **Scrap bodies** | `<pre><code>` HTML blocks (not fenced code blocks), allowing fragment references within scraps to be rendered as clickable HTML links with italic/bold formatting |
| **HTML escaping** | `<`, `>`, and `&` inside scraps are escaped as `&lt;`, `&gt;`, and `&amp;` |
| **Cross-reference anchors** | `<a id="weftN">` on each scrap |
| **Cross-reference links** | `<a href="#weftN">` for clickable navigation between definitions and references |
| **Footer notes** | "defined by", "referenced in" appear as Markdown blockquotes |
| **Sequential numbering** | Forced automatically (Markdown has no page numbers); scraps are numbered 1, 2, 3... without letter suffixes |
| **Syntax highlighting** | `<code class="language-X">` when the language is known (from `@l` tag or file extension), allowing client-side highlighters (highlight.js, Prism) and platforms like GitHub to apply syntax colouring |
| **Indices** | `@f`, `@m`, and `@u` produce Markdown lists with links; in the user identifier index, defining scraps appear in bold |
| **Prose** | Text outside scraps passes through unmodified — the author writes Markdown prose directly in the `.weft` source |

### Cosmetic Fence Stripping

When writing `.weft` files with Markdown prose, it is often convenient to surround scrap definitions with Markdown fenced code blocks (triple backticks) so that the source file renders nicely in Markdown previewers such as GitHub or VS Code:

```
Some prose in Markdown.

` `` `c
@d My fragment @{
code here
@}
` `` `

More prose.
```

weft silently strips these fence lines (lines that consist of three or more backticks, optionally followed by a language identifier) during Markdown weaving. They do not appear in the generated `.md` file and do not affect tangling. This is purely a cosmetic convenience for making the source file readable in any Markdown viewer.

Note that fences are only stripped in Markdown mode (`-w md`). In LaTeX mode, backtick lines are treated as ordinary text. Fences no longer set the language — use `@l` for that.

---

## 14. Restrictions and caveats

- File names and index entries **must not contain any `@` signs**.
- Fragment names may be almost any well-formed TeX string. You can change fonts or use math mode, but care should be taken to ensure matching braces, brackets, and dollar signs.
- Very long scraps (horizontally or vertically) may not typeset well.
- Temporary files are placed in the current directory. All output files should be on the same filesystem as the current directory (or use the `-p` flag).
- After running LaTeX, weft must be re-run to obtain correct page numbers in cross-references (LaTeX weave only). This restriction does not apply to Markdown output (`-w md`), which uses sequential numbering and requires no additional processing passes.
- References to undefined fragments generate a warning; the fragment name appears surrounded by `<>` in the output file. This is convenient during development.
- Error handling is not completely uniform: some errors produce warnings; others halt immediately.
- When a fragment is expanded in a scrap, the body of the fragment is indented to match the indentation of the fragment invocation. Tabs are expanded to spaces (every 8 characters) by default. These defaults can be overridden with the `-i` and `-t` per-file flags.



---

## 15. Quick reference

### Major commands

| Command | Description |
|---------|-------------|
| `@o file flags @{...@}` | Define output file |
| `@O file flags @{...@}` | Same, allows page break |
| `@d Name @{...@}` | Define named fragment |
| `@D Name @{...@}` | Same, allows page break |
| `@q Name @{...@}` | Define quoted fragment (internals not expanded) |
| `@Q Name @{...@}` | Same, allows page break |

### Scrap delimiters

| Start/End | Mode |
|-----------|------|
| `@{` ... `@}` | Verbatim |
| `@[` ... `@]` | Paragraph (LaTeX) |
| `@(` ... `@)` | Math |

### Inside scraps

| Command | Function |
|---------|----------|
| `@<Name@>` | Invoke fragment |
| `@<Name@(a1@,a2@)@>` | Invoke with arguments (old style) |
| `@'literal@'` | Embedded literal argument |
| `@1` ... `@9` | Reference to Nth argument |
| `@f` | Current output file name |
| `@t` | Current fragment title (with arguments substituted) |
| `@#` | Suppress indentation for this line |
| `@s` | Suppress indentation for next fragment expansion |
| `@%` | Comment (like % in LaTeX) |
| `@xlabel@x` | Label / numeric reference |
| `@| ID1 ID2 @}` | Declare identifiers for index |
| `@+` | Export identifiers to global scope |
| `@-` | Import identifiers from global scope |
| `@<+Name@>` | Invoke global fragment explicitly |

### Text commands (outside scraps)

| Command | Function |
|---------|----------|
| `@@` | Literal `@` |
| `@_text@_` | Bold text (for keywords) |
| `@i file` | Include file |
| `@rx` | Change escape character to `x` |
| `@v` | Insert version string |
| `@c` | Block comment |
| `@l lang` | Set language for next `@o`/`@d` directive (before it, not inside scrap) |
| `@L name style [+d]` | Define new language for section markers (`+d` enables `#line`) |
| `@W format` | Set weave format (`md` or `tex`) |
| `@{text@}` | Text in scrap format (inline) |
| `@<Name@>` | Verbatim fragment expansion in text |

### Sectioning

| Command | Function |
|---------|----------|
| `@s` | Start new local section |
| `@S` | Close local section (return to base) |
| `@d+` / `@D+` / `@q+` / `@Q+` | Global fragments |

### Indices

| Command | Function |
|---------|----------|
| `@f` | File name index |
| `@m` | Fragment name index |
| `@u` | User identifier index |
| `@m+` | Global fragment index |
| `@u+` | Global identifier index |

### Per-file output flags (`@o file -flags`)

| Flag | Effect |
|------|--------|
| `-d` | Generate `#line` directives (C/C++/Fortran) |
| `-i` | Suppress fragment indentation (Fortran) |
| `-t` | Suppress tab expansion (Makefiles) |
| `-cx` | Comments: `c`=C, `+`=C++, `p`=Perl/shell |
| `-s` | Suppress section markers for this file |

### CLI flags

| Flag | Effect |
|------|--------|
| `-w [format]` | Activate weave (`md`, `tex`, or from `@W`) |
| `-t` | Suppress weaving (override `-w` in scripts) |
| `-m` | JSON map to stdout (no tangle/weave) |
| `-e name` | Extract fragment as Markdown (no tangle/weave) |
| `-R file[:line]` | Reverse map: JSON tracing tangled output → `.weft` source |
| `-o` | Weave only (no tangle) |
| `-c` | Always overwrite (no compare) |
| `-v` | Verbose output |
| `-n` | Sequential scrap numbering |
| `-s` | Suppress scrap lists |
| `-d` | Print dangling identifiers |
| `-p path` | Prepend path to output files |
| `-x` | Cross-reference numbers in comments |
| `-I dir` | Add include search path |
| `-V string` | Version string for `@v` |
| `-h opts` | Hyperref options (LaTeX) |
| `-r` | Enable hyperlinks (LaTeX) |
| `-l` | Use listings package (LaTeX) |



---

## 16. Common mistakes

### 16.1 Unescaped `@`

The #1 error. Every literal `@` must be written as `@@`:

| What you want | What you write in weft |
|---------------|------------------------|
| `@array` (Perl) | `@@array` |
| `@_` (Perl) | `@@_` |
| `@{$ref}` (Perl) | `@@{$ref}` |
| `user@domain.com` | `user@@domain.com` |
| `@app.route` (Python/Flask) | `@@app.route` |

### 16.2 `@{` and `@}` as delimiters

`@{` opens a scrap and `@}` closes it. In Perl, `@{$hashref}` **must** be written `@@{$hashref}` to prevent weft from interpreting `@{` as a scrap start.

### 16.3 `@i` include paths

`@i payments.weft` searches in the current directory (or those specified with `-I`), **not** relative to the file performing the include.

### 16.4 Global namespace of fragments

All fragments from all included files share a single global namespace (unless you use sections with `@s`). Use descriptive names and prefixes to avoid collisions (e.g., `@d Payments: validate input` not just `@d validate input`).

### 16.5 Inclusion order matters

`@d` fragments with the same name are concatenated in **order of appearance in the master document**. Changing the order of `@i` includes changes the order of generated code.

### 16.6 No `\documentclass` in included files

Only the master file has the LaTeX preamble (when using LaTeX weave). Included files start directly with content.

### 16.7 Indentation behavior

When a fragment is expanded inside a scrap, the body of the fragment is indented to match the indentation of the fragment invocation. This is important for languages sensitive to indentation (e.g., Python, Fortran, Makefiles). Use the `-i` per-file flag to suppress this, or `@#` at the beginning of a line to suppress indentation for that line, or `@s` before a fragment invocation to suppress indentation for that expansion.

### 16.8 Use section markers for debugging

When a compilation error points to a line in tangled output (e.g., `server.js:47`), use `weft -R server.js:47` to instantly resolve the `.weft` source location. For manual inspection, open the tangled file and find the nearest `{N: file.weft:line}` marker above the error. See [debugging-tangled-code.md](debugging-tangled-code.md) for the complete debugging workflow.



---

## Appendix A: Minimal complete example

```
## My First Literate Program

This document generates a small Python script that greets
the user and computes the factorial of a number.

### The Main Program

Our script is composed of two parts: the factorial function
and the main execution block.

@o hello.py @{
#!/usr/bin/env python3
"""Script generated with weft."""

@<Factorial function@>

@<Main program@>
@}

### The Factorial Function

We use simple recursion to compute the factorial:

@d Factorial function @{
def factorial(n):
    """Compute n! recursively."""
    if n <= 1:
        return 1
    return n * factorial(n - 1)
@| factorial @}

### Main Program

The program asks the user for a name and a number:

@d Main program @{
if __name__ == "__main__":
    name = input("What is your name? ")
    print(f"Hello, {name}!")
    n = int(input("Give me a number: "))
    print(f"{n}! = {factorial(n)}")
@}
```

To build:

```bash
weft example.weft            # tangle: generates hello.py
python3 hello.py             # run the program
weft -w md example.weft      # optionally generate docs
```

---

## Appendix B: Multiple files with accumulated fragments

```
## Routes

Each module contributes its routes to the main router:

@o app.py @{
from flask import Flask
app = Flask(__name__)

@<Register routes@>

if __name__ == "__main__":
    app.run(debug=True)
@}

## Users Module

@d Register routes @{
@@app.route('/users')
def list_users():
    return "User list"
@}

## Products Module

@d Register routes @{
@@app.route('/products')
def list_products():
    return "Product list"
@}
```

Note: `@@app` is needed because `@app` would be an invalid weft command.

The generated `app.py` will contain both routes concatenated at the point where `@<Register routes@>` is invoked.

---

## Appendix C: Using the listings package (LaTeX weave)

For partial syntax highlighting in LaTeX documentation:

```latex
\usepackage{listings}
\lstset{
  language=Python,
  extendedchars=true,
  keepspaces=true,
  basicstyle=\ttfamily\small,
  keywordstyle=\bfseries\color{blue},
  commentstyle=\color{gray}
}
```

Run with:

```bash
weft -w tex -l master.weft
```

---

## Appendix D: Perl escaping reference

Since Perl uses `@` heavily, here is a complete escaping table:

| Perl code desired | Write in weft |
|-------------------|---------------|
| `@_` | `@@_` |
| `@ARGV` | `@@ARGV` |
| `my @items = (1,2)` | `my @@items = (1,2)` |
| `$self->render(json => \@data)` | `$self->render(json => \@@data)` |
| `push @{$arr}, $v` | `push @@{$arr}, $v` |
| `for my $x (@list)` | `for my $x (@@list)` |
| `my ($self, @args) = @_` | `my ($self, @@args) = @@_` |
| `"user\@host.com"` | `"user\@@host.com"` |
