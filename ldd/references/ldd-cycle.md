# LDD cycle — a worked walk-through

One unit of work — adding rate-limiting to an existing `auth` concept —
walked through all six steps. The point is to show *where the tokens go* and
*where the gates fire*.

## 1. NARRATE (human)

The human opens `literate/concepts/auth.weft` and writes prose first, naming
a chunk before implementing it:

```
Every login attempt must be throttled: no more than five failures per IP in
a rolling minute, after which we return 429 and a Retry-After header. We keep
the counters in the same Redis we already use for sessions, so the limiter
shares a connection with @<Auth: session store@>.

@d Auth: rate limit check @{
    @%  TODO — fill me
@}
```

The prose is the spec. The chunk name is the task. Nothing is implemented
yet.

## 2. ORIENT (AI — cheap)

The AI does **not** read `auth.weft` top to bottom. It queries:

```bash
weft -m --prose auth.weft | jq '.fragments["Auth: session store"]'
weft -e "Auth: session store" --callers auth.weft
```

That returns the exact Redis-connection fragment plus who calls it — a few
hundred tokens, not the whole file. Now the AI knows the connection handle to
reuse.

## 3. FILL (AI)

The AI fills the body, **referencing** the existing chunk instead of
re-deriving a connection:

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

Only `.weft` was edited. Stable code (`session store`) was reused by name,
not reloaded.

## 4. VERIFY (mechanical)

```bash
weft project.weft && make        # tangle, then build
make 2>&1 | weft --errors
```

If Go complains `auth.go:ramp: undefined: retryAfter`, `--errors` annotates
it as `literate/concepts/auth.weft:NN, scrap …`. The AI fixes the source at
that exact line — no guessing which generated file.

## 5. RECONCILE (AI)

```bash
weft --lint auth.weft     # did we reference a real chunk? orphans?
weft --diff auth.weft     # is the tangled output in sync?
```

`--lint` would have caught it instantly if the AI had typed
`@<Auth: sesion store@>` (misspelled) — it reports the undefined ref and
fuzzy-suggests `Auth: session store`. That is the anti-hallucination gate
firing.

Then the AI re-reads the prose: does it still describe the code? We wrote
"five failures per minute" and the code says `n > 5` with a one-minute
expiry — coherent. If the threshold had changed, the prose changes in the
same edit.

## 6. COMMIT

```
git commit -m "auth: throttle login failures (5/min/IP) via shared Redis"
```

One concept, one story, one commit. The tangled `auth.go` and the woven
chapter both update from the same source.

---

## Where the effort went

| Step | Tokens | Files opened |
|---|---|---|
| Orient | low | none — queried the map |
| Fill | low | one chunk in one `.weft` |
| Verify | mechanical | none — `--errors` mapped it |
| Reconcile | low | prose around one chunk |

The cost tracked the size of the *change*, not the size of the auth system.
