
#line 32 "literate/assembly.weft"
/* {12: literate/assembly.weft:32} */
/* Api: main */

#line 18 "literate/concepts/api.weft"
/* {7: literate/concepts/api.weft:18} */
#include <stdio.h>
#include "accounts.h"
#include "ledger.h"

int main(void) {
    /* Accounts: demo accounts */
    
#line 83 "literate/concepts/accounts.weft"
    /* {3: literate/concepts/accounts.weft:83} */
int cash    = acc_open("cash");
        int revenue = acc_open("revenue");/* {:3} */

#line 23 "literate/concepts/api.weft"

    /* Ledger: demo posting */
    
#line 62 "literate/concepts/ledger.weft"
    /* {6: literate/concepts/ledger.weft:62} */
Entry txn[] = { { cash, +10000 }, { revenue, -10000 } };
        int rc = ledger_post(txn, 2);/* {:6} */

#line 24 "literate/concepts/api.weft"


    printf("posted transaction: %s\n", rc == 0 ? "OK" : "REJECTED");
    printf("  %-8s %8.2f\n", acc_name(cash),    acc_balance(cash)    / 100.0);
    printf("  %-8s %8.2f\n", acc_name(revenue), acc_balance(revenue) / 100.0);

    /* The invariant, demonstrated: an unbalanced transaction is refused. */
    Entry bad[] = { { cash, +500 } };                 /* sums to 500, not zero */
    printf("unbalanced transaction: %s\n",
           ledger_post(bad, 1) == 0 ? "ACCEPTED (bug!)" : "REJECTED (correct)");

    long total = acc_balance(cash) + acc_balance(revenue);
    printf("sum of all balances: %ld cents (must be 0)\n", total);
    return 0;
}
/* {:7} */

#line 32 "literate/assembly.weft"

/* {:12} */
