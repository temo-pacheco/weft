
#line 12 "literate/assembly.weft"
/* {8: literate/assembly.weft:12} */
/* Accounts: header */

#line 17 "literate/concepts/accounts.weft"
/* {1: literate/concepts/accounts.weft:17} */
#ifndef LEDGER_ACCOUNTS_H
#define LEDGER_ACCOUNTS_H

#define ACC_MAX 64              /* the store holds at most this many accounts */

int         acc_open(const char *name); /* -> id >= 0, or -1 if the store is full */
long        acc_balance(int id);        /* balance in cents, 0 for an unknown id */
int         acc_apply(int id, long delta); /* add delta cents; 0 ok, -1 bad id  */
int         acc_exists(int id);         /* 1 if id is a live account, else 0     */
const char *acc_name(int id);           /* the account's name, or "?"            */

#endif
/* {:1} */

#line 12 "literate/assembly.weft"

/* {:8} */
