#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "account.h"
#include "transfer.h"

#define AMOUNT_ACCOUNTS 10
#define AMOUNT_THREADS 10

pthread_mutex_t gAuditMutex;
short gAuditFinished = 0;

void lockTwo(pthread_rwlock_t *first, pthread_rwlock_t *second) {
    if (pthread_rwlock_wrlock(first)) exit(-1);
    if (pthread_rwlock_wrlock(second)) exit(-1);
}

void unlockTwo(pthread_rwlock_t *first, pthread_rwlock_t *second) {
    if (pthread_rwlock_unlock(first)) exit(-1);
    if (pthread_rwlock_unlock(second)) exit(-1);
}

void *solve(void *arg) {
    account_t **accounts = (account_t **) arg;
    for (int i = 0; i < 3; i++) {
        int firstNumber = rand() % AMOUNT_ACCOUNTS, secondNumber = 0;
        while (firstNumber == secondNumber) {
            secondNumber = rand() % AMOUNT_ACCOUNTS;
        }
        account_t *firstAccount = accounts[firstNumber];
        account_t *secondAccount = accounts[secondNumber];
        int amount = rand() % 10;
        if (firstNumber < secondNumber) lockTwo(&firstAccount->lock, &secondAccount->lock);
        else lockTwo(&secondAccount->lock, &firstAccount->lock);
        firstAccount->balance -= amount;
        secondAccount->balance += amount;
        transfer_t *firstTransfer = createTransfer(secondAccount, TRANSFER_TO, amount, -1);
        transfer_t *secondTransfer = createTransfer(firstAccount, TRANSFER_FROM, amount, firstTransfer->id);
        addToList(firstAccount->transfers, firstTransfer);
        addToList(secondAccount->transfers, secondTransfer);
        if (firstNumber < secondNumber) unlockTwo(&secondAccount->lock, &firstAccount->lock);
        else unlockTwo(&firstAccount->lock, &secondAccount->lock);
    }
    return NULL;
}

void accountChecker(account_t** accounts) {
    for (int i = 0; i < AMOUNT_ACCOUNTS; i++) {
        account_t *account = accounts[i];
        if (pthread_rwlock_rdlock(&account->lock)) exit(-1);
        int balance = BASE_BALANCE;
        for (int j = 0; j < account->transfers->count; j++) {
            transfer_t* transfer = account->transfers->elements[j];
            if (transfer->type == TRANSFER_FROM) {
                balance += transfer->amount;
            } else {
                balance -= transfer->amount;
            }
        }
        printf("acc = %d, bal = %d, acc_bal = %d\n", account->id, balance, account->balance);
        if (balance != account->balance) {
            printf("balance wrong for account %d", account->id);
            exit(-1);
        }
        if (pthread_rwlock_unlock(&account->lock)) exit(-1);
    }
}

void *checker(void *arg) {
    account_t **accounts = (account_t **) arg;
    for (;;) {
        if (pthread_mutex_lock(&gAuditMutex)) exit(-1);
        if (gAuditFinished) {
            if (pthread_mutex_unlock(&gAuditMutex)) exit(-1);
            break;
        }
        if (pthread_mutex_unlock(&gAuditMutex)) exit(-1);
        accountChecker(accounts);
    }
    return NULL;
}

int main() {
    srand((unsigned) time(NULL));
    account_t *accounts[AMOUNT_ACCOUNTS] = {0};
    for (int i = 0; i < AMOUNT_ACCOUNTS; i++)
        accounts[i] = createAccount(i);
    pthread_t threads[AMOUNT_THREADS] = {0};
    for (int i = 0; i < AMOUNT_THREADS; i++)
        pthread_create(&threads[i], NULL, solve, accounts);

    pthread_mutex_init(&gAuditMutex, NULL);
    pthread_t checkerThread;
    pthread_create(&checkerThread, NULL, checker, accounts);

    for (int i = 0; i < AMOUNT_THREADS; i++)
        pthread_join(threads[i], NULL);

    if (pthread_mutex_lock(&gAuditMutex)) exit(-1);
    gAuditFinished = 1;
    if (pthread_mutex_unlock(&gAuditMutex)) exit(-1);

    pthread_join(checkerThread, NULL);
    for (int i = 0; i < AMOUNT_ACCOUNTS; i++) {
        account_t *account = accounts[i];
        freeAccount(&account);
    }
    pthread_mutex_destroy(&gAuditMutex);
    printf("Done!\n");
}
