#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "queue.h"

int8_t finishedProducer = 0;
queue_t *queue;

// vector struct (3d)
typedef struct {
    int i, j, k;
} vector_t;

typedef struct {
    int length;
    int *finalSum;
} add_params_t;

void *addNumbers(void *args) {
    int *finalSum = ((add_params_t *) args)->finalSum;
    int length = ((add_params_t *) args)->length;
    for (int i = 0; i < length; i++) {
        int lastProduct = popFromQueue(queue);
        printf("adding last product %d\n", lastProduct);
        *finalSum += lastProduct;
    }
    return NULL;
}

void *multiplyNumbers(void *args) {
    vector_t *vectors = (vector_t *) args;
    for (int i = 0; i < 3; i++) {
        int product = 0;
        if (i == 0) {
            product = vectors[0].i * vectors[1].i;
        } else if (i == 1) {
            product = vectors[0].j * vectors[1].j;
        } else if (i == 2) {
            product = vectors[0].k * vectors[1].k;
        }
        printf("adding to queue %d\n", product);
        addToQueue(queue, product);
    }
    finishedProducer = 1;
    return NULL;
}

int main() {
    queue = initializeQueue();
    vector_t firstVector = {
            .i = 1,
            .j = 2,
            .k = 0
    };
    vector_t secondVector = {
            .i = -1,
            .j = 5,
            .k = 3
    };
    vector_t vectors[] = {firstVector, secondVector};

    pthread_t addNumbersThread, multiplyNumbersThread;
    int result = 0;

    add_params_t addParams = {
            .length = 3,
            .finalSum = &result
    };

    pthread_create(&multiplyNumbersThread, NULL, multiplyNumbers, &vectors);
    pthread_create(&addNumbersThread, NULL, addNumbers, &addParams);
    pthread_join(multiplyNumbersThread, NULL);
    pthread_join(addNumbersThread, NULL);

    freeQueue(queue);

    printf("Result: %d\n", result);
    return 0;
}
