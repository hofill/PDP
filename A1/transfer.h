//
// Created by Nita Horia on 03.11.2022.
//

#ifndef A1_TRANSFER_H
#define A1_TRANSFER_H

#include "account.h"

typedef enum {
    TRANSFER_FROM,
    TRANSFER_TO
} TRANSFER_TYPE;

typedef struct {
    int id;
    int amount;
    account_t *otherAccount;
    TRANSFER_TYPE type;
    time_t time;
} transfer_t;

transfer_t *createTransfer(account_t* other, TRANSFER_TYPE transferType, int amount, int id);

void freeTransfer(transfer_t **pTransfer);

#endif //A1_TRANSFER_H
