#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "inc/queue.h"


int main() {
    Queue myQueue;
    Prep_Queue(&myQueue);  // Initialize the queue

    // Enqueue 5 objects (in this case, integers)
    int *data1 = malloc(sizeof(int));
    *data1 = 10;
    Enqueue(&myQueue, data1);

    int *data2 = malloc(sizeof(int));
    *data2 = 20;
    Enqueue(&myQueue, data2);

    int *data3 = malloc(sizeof(int));
    *data3 = 30;
    Enqueue(&myQueue, data3);

    int *data4 = malloc(sizeof(int));
    *data4 = 40;
    Enqueue(&myQueue, data4);

    int *data5 = malloc(sizeof(int));
    *data5 = 50;
    Enqueue(&myQueue, data5);

    // Dequeue and print 5 objects
    for (int i = 0; i < 5; i++) {
        int *deqData = (int *)Dequeue(&myQueue);
        if (deqData != NULL) {
            printf("Dequeued: %d\n", *deqData);
            free(deqData);  // Free the dequeued data
        } else {
            printf("Queue is empty!\n");
        }
    }

    return 0;
}