//
// Created by Nita Horia on 17.11.2022.
//

#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

queue_t *initializeQueue() {
    queue_t *queue = malloc(sizeof(queue_t));
    queue->size = 0;
    queue->elements = malloc(sizeof(int) * DEFAULT_SIZE);
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);
    return queue;
}

int popFromQueue(queue_t *queue) {
    pthread_mutex_lock(&queue->mutex);
    if (queue->size == 0) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }
    int firstElement = queue->elements[0];
    for (int i = 1; i <= queue->size; i++) {
        queue->elements[i - 1] = queue->elements[i];
    }
    queue->size -= 1;
    pthread_mutex_unlock(&queue->mutex);
    return firstElement;
}

void freeQueue(queue_t *queue) {
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
    free(queue->elements);
    free(queue);
}

void addToQueue(queue_t *queue, int element) {
    pthread_mutex_lock(&queue->mutex);
    queue->elements[queue->size] = element;
    queue->size += 1;
    pthread_mutex_unlock(&queue->mutex);
    pthread_cond_signal(&queue->cond);
}
