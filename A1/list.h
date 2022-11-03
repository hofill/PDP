//
// Created by Nita Horia on 03.11.2022.
//

#ifndef A1_LIST_H
#define A1_LIST_H

#include <pthread.h>

#define DEFAULT_CAPACITY 5

typedef void (*destructor)(void**);

typedef struct {
    void** elements;
    int count;
    int capacity;
    destructor destructor;
    pthread_rwlock_t lock;
} list_t;

list_t* createList(destructor destructor);
void freeList(list_t** pList);

void addToList(list_t* list, void* element);
void* getFromList(list_t* list, int index);

#endif //A1_LIST_H
