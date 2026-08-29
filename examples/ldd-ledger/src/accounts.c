
#line 16 "literate/assembly.weft"
/* {9: literate/assembly.weft:16} */
/* Accounts: implementation */

#line 39 "literate/concepts/accounts.weft"
/* {2: literate/concepts/accounts.weft:39} */
#include "accounts.h"
#include <string.h>

static struct { char name[32]; long balance; int used; } accounts[ACC_MAX];

int acc_open(const char *name) {
    for (int i = 0; i < ACC_MAX; i++) {
        if (!accounts[i].used) {
            accounts[i].used = 1;
            accounts[i].balance = 0;
            strncpy(accounts[i].name, name, sizeof accounts[i].name - 1);
            accounts[i].name[sizeof accounts[i].name - 1] = '\0';
            return i;
        }
    }
    return -1;
}

int acc_exists(int id) {
    return id >= 0 && id < ACC_MAX && accounts[id].used;
}

long acc_balance(int id) {
    return acc_exists(id) ? accounts[id].balance : 0;
}

int acc_apply(int id, long delta) {
    if (!acc_exists(id)) return -1;
    accounts[id].balance += delta;
    return 0;
}

const char *acc_name(int id) {
    return acc_exists(id) ? accounts[id].name : "?";
}
/* {:2} */

#line 16 "literate/assembly.weft"

/* {:9} */
