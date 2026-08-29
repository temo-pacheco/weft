
#line 22 "literate/assembly.weft"
/* {10: literate/assembly.weft:22} */
/* Ledger: header */

#line 15 "literate/concepts/ledger.weft"
/* {4: literate/concepts/ledger.weft:15} */
#ifndef LEDGER_LEDGER_H
#define LEDGER_LEDGER_H

typedef struct { int account; long amount; } Entry; /* one posting, cents */

/* Post a balanced transaction. Returns 0 on success; -1 if the entries do
   not sum to zero (the invariant) or name an account that does not exist.
   On failure NO account is modified --- the post is atomic. */
int ledger_post(const Entry *entries, int n);

#endif
/* {:4} */

#line 22 "literate/assembly.weft"

/* {:10} */
