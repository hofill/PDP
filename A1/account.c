//
// Created by Nita Horia on 03.11.2022.
//

#include "account.h"
#include "transfer.h"

account_t *createAccount(int id) {
    account_t *account = malloc(sizeof(account_t));
    if (account == NULL)
        exit(-1);
    account->id = id;
    account->balance = BASE_BALANCE;
    if(pthread_rwlock_init(&account->lock, NULL) != 0)
        exit(-1);
    account->transfers = createList((destructor)freeTransfer);
    return account;
}

void freeAccount(account_t** account) {
    freeList(&(*account)->transfers);
    if(pthread_rwlock_destroy(&(*account)->lock))
        exit(-1);
    free(*account);
    *account = NULL;
}