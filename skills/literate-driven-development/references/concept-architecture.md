
# Architecture: Organization by Concept

## Concept-Oriented vs Layer-Oriented

**Layer-oriented** (traditional):
```
src/
├── models/
│   ├── user.go
│   └── payment.go
├── services/
│   ├── auth_service.go
│   └── payment_service.go
├── handlers/
│   ├── auth_handler.go
│   └── payment_handler.go
└── tests/
    ├── auth_test.go
    └── payment_test.go
```

To understand "authentication," you read 3+ files across 3+ directories.

**Concept-oriented** (literate):
```
literate/concepts/
├── auth.weft       # model + service + handler + test for auth
├── payments.weft   # model + service + handler + test for payments
└── ...
```

To understand "authentication," you read one file front-to-back.

The tangled output still produces the layer-oriented structure that tools
expect. You get concept-oriented source for understanding and
layer-oriented output for building.



## The Three-File Pattern

For code projects, a common organization:

1. **Master file** (`project.weft`) — LaTeX/Markdown structure, `@i` includes, indices
2. **Concept files** (`literate/concepts/*.weft`) — `@d` chunks only
3. **Assembly file** (`literate/assembly.weft`) — all `@o` directives

This separation has a key property: **concept files never specify output
paths**. They define named chunks like `@d Auth: user model` that describe
*what* exists. The assembly file decides *where* each chunk goes:

```
@o tmp/src/models/user.go @{
@<Auth: user model@>
@}
```

This means you can:
- Reorganize the output file structure without touching concept files
- Reuse chunks in multiple output files
- Keep concept files focused on narrative and logic



## Complete Example

Consider a "notifications" concept:

**`literate/concepts/notifications.weft`** defines:
```
@d Notifications: entity       → the Notification struct
@d Notifications: repository   → database queries
@d Notifications: use case     → send/read/dismiss logic
@d Notifications: handler      → API endpoints
@d Notifications: test         → test cases
```

**`literate/assembly.weft`** assembles:
```
@o tmp/src/entity/notification.go @{
@<Notifications: entity@>
@}

@o tmp/src/repository/notification_repo.go @{
@<Notifications: repository@>
@}

@o tmp/src/usecase/notification_usecase.go @{
@<Notifications: use case@>
@}

@o tmp/src/handler/notification_handler.go @{
@<Notifications: handler@>
@}

@o tmp/src/test/notification_test.go @{
@<Notifications: test@>
@}
```

The concept file tells the story: what notifications are, how they work,
and how they're tested. The assembly file handles the plumbing.



## When to Split Concepts

**Create a new `.weft` file when:**
- The concept is large enough to tell its own story (typically 100+ lines
  of combined code)
- The concept has its own domain model, even a small one
- Multiple people might work on this concept independently
- The concept has distinct business rules worth narrating

**Keep in a shared file when:**
- It's a small utility (helper function, constant)
- It's pure infrastructure with no business logic
- It would result in a `.weft` file shorter than the boilerplate

**Rule of thumb**: if you can write a meaningful section title for it,
it probably deserves its own `.weft` file. If you'd struggle to write more
than a paragraph of prose about it, it might be too small.
