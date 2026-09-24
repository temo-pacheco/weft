
# The Craft of Literate Programming

A literate program is an essay, not commented code. The syntax reference
tells you **what** weft can do; the philosophy guide tells you **why**;
this document tells you **how** to write literate programs that achieve
publishable quality — programs worth reading from beginning to end.

Excellence has three dimensions: exposition (the narrative teaches),
software quality (the code is correct and maintainable), and
debuggability (errors trace back to their source). This guide addresses
the first dimension — the craft of composing the narrative itself.


## 1. The Standard: Literary Works of Programming

Knuth's standard is explicit: literate programs should be **literary and
artistic works of programming**. The bar is not "well-commented code" —
it is a document that a reader can follow from start to finish, as one
reads a well-written book.

> "A programmer is ideally an essayist who works with traditional
> aesthetic and literary forms as well as mathematical concepts, to
> communicate the way that an algorithm works and to convince a reader
> that the results will be correct." — Donald Knuth

> "Some programs are elegant, some are exquisite, some are sparkling.
> My claim is that it is possible to write grand programs, noble
> programs, truly magnificent ones!" — Donald Knuth, *Computer
> Programming as an Art* (1974 Turing Award Lecture)

What publishable quality looks like:

| Dimension | Mediocre | Publishable |
|-----------|----------|-------------|
| Prose | "Here is the validation function" | "Users must provide a valid JWT. We check signature, expiration, and issuer — if any fails, we return 401 immediately rather than risk a compromised identity." |
| Chunk flow | Three chunks in a row with no prose | Each chunk introduced by a paragraph of motivation, connected to the next by a bridging sentence |
| Names | `@d helper` | `@d Auth: validate JWT expiration` |
| Structure | 2000-line monolithic file | One concept file per business idea, each ~200–400 lines |
| Reader test | Reader must jump between files to understand | Reader understands the concept by reading one file top-to-bottom |

The practical test: **would a new teammate understand this system by
reading the document alone, without asking questions?** If not, the
narrative needs work.



## 2. Narrative Patterns

Every chunk needs surrounding prose. But *what* prose? These four
patterns handle the most common situations.

### Starting a Project: Problem Statement First

Before writing any code — before even creating chunk definitions —
write the **problem statement** as pure prose. Gradman observed that
teams who start with a `.weft` file consisting of a problem statement
and its constituent parts, then let the project grow from inception,
produce better results than teams who write code first and document
later.

In practice: create the concept file and write 2–3 paragraphs
describing what the concept IS, what problem it solves, and what
constraints it operates under. Only then begin defining chunks. This
prose becomes the permanent introduction to the concept file, and it
forces you to think through the problem before solving it.

```
## Payment Processing

Our platform needs to handle payment transactions between users and
merchants. A payment flows through five states: pending → processing
→ approved (or rejected or fraud_detected). The user initiates in the
mobile app, the backend validates and persists, and a separate fraud
detection service evaluates risk via gRPC.

Key constraints: payments are immutable after creation (state changes
produce new records), amounts must be positive (enforced at the
database level), and fraud detection must complete before persistence
to prevent storing rejected payments.

[chunks begin here]
```

### Opening a Concept

**Pattern**: context → problem → approach → code

Before writing any code in a concept file, orient the reader. What is
this concept? What problem does it solve? What approach did we choose?

**Bad:**
```
## Token Validation

@d Auth: validate token @{
func validateToken(t string) error { ... }
@}
```

**Good:**
```
## Token Validation

Every API request carries a JWT in the Authorization header. We must
verify three properties before trusting the caller's identity: the
cryptographic signature (was this token issued by us?), the expiration
timestamp (is it still valid?), and the issuer claim (does it match
our service name?). If any check fails, the request is rejected at
the gateway — no business logic ever sees an unvalidated identity.

We chose JWT over opaque tokens because our microservices need to
validate tokens without calling a central auth server on every request.

@d Auth: validate token @{
func validateToken(t string) error { ... }
@}
```

### Introducing a Chunk

**Pattern**: one paragraph of "why" before every `@d`

The minimum: one paragraph explaining *why this code exists* and *what
decision it embodies*. Not what the code does — the code shows that.

**Bad:**
```
@d Auth: hash password @{
func hashPassword(p string) (string, error) {
    return bcrypt.GenerateFromPassword([]byte(p), 12)
}
@}
```

**Good:**
```
We use bcrypt with a cost factor of 12 for password hashing. bcrypt is
intentionally slow, making brute-force attacks impractical. Cost 12
takes ~250ms per hash on current hardware — high enough for security,
low enough that login doesn't feel sluggish. We chose bcrypt over
argon2 because our deployment targets lack the memory guarantees that
argon2 requires.

@d Auth: hash password @{
func hashPassword(p string) (string, error) {
    return bcrypt.GenerateFromPassword([]byte(p), 12)
}
@}
```

### Bridging Chunks

**Pattern**: one sentence connecting the chunk just completed to the
chunk about to begin

Without bridges, a sequence of chunks reads like a list of disconnected
parts. A single bridging sentence creates narrative flow:

**Bad:**
```
@d Auth: validate token @{...@}

@d Auth: extract claims @{...@}
```

**Good:**
```
@d Auth: validate token @{...@}

Once the token's integrity is confirmed, we extract the claims — the
user ID, roles, and expiration — that downstream handlers will need.

@d Auth: extract claims @{...@}
```

### Closing a Section

**Pattern**: summarize what was built, foreshadow what's next

At the end of a major section (not every chunk — just logical
boundaries), tell the reader what they now have and where the story
goes:

```
At this point the authentication subsystem is complete: tokens are
issued at login, validated on every request, and refreshed before
expiration. What remains is authorization — deciding what an
authenticated user is *allowed* to do. That logic lives in
`@<Auth: check permissions@>`, which we define next.
```



## 3. The Hierarchy of "Why"

The most common prose failure is explaining *what* the code does instead
of *why*. But "why" operates at multiple levels. A publishable literate
program addresses all five:

### Level 1: Why This Approach

The algorithmic or architectural choice. This is the most important
"why" — it justifies the fundamental approach.

> We use a bloom filter for membership testing because the dataset has
> 10M entries and we can tolerate a 1% false positive rate. A hash set
> would use ~400MB; the bloom filter uses ~12MB.

### Level 2: Why This Structure

The data representation choice. Why this type, this shape, this layout?

> Payments are stored as a struct with value semantics (not a pointer)
> because they are immutable after creation — the status field changes
> via state machine transitions that return new Payment values.

### Level 3: Why This Order

The sequencing rationale. Why do operations happen in this particular
order?

> We validate the JWT *before* parsing the request body. This is
> intentional: body parsing is expensive (it may buffer large uploads),
> and we want to reject unauthenticated requests as cheaply as possible.

### Level 4: Why This Name

Naming justification for non-obvious names. Most names are
self-documenting, but some deserve explanation:

> The variable `stale` (not `expired` or `invalid`) distinguishes
> tokens that were once valid but have passed their expiration from
> tokens that were never valid. A stale token triggers a refresh flow;
> an invalid token triggers re-authentication.

### Level 5: Why This Constraint

Business rules and invariants that the code enforces:

> The `CHECK (amount > 0)` constraint lives in the database, not in
> application code, because no code path — including direct SQL access
> by operations staff — should ever bypass it. This is a business
> invariant, not a validation convenience.

You don't need all five levels for every chunk. But if you find yourself
writing only Level 1 prose, consider whether Levels 2–5 would help
a future reader.



## 4. Prose-to-Code Ratio

### Guidelines

- **Minimum**: one paragraph per chunk. No exceptions.
- **Typical**: 1:1 ratio — roughly as many lines of prose as lines of
  code. This is a rough heuristic, not a rule.
- **More prose needed**: complex algorithms, non-obvious decisions,
  business rules, security-sensitive code, anything that took more
  than a few minutes to think through.
- **Less prose acceptable**: trivial glue code, mechanical boilerplate,
  straightforward type definitions. Even here, one sentence of context
  ("The config struct mirrors the YAML structure defined in the ops
  guide") beats silence.

### Warning Signs

Three consecutive chunks with no prose between them is a red flag.
It means the author stopped *writing* and started *coding*. Go back
and add the narrative.

A `.weft` file that's mostly code with a few introductory sentences
is not a literate program — it's source code with a header comment.

### The Teammate Test

Before committing, read the file as if you were a new hire seeing the
project for the first time. At every chunk, ask:

- Do I understand *why* this code exists?
- Do I understand *why* it works this way and not some other way?
- Could I confidently modify this code without breaking something?

If any answer is "no," the prose is insufficient.

### Write for Future-You, Not Present-You

Childs observed that the value of literate programming becomes most
evident when looking back at old code — the narrative preserves
understanding that would otherwise be lost. The prose you write today
is not for the you who just figured out the algorithm; it's for the
you six months from now who has forgotten every detail.

**Practices:**

- Don't explain what is obvious *to you right now*. Explain what will
  be **opaque in six months**: why this approach instead of the simpler
  one, why this order matters, why this edge case exists.
- Document the **dead ends**: "We tried X first but it failed because
  Y." This prevents future-you from re-exploring paths already known
  to be wrong.
- State **invariants explicitly**: "This function assumes the input is
  already validated — callers must call `@<Auth: validate token@>`
  first." Future-you won't remember implicit contracts.
- When the code looks wrong but is correct, say so: "The double-fetch
  is intentional — the cache may be invalidated between validation
  and use." This prevents future-you from "fixing" correct code.



## 5. Reading Order vs Writing Order

### The Writer's Problem

Writers typically build bottom-up: define helper functions first, then
compose them. This is natural — you can't test a function that calls
undefined helpers.

### The Reader's Need

Readers need top-down flow: the concept first, the details second, the
edge cases last. A reader encountering a helper function before
knowing *what it helps* learns nothing from it.

### The Solution: Restructure After Writing

Literate programming exists precisely to solve this mismatch. As Knuth
discovered:

> "The fact that there's no need to be hung up on the question of
> top-down versus bottom-up — since a programmer can now view a large
> program as a web, to be explored in a psychologically correct order
> — is perhaps the greatest lesson I have learned from my recent
> experiences." — Donald Knuth (1984)

weft lets you present code in any order regardless of how the compiler
needs it. Use this power:

1. **Write** in whatever order is natural (bottom-up, exploratory)
2. **Restructure** for reading: move the high-level orchestration to
   the top, push details and helpers down
3. **Bridge** with prose: "This function uses `@<Auth: validate token@>`,
   which we define in the next section"

### Multiple Audiences

A well-structured literate program serves three reading modes:

| Audience | Mode | What they need |
|----------|------|----------------|
| New developer | Full read, top-to-bottom | Conceptual understanding, motivation, architecture |
| Maintainer | Targeted: jump to a specific chunk | Context around the chunk, cross-references to related code |
| Reviewer | Scanning: skim prose, focus on code changes | Clear chunk boundaries, bridging prose that highlights intent |

Structure for the full-read audience; the other two are served
automatically by chunk naming and cross-references.

### Linear Reading vs. Selective Browsing

Knasmüller (*Reverse Literate Programming*) identified that readers
use literate programs in two fundamentally different modes:

1. **Linear** — reading like a book, front to back, to build
   understanding of the whole system
2. **Selective** — browsing like an encyclopedia, jumping to the
   specific chunk relevant to the task at hand

The linear reader needs narrative flow — bridging prose, foreshadowing,
section summaries. The selective reader needs **navigation anchors** —
chunk names that are meaningful in isolation, cross-references that
point to dependencies, and self-contained sections that make sense
without reading everything before them.

**Practices for selective readers:**

- Make every chunk's introductory paragraph state what the chunk does
  AND what it depends on: "This handler uses `@<Auth: validate token@>`
  (defined above) and `@<DB: get user by ID@>` (from `database.weft`)."
- Use the `Domain: description` naming convention consistently — it
  turns the `@m` index into a navigable table of contents
- When a chunk references a concept defined elsewhere, name the file:
  "the migration schema (in `payments.weft`)" — so the selective reader
  knows where to jump
- `weft -e "chunk name"` extracts a chunk and its transitive
  dependencies — write chunks that are useful when extracted this way



## 6. Testing as Narrative

### Tests Are Part of the Story

Tests are not an appendix — they are integral to the narrative. They
demonstrate that the code works as described and document the expected
behaviour more precisely than prose alone.

### Pattern: "Let's Verify That..."

Introduce test chunks with prose that states the expectation:

```
Let's verify that expired tokens are rejected immediately. The
validation function should return an `ErrTokenExpired` error, not
a generic validation error, so that callers can distinguish
"please refresh" from "please re-authenticate."

@d Auth: test expired token @{
func TestValidateToken_Expired(t *testing.T) {
    token := makeExpiredToken()
    err := validateToken(token)
    assert.ErrorIs(t, err, ErrTokenExpired)
}
@}
```

### Placement: Near the Code They Verify

Place test chunks close to the code they test in the narrative — not
in a separate "Tests" section at the end. When a reader finishes
understanding the validation logic, the test immediately confirms
their understanding:

```
## Token Validation

[prose about validation approach]

@d Auth: validate token @{...@}

[test prose: "Let's verify that..."]

@d Auth: test expired token @{...@}
@d Auth: test invalid signature @{...@}

## Token Refresh

[next section of the narrative]
```

The assembly file routes test chunks to test files — the concept file
doesn't need to know.

### The Test as Documentation

A test that says `assert.ErrorIs(t, err, ErrTokenExpired)` documents
the contract more precisely than prose. Together, prose and test are
stronger than either alone: prose explains the *intention*, the test
proves the *behaviour*.



## 7. Cross-Concept Narrative

### Referencing Other Concept Files

When one concept depends on another, reference it explicitly in prose:

```
The payment processor calls the fraud detection service (defined in
`fraud-detection.weft`) via gRPC. The contract between them is the
Protobuf definition in `@<Fraud: proto contract@>`, which both
sides must respect.
```

This tells the reader where to find more detail without requiring them
to read the other file first.

### Establishing Contracts

When two concepts share a boundary (an API, a database table, a message
format), state the contract explicitly:

```
The payments concept writes to the `payments` table with status
'pending'. The notifications concept reads this table for rows with
status 'approved'. The contract: payments owns the write path;
notifications only reads.
```

### The Assembly File as Table of Contents

The assembly file (`assembly.weft`) serves a dual purpose:

1. **Technical**: routes chunks to output files with `@o` directives
2. **Narrative**: acts as a table of contents showing how concepts
   compose into the physical system

A well-organized assembly file is itself readable:

```
% === Database Layer ===
@o tmp/migrations/001_users.sql @{@<Auth: migration@>@}
@o tmp/migrations/002_payments.sql @{@<Payments: migration@>@}

% === Backend ===
@o tmp/server/auth/handler.go @{@<Auth: handler@>@}
@o tmp/server/payments/handler.go @{@<Payments: handler@>@}

% === Frontend ===
@o tmp/app/features/auth/screen.dart @{@<Auth: screen@>@}
@o tmp/app/features/payments/screen.dart @{@<Payments: screen@>@}
```

### Team Benefits of Cross-Concept Narrative

Mike Gradman observed that teams using literate programming can start
with a `.weft` file consisting of a problem statement and its
constituent parts, then let the project grow from inception. This
results in a better understanding of the application and its problem
domain, leading to fewer feature changes, more predictable testing,
and simpler debugging. The clarity of literate programs enables team
members to reuse existing code and to provide constructive feedback
during code reviews — because reviewing a literate program is reading
an essay, not deciphering a diff.

### Traceability: Every Reference Findable

Knuth introduced *mini-indexes* — local cross-reference indexes on
every page spread — so that at any point in reading, every identifier
is traceable to its definition. weft achieves the same through the
`@m` fragment index, the `@u` identifier index, and inline
cross-references.

**Practice**: when you mention a chunk, a function, or a data structure
in prose, make the reference explicit and traceable:

**Vague** (reader has to search):
```
The handler validates the token and returns the user.
```

**Traceable** (reader knows exactly where to look):
```
The handler calls `@<Auth: validate token@>` (defined in the
authentication section above) and then queries the user via
`@<DB: get user by ID@>` (from `database.weft`).
```

For identifiers (variables, functions, types) that appear in multiple
chunks, use the `@|` declaration at the end of the defining scrap.
This populates the `@u` index so that a reader — or an AI agent —
can find every definition and use:

```
@d Auth: token types @{
type TokenKind int
const (
    AccessToken  TokenKind = iota
    RefreshToken
)
@| TokenKind AccessToken RefreshToken @}
```

The more traceable your cross-references, the more navigable the
program becomes in both linear and selective reading modes.

### Avoiding Circular Narrative Dependencies

If concept A's prose says "see concept B" and concept B's prose says
"see concept A," the reader is stuck in a loop. Break circles by
establishing a reading order: one concept is foundational, the other
builds on it. The foundational one should be readable without knowing
the other.



## 8. Refactoring Literate Programs

### Splitting a Concept File

When a `.weft` file grows beyond ~500 lines, it likely covers more than
one concept. Split it:

1. Identify the distinct concepts within the file
2. Create a new `.weft` file for each extracted concept
3. Move the relevant `@d` chunks and their surrounding prose
4. Update cross-references in prose
5. Add `@i` include in the master file
6. Verify: `weft -m project.weft | jq` should show no undefined fragments

### Renaming Chunks

Chunk names are referenced in three places:

1. The `@d` definition
2. `@<...@>` invocations in other chunks
3. Prose text (informal references)

Use `weft -m | jq '.fragments'` to find all definitions, then search
the `.weft` files for the old name. Rename all three consistently.

### Moving Chunks Between Concept Files

When restructuring, you may need to move a chunk from one concept file
to another:

1. Cut the `@d` definition and its prose from the source file
2. Paste into the target file at the right narrative position
3. Update the `@i` order if the include order matters for concatenation
4. Verify: `weft -m project.weft | jq` — no warnings

### Maintaining Narrative Coherence

After any structural change, re-read the affected files top-to-bottom.
Check that:

- Bridging prose still makes sense (it may reference chunks that moved)
- Forward references ("as we'll see in...") still point forward
- Back references ("as we defined in...") still point backward
- The introduction still accurately describes the file's scope

### Verification

After any refactoring, verify structural integrity:

```bash
# Check for undefined or unreferenced fragments
weft -m project.weft | jq '.fragments | to_entries[] | select(.value.uses == 0)'

# Tangle and diff against the previous output
weft project.weft
diff -r tmp/ tmp-backup/    # output should be identical
```

If the tangled output changes after a pure refactoring, something was
lost in the move.



## 9. Migrating Existing Projects to LP

### Step 1: Identify Concepts (Not Files)

Don't mirror the existing file structure. Instead, identify the
**business concepts**: auth, payments, notifications, user profiles.
Each concept becomes one `.weft` file regardless of how many source
files it currently spans.

### Step 2: Create the Skeleton

```
project.weft                    # Master: @i includes
literate/
├── concepts/
│   ├── auth.weft              # Will hold all auth code + narrative
│   ├── payments.weft
│   └── ...
└── assembly.weft             # Will hold all @o directives
justfile                        # tangle + build pipeline
```

### Step 3: Migrate One Concept at a Time

Start with the most **self-contained** concept — the one with the
fewest dependencies on other concepts. This gives you a working
pipeline quickly.

For each concept:
1. Read all existing source files related to the concept
2. Write the narrative — explain the concept as if to a new teammate
3. Embed the existing code as `@d` chunks within the narrative
4. Add `@o` directives in `assembly.weft` pointing to the original file paths
5. Tangle and verify the output matches the original

### Step 4: Write the Narrative Around Existing Code

This is the hardest part. You're not just moving code — you're
explaining it. For each chunk, ask:

- Why does this code exist?
- Why is it structured this way?
- What alternatives were considered?
- What would break if this changed?

Write these answers as prose. This is where the value of LP is created.

### Step 5: Move Output Paths to Assembly

Ensure all `@o` directives live in `assembly.weft`, not in concept
files. Concept files should contain only `@d` definitions and prose.

### Step 6: Verify Tangle Produces Identical Output

```bash
# Save original files
cp -r src/ src-backup/

# Tangle the literate version
weft project.weft

# Compare
diff -r tmp/src/ src-backup/
```

The output must be **byte-identical** (modulo section markers, which
are new). If it differs, fix the `.weft` files until it matches.

### Step 7: Delete Original Source Files

Only after verification. The `.weft` files are now the source of truth.
The original source files become tangled output.

### Practical Tips

- **Use git**: commit the original, commit the LP migration, so you can
  always roll back
- **Migrate incrementally**: you can have some concepts in LP and others
  still as traditional source. The justfile handles both.
- **Don't rewrite the code**: the goal is to add narrative, not refactor.
  Refactoring can come later, as a separate step with its own narrative
  justification.
- **Section markers help**: after migration, section markers in the
  tangled output confirm that each line traces back to the right `.weft`
  source location



## 10. weft as Living Example

weft itself is written as a literate program — 15 `.weft` files in the
`literate/` directory. This is a real-world example of LP at scale in a
C codebase.

### How to Read weft's Source

Browse `literate/*.weft` to see how an actual literate project is
organized. Notable patterns:

- **`literate/introduction.weft`** — user-facing documentation (the
  `--help` output and man page narrative) treated as a first-class
  concept. Documentation is not separate from the code; it's a concept
  that generates output just like any other.

- **`literate/scraps.weft`** — the core tangle logic. This is one of
  the longest concept files, and it demonstrates how to narrate a
  complex algorithm: each step in the tangle process has prose explaining
  the approach, followed by the C code that implements it.

- **`literate/names.weft`** — data structures for fragment name management.
  Shows how to explain data representation choices in context: why a
  particular data structure was chosen, how it interacts with the
  rest of the system.

- **`literate/parser.weft`** — the weft input parser. Demonstrates
  the concept-crossing pattern: the parser touches lexing, error
  handling, and symbol table management, all narrated as one story.

- **`literate/arena.weft`** — memory management. A self-contained
  concept that other concepts depend on, showing how to write a
  foundational piece that makes sense on its own.

### What to Look For

When reading weft's source, notice:

1. **Prose before every chunk** — no chunk appears without context
2. **Forward references** — chunks are used before they are defined,
   with prose explaining what they will do
3. **The assembly pattern** — output files are assembled from chunks
   defined across multiple concept files
4. **Cross-references in prose** — concepts reference each other by
   chunk name, creating a navigable web

### LP and AI Agents

Recent research (*Renaissance of Literate Programming in the Era of
LLMs*, 2025) demonstrated that structuring code as literate programs
helps LLMs generate more contextually appropriate solutions for
complex projects. The reason is structural: a literate program's
chunk names, cross-references, and narrative prose form a **knowledge
graph** that AI agents navigate — the same structure that helps human
readers also helps AI maintain coherence across interdependent
components.

This means writing good LP IS writing good AI context. Practices
that serve both audiences:

- **Chunk names as semantic anchors** — `@d Auth: validate JWT
  expiration` tells both a human and an AI agent exactly what the
  chunk does. `@d helper3` tells neither.
- **Cross-references as edges** — when prose says "this handler uses
  `@<Auth: validate token@>`," the AI can trace dependencies just as
  a human reader can.
- **Problem-statement introductions** — the opening prose of each
  concept file gives an AI the context it needs to understand what
  modifications are appropriate and what invariants must be preserved.
- **`weft -m | jq`** — the JSON map is a machine-readable knowledge
  graph. An AI agent can query it to understand the project structure
  without reading every file.
- **`weft -e "chunk name"`** — extraction gives an AI exactly the
  code relevant to a task, with transitive dependencies, without
  loading the entire codebase.

weft's `-m`, `-e`, and `-R` features were designed with this dual
human-and-AI workflow in mind.



## 11. Review Checklist

Before considering a literate program complete, verify each dimension:

### Narrative Quality

| Check | Question |
|-------|----------|
| Why, not what | Does the prose explain *why*, not just restate the code? |
| Flow | Can a reader follow from top to bottom without jumping to other files? |
| Bridges | Is there prose connecting consecutive chunks? |
| Opening | Does the file open with context → problem → approach? |
| Closing | Do major sections summarize what was built? |

### Chunk Quality

| Check | Question |
|-------|----------|
| Size | Is each chunk one cognitive unit (~one screenful)? |
| Names | Do chunk names follow `Domain: description` pattern? |
| Prose coverage | Does every chunk have surrounding prose? |
| Self-contained | Can you understand a chunk's purpose from its name alone? |

### Structural Integrity

| Check | Question |
|-------|----------|
| Concept separation | Is each `.weft` file about one concept? |
| Assembly | Are all `@o` directives in the assembly file? |
| No orphans | Are there no defined-but-unused chunks? (`weft -m \| jq`) |
| Sync | Is prose consistent with the current code? |
| Tests | Are test chunks integrated into the narrative, not appended? |

### Build Verification

| Check | Question |
|-------|----------|
| Tangle | Does `just tangle` succeed without warnings? |
| Build | Does `just build` produce correct output? |
| Tests | Do `just test` pass? |
| Full pipeline | Does `just all` complete successfully? |

### Childs' Literacy Test

The final gate — from [naming-conventions.md](naming-conventions.md):

1. Is the level of documentation appropriate?
2. Does the documentation relate to the code at hand?
3. Are the code fragments appropriately concise?
4. Do the chunk names agree in tenor with their code?
5. Are the variable names appropriate?

If any answer is "no," the program compiles but is not yet *literate*.



## Further Reading

Key resources from the literate programming literature, many hosted
at [literateprogramming.com](http://www.literateprogramming.com/):

| Resource | Author | Why it matters |
|----------|--------|----------------|
| *Literate Programming* (1984) | Donald Knuth | The foundational paper — defines the paradigm and introduces WEB |
| *Computer Programming as an Art* (1974) | Donald Knuth | Turing Award lecture — programs as aesthetic objects |
| *Mini-Indexes for Literate Programs* (1994) | Donald Knuth | Technique for local cross-reference indexes on each page spread |
| *An Introduction to the WEB Style of LP* | Bart Childs | The 7 requirements for literate programs and 5 quality questions |
| *Literate Programming, A Practitioner's View* (1992) | Bart Childs | Practical experience and quality evaluation |
| *Literate Programming and Documentation Reuse* (1995) | Childs & Sametinger | Reuse analysis of TeX, METAFONT, and CWEB |
| FunnelWeb Tutorial Manual | Ross Williams | Philosophy of LP as "exposing the magic trick" |
| *Reading Computer Programs* (SEI-90-EM-3) | Deimel & Naveda | Techniques for reading and comprehending programs |
| *Literate Programming* (book, 1992) | Donald Knuth | Collection including "How to Read a WEB" — essential for readers |
| *Physically Based Rendering* | Pharr & Humphreys | LP at production scale — a ray tracer as a textbook |
| *Renaissance of LP in the Era of LLMs* (2025) | arXiv:2502.17441 | LP principles improve LLM code generation in large projects |
