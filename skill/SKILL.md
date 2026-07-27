---
name: literate-programming
description: >
  Literate programming paradigm with weft for any project.
  Each .weft file is a self-contained narrative crossing all architectural
  layers. Pipeline: .weft → weft tangle → output files.
  Triggers: "literate programming", "programacion literaria", "weft",
  "nuweb", ".weft files", "weft skill", "literate skill".
---


# Literate Programming with weft

A paradigm where **documentation is the source code**. You write `.weft` files
as narratives with embedded code chunks. weft tangles them into executable
output and optionally weaves them into documentation.

This skill covers the generic paradigm (any language). It was itself
built with weft.



## Philosophy

Literate programming inverts code and documentation: you write a **narrative
first**, with code woven in. The `.weft` file is the single source of truth.

The goal is to produce **literary and artistic works of programming** —
programs that achieve excellence in exposition, software quality, and
debuggability. A literate program is not code with comments; it is a
carefully crafted essay where code and prose illuminate each other.
The standard is publishable quality: a reader should be able to understand
the system by reading the document from beginning to end, as one reads
a well-written book.

Core principles:

- **Paradigm, not documentation** — LP is a programming paradigm where
  named chunks form a human-readable meta-language of composable abstractions
- **Web, not tree** — a program is neither top-down nor bottom-up; LP
  lets you present parts in "stream of consciousness" order, whatever
  best serves human comprehension
- **Appropriate size** — each chunk gets its natural size without
  distorting neighboring parts; error handling, edge cases, and minor
  logic all receive the attention they deserve
- **Variation theory** — every concept has prose + code representations
  that reinforce each other
- **Visual complexity reduction** — chunks replace inline detail with
  meaningful names, making structure visible at every level
- **Single source of truth** — one `.weft` file generates both documentation
  and executable output
- **Expository mode reduces bugs** — explaining code forces clearer
  thinking; the extra time writing prose is recovered in less debugging

> "Instead of writing code containing documentation, the literate programmer
> writes documentation containing code. It is the difference between
> performing and exposing a magic trick." — Ross Williams

> "Let us change our traditional attitude to the construction of programs:
> instead of imagining that our main task is to instruct a computer what to
> do, let us concentrate rather on explaining to human beings what we want
> a computer to do." — Donald Knuth

See [references/literate-philosophy.md](references/literate-philosophy.md)
for the complete philosophy guide including Childs' 7 requirements.


## Workflow

When the user asks to create or work on a literate project:

**1. Identify concepts** — ask what the project does, not what files it
needs. Break it into business concepts (auth, payments, notifications).

**2. Create structure:**
```
project.weft                     # Master: @i includes + LaTeX
literate/
├── concepts/
│   ├── concept-a.weft           # @d chunks only, narrative + code
│   ├── concept-b.weft
│   └── ...
└── assembly.weft              # ALL @o directives
justfile                         # tangle + format + build
```

**3. Write each concept** as a narrative that crosses all layers.
Prose first, code second. One `.weft` per business concept.

**4. Assemble** — `assembly.weft` routes chunks to output files. This is
the only file with `@o` directives. Concepts never know their output paths.

**5. Build** — `just all` tangles, formats, and builds.

**6. Iterate** — all changes go through `.weft` files. Never edit tangled output.

**Definition of done**: a change is **not complete** until its surrounding
prose is updated. Code and narrative must always be in sync. If you modify
a chunk, update the prose that explains it. If you add a chunk, write the
prose that introduces it. A code change without updated prose is not finished.

### Common `@` Escaping by Language

Inside `.weft` chunks, every literal `@` in the desired output must be `@@`:

| Context | In `.weft` file | Output |
|---------|-----------------|--------|
| GraphQL Federation | `@@key(fields: "id")` | `@key(fields: "id")` |
| Dart/Flutter | `@@override` | `@override` |
| Python decorators | `@@property` | `@property` |
| Java annotations | `@@Override` | `@Override` |
| Perl arrays | `@@array` | `@array` |
| Email addresses | `user@@host.com` | `user@host.com` |

**Meta-escaping** (when your output contains weft syntax as content,
e.g. a skill teaching weft): `@@@@` → `@@` in output → `@` when
that output is itself processed. This is rare but occurs when building
skills about literate programming.


## weft Syntax Reference

For the complete command reference (core commands, scrap delimiters, flags,
CLI options), see [references/weft-syntax.md](references/weft-syntax.md).

Key points: `@o` defines output files, `@d` defines reusable fragments,
`@<Name@>` invokes them, `@@` escapes literal `@`. weft tangles by default;
use `-w md` or `-w tex` to activate weave (or declare `@W md`/`@W tex` in
the source). Use `-I dir` to add include search paths for `@i`.

Additional commands: `@O`/`@D`/`@Q` are page-break variants of `@o`/`@d`/`@q`.
`@c` defines block comments. `@%` comments out lines inside scraps. `@r x`
changes the escape character from `@` to `x`.


## Section Markers and Debugging

weft automatically wraps every scrap in the tangled output with **section
markers** — comment annotations that trace each line back to its `.weft`
source file and line number.

### How It Works

weft detects the output language from the file extension (250+ extensions
supported) and generates markers in the appropriate comment syntax:

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

For C/C++, weft also generates `#line` directives so the compiler
reports errors against the `.weft` source directly.

### Debugging Workflow

**Manual**: open the tangled file, scan upward for the nearest opening
marker, go to the `.weft` source.

**Programmatic** (recommended): use `-R` for instant reverse mapping:

```bash
weft -R server.js:47
# → {"scrap": 3, "file": "routes.weft", "line": 42}

weft -R server.js          # full file mapping (all regions)
```

No `.weft` files are needed — `-R` parses the section markers already
embedded in the tangled output. This enables CI/CD error translation
and AI agent workflows.

### Mid-Line Inline Expansion

When a fragment reference `@<name@>` appears **mid-line** (after at least
one visible character on the same line), section markers, `#line` directives,
and fragment-name comments are automatically suppressed. The fragment content
is spliced seamlessly into the surrounding text:

```
@d port @{4000@}
@d host @{"localhost"@}

@l c
@o config.c -s
@{const char *host = @<host@>;
int port = @<port@>;
@}
```

Produces:

```c
const char *host = "localhost";
int port = 4000;
```

At start-of-line positions (even indented), markers are emitted normally.
No special flags needed — the position on the line determines the behaviour.

### Language Override

- `@l lang` placed before an `@o`/`@d` directive overrides auto-detection
- `@L name style [+d]` defines new languages in the preamble

See [references/debugging-tangled-code.md](references/debugging-tangled-code.md)
for the complete debugging guide with examples by language.


## AI-Assisted Navigation

weft provides a full toolkit for AI-assisted workflows: **JSON maps**
(`-m`), **fragment extraction** (`-e`), **reverse maps** (`-R`), and
six analysis flags (`--bodies`, `--prose`, `--diff`, `--callers`,
`--lint`, `--errors`). Together they enable precise, directed navigation
of literate codebases in both directions — from `.weft` source to
tangled output and back.

### JSON Map (`-m`)

```bash
weft -m project.weft
```

Emits a JSON graph to stdout containing:
- All output files and which fragments compose them
- All fragment definitions with source locations
- All fragment uses (cross-references)
- Transitive dependency information

Use with `jq` to answer structural questions:

```bash
# Which fragments contribute to server.js?
weft -m project.weft | jq '.files["server.js"]'

# Where is "Validate input" defined?
weft -m project.weft | jq '.fragments["Validate input"]'
```

### Fragment Extraction (`-e`)

```bash
weft -e "Validate input" project.weft
```

Extracts a single fragment and all its transitive dependencies as a
self-contained Markdown document to stdout. The output includes:
- The fragment's code with syntax highlighting
- All fragments it references (recursively)
- Source file and line information

### Reverse Map (`-R`)

```bash
weft -R server.js:47
```

Given a tangled output file and a line number, returns the innermost `.weft`
source location that generated that line:

```json
{"scrap": 3, "file": "routes.weft", "line": 42}
```

Without a line number, returns all scrap regions in the file:

```bash
weft -R server.js
```

```json
{"source": "server.js", "regions": [
  {"start": 2, "end": 15, "scrap": 3, "file": "routes.weft", "line": 42},
  {"start": 5, "end": 8, "scrap": 7, "file": "database.weft", "line": 18}
]}
```

Key property: operates on tangled output only — no `.weft` files needed,
no pass1. Parses the section markers already embedded in the file.

### Enriched JSON Map (`-m --bodies --prose`)

The `-m` JSON map can be enriched with two optional flags:

```bash
weft -m --bodies project.weft          # include scrap source code
weft -m --prose project.weft           # include documentation paragraphs
weft -m --bodies --prose project.weft  # both
```

Each `defs` entry always includes `"end_line"` (the full line span).
With `--bodies`, it adds `"body"` (JSON-escaped raw scrap text).
With `--prose`, it adds `"prose"` (the documentation paragraph preceding
the `@d`/`@o` directive).

Use with `jq` for deep inspection:

```bash
# Get the code body of a specific fragment
weft -m --bodies project.weft | jq '.fragments["Validate input"].defs[0].body'

# Get the prose context explaining a fragment
weft -m --prose project.weft | jq '.fragments["Validate input"].defs[0].prose'
```

### Dry-Run Diff (`--diff`)

```bash
weft --diff project.weft
```

Tangles all output files to temp files and shows a unified diff of what
would change — without writing anything. Useful for previewing changes
before committing, or for CI checks that verify tangle is up to date.

For new files (not yet on disk), all lines are shown with a `+` prefix.

### Call Chain (`--callers`)

```bash
weft -e "Validate input" --callers project.weft
```

Shows the upward call chain from `@o` roots down to the named fragment
as an indented tree. Answers "who uses this fragment?" transitively.
Useful for understanding how a fragment fits into the larger program.

### Static Analysis (`--lint`)

```bash
weft --lint project.weft
```

Post-parse analysis that reports:
- **Unused fragments** — defined but never referenced by any `@o` or other `@d`
- **Undefined references** — used in a `@<Name@>` but never defined
- **Fuzzy suggestions** — for undefined references with a close match
  (Levenshtein distance ≤ 3), suggests the likely intended name

### Error Annotation (`--errors`)

```bash
make 2>&1 | weft --errors
```

Reads compiler or linter output from stdin. For each `file:line:` pattern,
looks up the corresponding `.weft` source location by parsing section
markers in the tangled output file. Prints the original error line
followed by an indented annotation:

```
src/output.c:42: error: undeclared identifier
  [weft: literate/map-output.weft:77, scrap 204]
```

No `.weft` source files are needed — operates entirely on section markers
already embedded in the tangled output. Works with any compiler or linter
that uses `file:line:` format (gcc, clang, rustc, eslint, etc.).

### Built-in Help (`--help`)

If you lose context during a session, run:

```bash
weft --help
```

The output includes everything needed to reorient: what weft is, how it
works, all options, examples, a directives quick reference, and an
AI-specific section with rules and a step-by-step workflow.

### AI Workflow

The map, extract, and reverse map features enable a **directed** workflow
for AI agents:

1. **Orient**: `weft --help` → understand what weft is and how to use it
2. **Map**: `weft -m --bodies --prose project.weft | jq` → discover structure with code and context
3. **Identify**: find the fragment relevant to the task
4. **Extract**: `weft -e "fragment name" project.weft` → get exactly the code needed
5. **Trace callers**: `weft -e "name" --callers project.weft` → understand who uses a fragment
6. **Edit**: modify the `.weft` source at the indicated location
7. **Preview**: `weft --diff project.weft` → see what tangle would change
8. **Verify**: re-tangle and test
9. **Debug**: `make 2>&1 | weft --errors` → translate compiler errors to `.weft` source
10. **Lint**: `weft --lint project.weft` → check for unused fragments and undefined refs

The `--errors` flag closes the loop: pipe compiler output through weft and
every `file:line:` error is annotated with the `.weft` source location.
No `.weft` files need to be loaded — it reads section markers from the
tangled output. For single lookups, `weft -R file:line` works too.


## Markdown Output

weft can weave documentation to Markdown (in addition to LaTeX):

```bash
weft -w md file.weft    # weave to Markdown
```

Or set it per-file with the `@W` directive in the source:

```
@W md
```

The Markdown output includes:
- HTML anchors for cross-references (`<a id="weft1"></a>`)
- `<pre><code>` blocks for scraps with syntax highlighting via `@l`
- Sequential scrap numbering (forced with Markdown output)
- Cosmetic fence stripping — fences (` ``` `) in the `.weft` source
  that exist only for editor syntax highlighting are stripped from weave output

This is ideal for GitHub-rendered documentation alongside your literate source.


## Architecture: Organization by Concept

Each `.weft` file organizes code by **concept**, not by layer:

```
literate/
├── concepts/
│   ├── auth.weft       # Everything about authentication
│   ├── payments.weft   # Everything about payments
│   └── notifications.weft # Everything about notifications
└── assembly.weft     # @o directives → physical files
```

One concept file contains the model, logic, API, and tests for a
single business concept — woven into a narrative. The assembly file
distributes fragments to their layer-oriented locations on disk.

**Benefits**:
- Read one file to understand one concept completely
- Changes to a concept touch one file, not five
- The narrative captures *why* decisions were made
- Tangling produces the layer-oriented structure tools expect

See [references/concept-architecture.md](references/concept-architecture.md)
for the detailed architecture guide. For a cross-system example (one concept
spanning Go, Flutter, SQL, Protobuf, and Shell), see
[references/worked-example.md](references/worked-example.md).


## Build Pipeline

Every literate project follows this pipeline:

```
.weft files → weft tangle → output files → language tools → build
```

Managed with a `justfile`:

```bash
just tangle    # .weft → weft → output files
just build     # tangle + compile/format
just test      # tangle + test
just doc       # weft -w md → Markdown docs
just map       # weft -m → JSON structure map
just all       # full pipeline
just clean     # remove generated files
```

**Key rule**: never edit tangled output. All changes go through `.weft` files.

See [references/build-pipeline.md](references/build-pipeline.md)
for the complete pipeline guide with language-specific examples. For
multi-system pipelines (one tangle producing outputs for multiple
languages), see [references/worked-example.md](references/worked-example.md).


## Writing Conventions

### Chunk Naming
Use `Domain: description` format:
- `Auth: validate token` — domain is Auth, describes the chunk
- `Payments: process refund` — clear, scannable in indices
- `Paradigm: philosophy` — for skill output chunks
- `Ref topic: section` — for reference file chunks

### Narrative Rules
- Explain **why**, not just what — the code shows what
- Introduce concepts before using them
- One paragraph of prose per chunk minimum
- Use cross-references: "as we defined in `@<Auth: user model@>`"
- Chunks are **cognitive units** — each ~one screenful, representing
  one idea a reader can absorb at a glance
- When in doubt, err on verbosity — too much prose beats too little

### Anti-patterns to Avoid
- Chunks with no surrounding prose (code dump)
- Generic names like "helper" or "utils"
- Editing tangled output instead of `.weft` source
- Monolithic `.weft` files (split by concept)
- Marking a change as done without updating the prose — code and
  narrative must stay in sync; stale prose is a bug

See [references/naming-conventions.md](references/naming-conventions.md)
for the complete conventions guide.

See [references/literate-craft.md](references/literate-craft.md)
for the complete craft guide: narrative patterns, prose quality,
migration strategies, and review checklists.


## Worked Example: Cross-System Concepts

A single `.weft` file can define a business concept that spans **multiple
systems and languages**. "Payments" might touch Go (backend), Flutter
(app), SQL (database), Protobuf (contracts), and Shell (deployment) ---
all narrated in one file, tangled to their respective locations.

Example structure of a cross-system `payments.weft`:

```
@<Payments: schema GraphQL@>       → tmp/go-service/gql/schema/payment.graphql
@<Payments: Go entity@>            → tmp/go-service/internal/entity/payment.go
@<Payments: migration@>            → tmp/go-service/migrations/003_payments.sql
@<Payments: Flutter model@>        → tmp/flutter-app/lib/features/payments/model.dart
@<Payments: Flutter screen@>       → tmp/flutter-app/lib/features/payments/screen.dart
@<Payments: proto gRPC@>           → tmp/proto/payments/v1/fraud.proto
@<Payments: deploy script@>        → tmp/scripts/deploy-payments.sh
@<Payments: seed data@>            → tmp/go-service/seeds/payments.sql
```

The narrative explains the **complete flow**: user initiates payment in
Flutter, GraphQL mutation hits Go, Go validates and queries PostgreSQL,
calls fraud detection via gRPC, returns result. One file, one story,
all layers.

This is language-agnostic. The same pattern works for any combination:
Python + React + SQL, Rust + Swift + Terraform, etc. What changes is
the output paths and post-tangle tools, not the LP structure.

See [references/worked-example.md](references/worked-example.md)
for the complete worked example with full code.
