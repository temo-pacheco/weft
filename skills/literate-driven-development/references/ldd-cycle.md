# One unit of work, walked through

Phase 3 is *write the chapter, review, advance*. Inside a chapter the work
moves in small units — one substage, one concept, often one chunk. This is one
such unit walked end to end: adding rate-limiting to an existing `auth`
concept. The point is to show **where the tokens go** and **where the checks
fire**.

The five movements: **narrate → orient → fill → verify → reconcile**, then the
review and the commit.


## Narrate (human)

Open `literate/concepts/auth.weft` and write prose first, naming the chunk
before implementing it:

```
Every login attempt must be throttled: no more than five failures per IP in
a rolling minute, after which we return 429 with a Retry-After header. The
counters live in the same Redis we already use for sessions, so the limiter
shares a connection with @<Auth: session store@>.

@d Auth: rate limit check @{
    @%  TODO — fill me
@}
```

The prose is the spec. The chunk name is the task. Nothing is implemented yet,
and nothing needs to be: the next author — human or AI — now knows the rule,
the reason, and where the connection comes from.


## Orient (AI — cheap)

The AI does **not** read `auth.weft` top to bottom. It queries:

```bash
weft -m --prose auth.weft                       # structure + the prose
weft -e "Auth: session store" --callers auth.weft
```

That returns the exact Redis-connection fragment plus who calls it — a few
hundred tokens instead of the whole file. Now the AI knows the handle to reuse
and who else depends on it.

(`weft -m` emits JSON; an AI reads it directly. A human at a shell may prefer
to pipe it through `jq`, but nothing in LDD requires it.)


## Fill (AI)

Fill the body, **referencing** the existing chunk instead of re-deriving a
connection:

```
@d Auth: rate limit check @{
    key := "rl:" + ip
    n, _ := @<Auth: session store@>.Incr(ctx, key).Result()
    if n == 1 {
        @<Auth: session store@>.Expire(ctx, key, time.Minute)
    }
    if n > 5 {
        return tooManyRequests(retryAfter(ctx, key))
    }
@}
```

Only the `.weft` was edited. Stable code (`session store`) was reused **by
name**, not reloaded — it is a named black box, and that is what keeps the cost
of this change proportional to the change and not to the auth system.

The test comes from the same narrative, in the same file, next to what it
proves:

```
The rule is worth a test that states it in the same words the prose does:
five failures pass, the sixth is refused.

@d Auth: tests — rate limit @{
    for i := 0; i < 5; i++ { … }   @% five failures: allowed
    …                              @% the sixth: 429
@}
```


## Verify (mechanical)

```bash
weft project.weft && make        # tangle, then build
make 2>&1 | weft --errors
```

If the compiler says `auth.go:214: undefined: retryAfter`, `--errors`
annotates it as `literate/concepts/auth.weft:NN, scrap 204`. The AI fixes the
source at that exact line — no guessing which generated file, and no
temptation to patch the generated one.


## Reconcile (AI)

```bash
weft --lint auth.weft     # did we reference a real chunk? orphans?
weft --diff auth.weft     # is the tangled output in sync?
```

`--lint` would have caught it instantly if the AI had typed
`@<Auth: sesion store@>` — it reports the undefined ref and fuzzy-suggests
`Auth: session store`. That is the anti-hallucination check firing on the
cheapest possible command.

Then re-read the prose. We wrote *five failures per minute*; the code says
`n > 5` with a one-minute expiry — coherent, and the prose stays. Had the
threshold changed, the paragraph changes **in the same edit**. That is the
Prime Directive, and the AI says out loud which it was.


## Review, then commit

The full checklist is in [ldd-review.md](ldd-review.md). For a unit this size
it is quick: lint clean, diff empty, build green, prose true, and the section
still reads as one story — including the new paragraph, which should not read
like a patch sewn onto the old one.

```
git commit -m "auth: throttle login failures (5/min/IP) via shared Redis"
```

One concept, one story, one commit. The tangled `auth.go` and the woven
chapter both update from the same source.


## Where the effort went

| Movement | Tokens | Files opened |
|---|---|---|
| Narrate | human | one `.weft`, one paragraph |
| Orient | low | none — queried the map |
| Fill | low | one chunk in one `.weft` |
| Verify | mechanical | none — `--errors` mapped it |
| Reconcile | low | the prose around one chunk |

The cost tracked the size of the *change*, not the size of the auth system.
That is the property that makes LDD viable on a book that keeps growing: you
always work at the frontier, on top of what is already proven.
