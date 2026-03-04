---
name: literate-programming
description: >
  Literate programming paradigm with weft for any project.
  Each .weft file is a self-contained narrative crossing all architectural
  layers. Pipeline: .weft → weft tangle → output files.
  Triggers: "literate programming", "programacion literaria", "weft",
  "nuweb", ".weft files", ".w files", "weft skill", "literate skill".
---


# Literate Programming with weft

A paradigm where **documentation is the source code**. You write `.weft` files
(or `.w` as shorthand) as narratives with embedded code chunks. weft tangles
them into executable output and optionally weaves them into documentation.

This skill covers the generic paradigm (any language). It was itself
built with weft.



## Philosophy

Literate programming inverts code and documentation: you write a **narrative
first**, with code woven in. The `.weft` file is the single source of truth.

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
use `-w md` or `-w tex` to activate weave.


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

1. **Error at `server.js:47`** → open the tangled file at that line
2. **Scan upward** for nearest opening marker: `// {3: routes.weft:42}`
3. **Go to the `.weft` source** at `routes.weft:42` — the scrap with the
   bug, surrounded by prose explaining its intent
4. **Fix in `.weft`**, re-tangle, done

### Language Override

- `@l lang` placed before an `@o`/`@d` directive overrides auto-detection
- `@L name style [+d]` defines new languages in the preamble

See [references/debugging-tangled-code.md](references/debugging-tangled-code.md)
for the complete debugging guide with examples by language.


## AI-Assisted Navigation

weft provides two features designed for AI-assisted workflows: **JSON maps**
and **fragment extraction**. Together they enable precise, directed navigation
of literate codebases.

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

### AI Workflow

The map and extract features enable a **directed** workflow for AI agents:

1. **Map first**: `weft -m | jq` → understand the project structure
2. **Identify**: find the fragment relevant to the task
3. **Extract**: `weft -e "fragment name"` → get exactly the code needed
4. **Edit**: modify the `.weft` source at the indicated location
5. **Verify**: re-tangle and test

This approach uses ~6 directed operations instead of ~14 exploratory file
reads, because the map provides the full dependency graph upfront.


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

See [references/naming-conventions.md](references/naming-conventions.md)
for the complete conventions guide.


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
