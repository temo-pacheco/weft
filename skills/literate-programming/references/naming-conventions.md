
# Writing Conventions

## Chunk Naming System

### The Domain:Description Pattern

Every chunk name follows: `Domain: description`

```
@d Auth: user entity           → entity definition
@d Auth: validate credentials  → business logic
@d Auth: login handler         → API endpoint
@d Auth: login test            → test cases
```

The domain prefix groups related chunks in the index, making it easy
to find all chunks related to a concept.

### Special Prefixes

For projects that generate multiple output types:

| Prefix | Purpose | Example |
|--------|---------|---------|
| `Domain: ...` | Code chunks | `Auth: validate token` |
| `SKILL: ...` | Main output sections | `SKILL: philosophy` |
| `Ref topic: ...` | Reference file sections | `Ref auth: overview` |
| `Config: ...` | Configuration chunks | `Config: database URL` |
| `Test: ...` | Test-specific chunks | `Test: auth integration` |
| `Migration: ...` | Database migrations | `Migration: add users table` |

### Naming Guidelines

1. **Be specific** — `Auth: validate JWT expiration` over `Auth: validate`
2. **Use natural language** — chunk names should read as phrases
3. **Be consistent** — pick a convention and stick with it project-wide
4. **Think about the index** — names should be useful when alphabetized



## Narrative Style

### The Why-First Rule

Every chunk should be preceded by prose that explains *why* this code
exists. The reader should understand the motivation before seeing the
implementation.

**Good:**
> Users must provide a valid JWT token with each request. We validate
> the token's signature, expiration, and issuer claim. If any check
> fails, we return 401 Unauthorized immediately rather than proceeding
> with a potentially compromised identity.

```
@d Auth: validate token @{...@}
```

**Bad:**
> Here is the token validation function:

```
@d Auth: validate token @{...@}
```

### Narrative Flow

Structure each concept file as a mini-essay:

1. **Introduction** — what is this concept? Why does it exist?
2. **Domain model** — the core entities and their relationships
3. **Business logic** — the rules and processes
4. **Interface** — how this concept connects to the outside world
5. **Tests** — verification that it works correctly

Each section has prose explaining the reasoning, with code chunks
embedded at the natural points in the narrative.

### Cognitive Chunks and Sizing

Childs emphasized that chunks should be **cognitive chunks** — units
sized to what a human reader can absorb at once. Each chunk represents
one coherent idea, not an arbitrary division of code.

**Guideline**: approximately one screenful per chunk. This is a guideline,
not a rule — some chunks will be shorter (a type definition) and some
longer (a complex algorithm). But if a chunk sprawls beyond what fits
on a screen, consider splitting it.

Chunk names should be **descriptive pseudo-code**: they act as named
abstractions in a human-readable meta-language. When you read
`@<Auth: validate token@>` at a call site, you should understand what
it does without reading its contents. As Knuth envisioned, the literate
programmer selects names "with thesaurus in hand."

### When in Doubt, Err on Verbosity

> "Good documentation and code will be concise, however it is better to
> err on the side of slight excess in documentation rather than too
> little." — Bart Childs

Literate programs are **not created accidentally** — they require
concentrated effort. It is tempting to skip prose for "obvious" chunks,
but a chunk that seems obvious to the author today may be opaque to a
reader (or the same author) six months later. Write the prose.

### Cross-References

Use chunk references in prose to connect related concepts:

> The authentication handler (defined in `@<Auth: login handler@>`)
> calls the validation logic from `@<Auth: validate token@>` before
> creating a session.

This creates a web of connections that helps readers navigate the
codebase.



## Anti-Patterns

### 1. Code Dump
A `.weft` file that's just code chunks with no prose. This defeats the
entire purpose of literate programming. Every chunk needs surrounding
narrative.

### 2. Comment-Style Prose
Writing prose that just restates the code: "This function adds two
numbers and returns the result." Instead, explain *why* this operation
exists and what design decisions it embodies.

### 3. Monolithic Files
A single `.weft` file with thousands of lines covering multiple concepts.
Split by concept — each `.weft` file should tell one coherent story.

### 4. Editing Tangled Output
Making changes to generated files instead of the `.weft` source. These
changes are lost on the next tangle. Always edit `.weft` files.

### 5. Orphan Chunks
Chunks that are defined but never referenced. Use the `@m` index or
`weft -m | jq` to find unreferenced chunks and either use them or
remove them.

### 6. Generic Names
Chunk names like `helper`, `utils`, `process`, `handle` that don't
convey meaning. Every chunk name should be specific enough to
understand without reading the content.

### 7. Layer-Oriented .weft Files
Organizing `.weft` files by layer (models.weft, handlers.weft) instead of by
concept (auth.weft, payments.weft). This loses the main benefit of literate
programming — seeing a complete concept in one place.



## Quality Checklist

Before considering a `.weft` file complete, verify:

### Narrative
- [ ] Every chunk has surrounding prose explaining *why*
- [ ] The file reads coherently from top to bottom
- [ ] A newcomer could understand the concept from this file alone
- [ ] Design decisions and trade-offs are documented

### Naming
- [ ] All chunks follow the `Domain: description` convention
- [ ] Names are specific and meaningful
- [ ] Names read well in the generated index

### Structure
- [ ] One concept per `.weft` file
- [ ] Concept files have `@d` only (no `@o`)
- [ ] Assembly file has all `@o` directives
- [ ] Master file has `@i` includes and indices

### Build
- [ ] `just clean && just tangle` succeeds
- [ ] All output files are syntactically valid
- [ ] No warnings from weft
- [ ] Tests pass on the tangled output

### Meta
- [ ] No orphan chunks (defined but unused)
- [ ] No undefined chunk references
- [ ] `@` characters properly escaped in output
- [ ] Cross-references between chunks are accurate

### Literacy Review (Childs' Questions)

After the structural checks above, evaluate the *quality* of the literate
program with these questions:

- [ ] Is the level of documentation appropriate? (not too sparse, not padding)
- [ ] Does the documentation relate to the code at hand? (not generic filler)
- [ ] Are the code fragments appropriately concise? (~one screenful each)
- [ ] Do the chunk names agree in tenor with their code? (name matches content)
- [ ] Are the variable names appropriate? (descriptive words, not acronyms)

If any answer is "no," the program compiles but is not yet *literate*.
