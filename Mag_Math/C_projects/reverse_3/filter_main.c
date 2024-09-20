
#ifdef FILTER_MAIN

#include <stdio.h>
#include <stdlib.h>
#include "inc/filter.h"


#include <stdio.h>
#include <stdlib.h>
#include "filter.h"

#define BUFF_MAX 9  // Setting BUFF_MAX to 9

// Function to print all points currently in the queue
void print_queue(buffer_linked_list* queue) {
    printf("Current points in the queue:\n");
    point* curr = queue->start;
    while (curr != NULL) {
        printf("(%d, %d) ", curr->x, curr->y);
        curr = curr->next;
    }
    printf("\n");
}

int main() {
    // Initialize the buffer-linked list (deque)
    buffer_linked_list *queue = init_buffer_LL();

    // Test points to be added (from (12, 12) to (-10, -10))
    point points_to_add[] = {
    {100, 100}, {-50, -50}, {-1, 4}, {-10, -2}, {-6, 8}, {7, 8}, {-4, 6}, {-100, -100}, {0, 3}, {10, 10}, {-5, -7}, {-4, 1}, {2, 8}, {6, 8}, {500,500},
    {-8, -6}, {0, -7}, {-6, -7}, {100, 100}, {8, 9}, {3, 7}, {-100, -100}, {3, -4}, {-8, -4}, {-9, 3}, {-5, -9}, {-100, -100}, {100, 100}
    };

    // Enqueue points (simulating non-ascending insertion) and print each point as you queue it
    for (int i = 0; i < 23; i++) {
        point *new_point = init_point(points_to_add[i].x, points_to_add[i].y);
        printf("Queued point: (%d, %d)\n", new_point->x, new_point->y);

        // Print all points in the queue once it's full (after BUFF_MAX points are added)
        print_queue(queue);
        queue->add(queue, new_point);
    }

    // Display midpoint after all points are added
    if (queue->midpt) {
        printf("Midpoint after adding all points: (%d, %d)\n", queue->midpt->x, queue->midpt->y);
        point * avg = find_avg(queue);
        printf("Average after all queued: (%d, %d)\n", avg->x, avg->y);
        free_point(avg);
    }

    // Dequeue points one by one, and display midpoint after each dequeue
    for (int i = 0; i < 9; i++) {
        printf("Dequeuing a point...\n");
        queue->push(queue);
    }

    // Print the buffer max at the end
    printf("Buffer Max: %d\n", BUFF_MAX);

    // Free the buffer-linked list
    free_buffer_LL(queue);

    return 0;
}

#endif