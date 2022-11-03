//
// Created by Nita Horia on 03.11.2022.
//

#include <stdlib.h>
#include "list.h"

list_t *createList(destructor destructor) {
    list_t *list = malloc(sizeof(list_t));
    if (list == NULL) {
        exit(-1);
    }
    list->elements = malloc(sizeof(void *) * DEFAULT_CAPACITY);
    if (list->elements == NULL) {
        exit(-1);
    }
    list->capacity = DEFAULT_CAPACITY;
    list->count = 0;
    list->destructor = destructor;
    pthread_rwlock_init(&list->lock, NULL);
    return list;
}

void freeList(list_t **pList) {
    pthread_rwlock_destroy(&(*pList)->lock);
    for (int i = 0; i < (*pList)->count; i++) {
        (*pList)->destructor(&(*pList)->elements[i]);
        (*pList)->elements[i] = NULL;
    }
    free(*pList);
    *pList = NULL;
}

void addToList(list_t *list, void *element) {
    if (pthread_rwlock_wrlock(&list->lock))
        exit(-1);
    if (list->count == list->capacity - 1) {
        list->capacity *= 2;
        void **copiedElements = realloc(list->elements, sizeof(void *) * (list->capacity));
        if (copiedElements == NULL) {
            exit(-1);
        }
        list->elements = copiedElements;
    }
    list->elements[list->count] = element;
    list->count += 1;
    if (pthread_rwlock_unlock(&list->lock))
        exit(-1);
}

void *getFromList(list_t *list, int index) {
    if (pthread_rwlock_rdlock(&list->lock))
        exit(-1);
    void* element = list->elements[index];
    if (pthread_rwlock_unlock(&list->lock))
        exit(-1);
    return element;
}