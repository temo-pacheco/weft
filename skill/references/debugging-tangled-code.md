
# Debugging Tangled Code

## The Problem

When you work with literate programming, the code you run is **tangled
output** — generated files assembled from scraps scattered across `.weft`
files. An error at `server.js:47` doesn't correspond to line 47 of any
file you wrote. It's line 47 of a file weft assembled from fragments in
`server.weft`, `routes.weft`, and `database.weft`.

Without markers, debugging means mentally reconstructing which scrap
produced which line. With markers, the tangled file tells you directly.


## Section Markers: The Knuth Solution

Weft wraps every scrap in the tangled output with **opening** and
**closing** comment markers. They use the comment syntax of the target
language and follow this format:

```
COMMENT_START {SCRAP_NUMBER: SOURCE_FILE:LINE} COMMENT_END
...code from this scrap...
COMMENT_START {:SCRAP_NUMBER} COMMENT_END
```

### Reading the Markers

Each marker encodes three pieces of information:

| Part | Meaning |
|------|---------|
| `{` or `{:` | Opening or closing delimiter |
| Number | Scrap ID — unique across the entire document |
| `file:line` | Source `.weft` file and line number (opening only) |

### Examples by Language

**JavaScript** (`//` comments):
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

**Python** (`#` comments):
```python
# {5: models.weft:31}
class User:
    def __init__(self, name, email):
        self.name = name
        self.email = email
# {:5}
```

**C** (`/* */` comments + `#line` directives):
```c
#line 12 "memory.weft"
/* {2: memory.weft:12} */
void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        /* {4: errors.weft:8} */
        fprintf(stderr, "out of memory\n");
        exit(1);
        /* {:4} */
    }
    return ptr;
}
/* {:2} */
```

**Lua** (`--` comments):
```lua
-- {9: config.weft:55}
local defaults = {
  port = 8080,
  host = "localhost",
}
-- {:9}
```

**HTML** (`<!-- -->` comments):
```html
<!-- {6: layout.weft:20} -->
<nav class="sidebar">
  <!-- {11: navigation.weft:7} -->
  <ul>
    <li><a href="/">Home</a></li>
  </ul>
  <!-- {:11} -->
</nav>
<!-- {:6} -->
```


## Debugging Workflow

### Step 1: Locate the Error in Tangled Output

You get an error like:

```
TypeError: Cannot read property 'id' of undefined
    at Object.<anonymous> (server.js:47)
```

Open `server.js` and go to line 47. You'll see something like:

```js
// {3: routes.weft:42}
app.get("/api/users/:id", async (req, res) => {
  const user = await getUser(req.params.id);    // ← line 47
  res.json(user.id);                            // ← crashes here
});
// {:3}
```

### Step 2: Read the Nearest Opening Marker

Scan **upward** from the error line until you find an opening marker:

```
// {3: routes.weft:42}
```

This tells you:
- **Scrap 3** generated this code
- It was defined in **`routes.weft`** starting at **line 42**

### Step 3: Go to the Literate Source

Open `routes.weft` at line 42. You'll find the scrap with the bug,
surrounded by the prose that explains its intent:

```
We fetch the user by ID from the database. If the user doesn't
exist, we should return a 404, but currently we assume the user
always exists — this is the bug.

@d Handle user fetch
@{const user = await getUser(req.params.id);
res.json(user.id);
@}
```

The prose makes the bug obvious: there's no null check. Fix it in
the `.weft` file, re-tangle, and the error is gone.

### Step 4: Nested Markers = Nested Fragments

When fragments invoke other fragments, markers nest:

```js
// {3: routes.weft:42}
app.get("/api/users/:id", async (req, res) => {
  // {7: database.weft:18}
  const user = await db.query("SELECT * FROM users WHERE id=$1", [req.params.id]);
  // {:7}
  if (!user) return res.status(404).json({error: "not found"});
  res.json(user);
});
// {:3}
```

If the bug is inside the `{7:...:7}` block, go to `database.weft:18`,
not `routes.weft:42`. The **innermost enclosing markers** point to the
most specific source location.

**Rule**: always look at the **tightest pair** around the error line.


## Interpreting Nested Hierarchies

Complex tangled output can have several nesting levels. Read them
like parentheses — each opening matches its corresponding close:

```c
/* {1: main.weft:10} */          ← outermost: main file structure
#include <stdio.h>
/* {2: io.weft:25} */            ← middle: I/O subsystem
void print_report() {
    /* {5: format.weft:8} */     ← innermost: formatting detail
    printf("Report v%d\n", VERSION);
    /* {:5} */
}
/* {:2} */
int main() {
    /* {3: init.weft:14} */
    init_subsystems();
    /* {:3} */
    print_report();
    return 0;
}
/* {:1} */
```

The scrap numbers are **document-global** — scrap 1 is the first
`@{...@}` in the entire woven document, scrap 5 is the fifth.
They're stable as long as you don't reorder scraps.


## C/C++: #line Directives

For C-family languages, weft also generates `#line` directives
**above** the opening marker. These make the C compiler itself
report errors against the `.weft` source file:

```c
#line 12 "memory.weft"
/* {2: memory.weft:12} */
void *safe_malloc(size_t size) { ... }
/* {:2} */
```

Now `gcc` will say:

```
memory.weft:14: error: implicit declaration of function 'malloc'
```

instead of:

```
memory.c:47: error: implicit declaration of function 'malloc'
```

The `#line` directive and the section marker are complementary:
- `#line` → compiler reports errors against the `.weft` file directly
- `{N: file:line}` → human-readable when browsing the tangled output


## Quick Reference: What Activates Markers

Markers are activated **automatically** when weft detects the
language of the output file. Three detection methods, in priority
order:

| Priority | Method | Example | Where |
|----------|--------|---------|-------|
| 1 (highest) | Explicit flags on `@o` | `@o file.c -d -cc` | `.weft` or `.w` |
| 2 | `@l` override | `@l js` inside a scrap | `.weft` or `.w` |
| 3 (auto) | File extension | `.js`, `.py`, `.go`, etc. | Automatic |
| 4 | `@L` definition | `@L nim #` in preamble | `.weft` or `.w` |

If no language is detected, no markers are generated (backward
compatible with older behavior).


## Common Debugging Scenarios

### Scenario: Runtime error with stack trace

```
Error: ENOENT: no such file or directory, open '/data/config.json'
    at Object.openSync (fs.js:498:3)
    at readConfig (server.js:123)
```

1. Open `server.js:123`
2. Find nearest opening marker: `// {12: config.weft:35}`
3. Open `config.weft:35` — the scrap that reads config
4. Fix the path or add existence check in the `.weft` file

### Scenario: Compiler error in C

```
parser.c:284: error: 'token_type' undeclared
```

With `#line` directives active, the compiler already tells you:

```
parser.weft:84: error: 'token_type' undeclared
```

Go directly to `parser.weft:84`. The prose around the scrap will
explain what `token_type` should be and where it's defined.

### Scenario: Wrong output, no error

The program runs but produces wrong results. Open the tangled file
and find the relevant code section by searching for keywords. The
markers tell you which `.weft` file and scrap to examine:

```python
# {15: transform.weft:67}
result = [x * 2 for x in data]    # should be x * 3?
# {:15}
```

Go to `transform.weft:67`. The prose says "triple each value" — the
code says `* 2`. Bug found.

### Scenario: Identifying which fragment owns a line

Sometimes you know the line of code but not which fragment defined
it (multiple fragments with the same name accumulate). The scrap
number in the marker is unique:

```js
// {14: handlers.weft:89}
validateInput(data);
// {:14}
// {22: handlers.weft:112}
validatePermissions(user);
// {:22}
```

Scraps 14 and 22 are two different `@d` definitions (or
continuations) of the same fragment. The line numbers point to each
one independently.


## Tips

- **Don't edit tangled files.** The markers remind you these are
  generated. Always trace back to the `.weft` source and edit there.

- **Scrap numbers are stable** as long as you don't add or remove
  `@{...@}` blocks before them. If numbering shifts after edits,
  re-tangle and the markers update automatically.

- **Use `grep` on markers** to find all scraps from a specific file:
  ```bash
  grep -n '{[0-9]*: database.weft:' server.js
  ```

- **Closing markers are cheap verification.** If you see `{:7}` but
  no `{7:` above it in your viewport, scroll up — the scrap is
  larger than your screen. If you see `{7:` but no `{:7}`, the
  scrap extends below your viewport.

- **Markers in test output.** If your test framework shows code
  snippets on failure, the markers in those snippets point straight
  to the literate source.

- **Disable markers selectively.** Use explicit flags `@o file -t`
  (no comment flags) to suppress markers for a specific output file
  when the comments would interfere (e.g., binary formats, data
  files, or files where comments aren't valid).

- **Use `weft -m | jq`** to get a structural overview before diving
  into individual files. The JSON map shows the complete dependency
  graph.
