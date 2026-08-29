
#line 26 "literate/assembly.weft"
/* {11: literate/assembly.weft:26} */
/* Ledger: implementation */

#line 37 "literate/concepts/ledger.weft"
/* {5: literate/concepts/ledger.weft:37} */
#include "ledger.h"
#include "accounts.h"

int ledger_post(const Entry *entries, int n) {
    long sum = 0;
    for (int i = 0; i < n; i++) sum += entries[i].amount;
    if (sum != 0) return -1;                  /* the invariant: money is conserved */

    for (int i = 0; i < n; i++)
        if (!acc_exists(entries[i].account)) return -1;   /* validate before mutate */

    for (int i = 0; i < n; i++)
        acc_apply(entries[i].account, entries[i].amount); /* now safe: all valid */
    return 0;
}
/* {:5} */

#line 26 "literate/assembly.weft"

/* {:11} */
