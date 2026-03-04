
# Build Pipeline

## Pipeline Overview

```
┌──────────────┐     ┌─────────┐     ┌──────────────┐     ┌───────┐
│  .weft files │────→│  weft   │────→│ output files  │────→│ tools │
│ (source of   │     │ tangle  │     │ (tmp/ or src/)│     │ build │
│   truth)     │     └─────────┘     └──────────────┘     └───────┘
└──────────────┘           │
                           │         ┌──────────────┐     ┌───────┐
                           └────────→│  .md / .tex   │────→│render │
                          -w flag    │(documentation)│     │→ view │
                                     └──────────────┘     └───────┘
```

The `.weft` files are the only files you edit. Everything else is generated.

**Note**: weft tangles by default. Weave output requires the `-w` flag
(`-w md` for Markdown, `-w tex` for LaTeX).



## Justfile Template

A generic justfile for any literate project:

```just
# Project-specific variables
master := "project.weft"
out_dir := "tmp"

# Create output directories
setup:
    mkdir -p {{out_dir}}/src {{out_dir}}/test

# Tangle: extract source from .weft files
tangle: setup
    weft {{master}}

# Build: tangle + language-specific compile
build: tangle
    cd {{out_dir}} && go build ./...    # or: npm run build, cargo build, etc.

# Test: tangle + run tests
test: tangle
    cd {{out_dir}} && go test ./...     # or: npm test, cargo test, etc.

# Documentation: weave to Markdown
doc: setup
    weft -w md {{master}}

# JSON structure map
map:
    weft -m {{master}}

# Extract a specific fragment
extract name:
    weft -e "{{name}}" {{master}}

# Full pipeline
all: build test

# Clean generated files
clean:
    rm -rf {{out_dir}}/
    rm -f *.aux *.log *.out *.toc *.tex *.md
```

Adapt the `build` and `test` recipes for your language.



## Post-Tangle Steps by Language

After weft tangles the `.weft` files, you typically need language-specific
processing:

### Go
```bash
weft master.weft
cd tmp && go generate ./...    # sqlc, gqlgen, etc.
cd tmp && go build ./...
cd tmp && go test ./...
```

### Rust
```bash
weft master.weft
cd tmp && cargo fmt
cd tmp && cargo build
cd tmp && cargo test
```

### Flutter / Dart
```bash
weft master.weft
cd tmp && dart format .
cd tmp && flutter pub get
cd tmp && flutter build
```

### JavaScript / TypeScript
```bash
weft master.weft
cd tmp && npm install
cd tmp && npm run build
cd tmp && npm test
```

### Python
```bash
weft master.weft
cd tmp && black .
cd tmp && pytest
```

### Plain text / Markdown (e.g., Claude Code skills)
```bash
weft master.weft
cp -r tmp/skill/* ~/.claude/skills/my-skill/
```



## Watch Mode

For rapid iteration, use a file watcher to re-tangle on save:

### Using watchexec
```bash
watchexec -e weft,w -- just tangle
```

### Using entr
```bash
find . -name '*.weft' -o -name '*.w' | entr just tangle
```

### Using just (with a watch recipe)
```just
# Watch .weft files and re-tangle on change
watch:
    watchexec -e weft,w -- just tangle
```

Watch mode is especially useful during initial development when you're
iterating rapidly on the narrative and code together.



## Version Control

### What to commit

Only the `.weft` source files and the build infrastructure are source code.
Everything under `tmp/` is generated and must not be committed:

```gitignore
# .gitignore for a literate project
tmp/
*.aux
*.log
*.out
*.toc
*.tex
*.dvi
*.pdf
```

### What IS your repository

```
repo/
├── project.weft           # ✓ commit (master document)
├── literate/
│   ├── concepts/*.weft   # ✓ commit (source of truth)
│   └── assembly.weft    # ✓ commit (assembly)
├── justfile               # ✓ commit (build automation)
├── .gitignore             # ✓ commit
└── tmp/                   # ✗ NEVER commit (generated)
```

**Rule**: if `weft` produces it, don't commit it. A new developer
clones the repo, runs `just all`, and gets the full output. The `.weft`
files are the only source of truth.

### Commit discipline

When you change a `.weft` file, the commit message describes the *concept*
change, not the generated files: "Add fraud detection to payments" rather
than "Update payment.go, payment.dart, fraud.proto." The generated files
don't appear in your diff because they're gitignored.



## Troubleshooting

### "File not found" during tangle
weft won't create directories. Add a `setup` recipe that creates all
needed output directories before tangling.

### Tangled output has wrong indentation
weft preserves the indentation of chunk references. If `@<Chunk@>`
appears indented by 4 spaces, the chunk content gets 4 extra spaces.
Adjust the reference indentation, not the chunk content.

### Changes not reflected after tangle
Make sure you're editing the `.weft` files, not the tangled output.
The tangled files are overwritten every time you run `weft`.

### Section markers help locate errors
When a compiler or runtime error points to a line in tangled output,
open that file and look for the nearest `{N: file.weft:line}` marker
above the error line. It tells you exactly which `.weft` file and line
produced that code.

### Chunk name "not defined" error
Check for exact name match including capitalization and whitespace.
`@<Setup database@>` and `@<Setup Database@>` are different chunks.
Use the `@m` index or `weft -m | jq` to verify chunk names.
