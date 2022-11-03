//
// Created by Nita Horia on 03.11.2022.
//

#include "transfer.h"

pthread_mutex_t gIdMutex = PTHREAD_MUTEX_INITIALIZER;
int gId = 0;

transfer_t *createTransfer(account_t* other, TRANSFER_TYPE transferType, int amount, int id) {
    transfer_t* transfer = malloc(sizeof(transfer_t));
    if (transfer == NULL)
        exit(-1);
    transfer->otherAccount = other;
    transfer->type = transferType;
    transfer->amount = amount;
    transfer->time = time(NULL);
    if (id == -1) {
        if (pthread_mutex_lock(&gIdMutex))
            exit(-1);
        transfer->id = gId + 1;
        gId += 1;
        if (pthread_mutex_unlock(&gIdMutex))
            exit(-1);
    }
    else {
        transfer->id = id;
    }
    return transfer;
}


void freeTransfer(transfer_t** pTransfer) {
    free(*pTransfer);
    *pTransfer = NULL;
}
