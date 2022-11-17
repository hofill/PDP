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
    return queue;
}

int popFromQueue(queue_t *queue) {
    pthread_mutex_lock(&queue->mutex);
    if (queue->size <= 0) {
        exit(1);
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
    free(queue->elements);
    free(queue);
}

int queueHasElements(queue_t *queue) {
    int queueSize = queue->size;
    return queueSize != 0;
}

void addToQueue(queue_t *queue, int element) {
    pthread_mutex_lock(&queue->mutex);
    queue->elements[queue->size] = element;
    queue->size += 1;
    pthread_mutex_unlock(&queue->mutex);
}
