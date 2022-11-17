//
// Created by Nita Horia on 17.11.2022.
//

#ifndef A2_QUEUE_H
#define A2_QUEUE_H

#include <pthread.h>

#define DEFAULT_SIZE 5

typedef struct {
    int size;
    int *elements;
    pthread_mutex_t mutex;
} queue_t;

queue_t* initializeQueue();
void freeQueue(queue_t *queue);
void addToQueue(queue_t *queue, int element);
int popFromQueue(queue_t *queue);
int queueHasElements(queue_t *queue);

#endif //A2_QUEUE_H
