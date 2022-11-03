//
// Created by Nita Horia on 03.11.2022.
//

#ifndef A1_ACCOUNT_H
#define A1_ACCOUNT_H

#include <pthread.h>
#include <stdlib.h>
#include "list.h"

#define BASE_BALANCE 500

typedef struct {
    int id;
    int balance;
    pthread_rwlock_t lock;
    list_t* transfers; // operations
} account_t;

account_t* createAccount(int id);
void freeAccount(account_t** account);

#endif //A1_ACCOUNT_H
