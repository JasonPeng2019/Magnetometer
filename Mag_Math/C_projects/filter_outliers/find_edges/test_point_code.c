#include <stdio.h>
#include <stdlib.h>

// Define the point structure
typedef struct point {
    int x;
    int y;
    struct point *next;
    struct point *prev;
} point;

// Function to initialize a point
point *init_point(int x, int y) {
    point* point_init = (point*)malloc(sizeof(point));
    if (point_init == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    point_init->x = x;
    point_init->y = y;
    point_init->next = NULL;
    point_init->prev = NULL;
    printf("Initialized point at %p with x=%d, y=%d, next=%p, prev=%p\n", 
           (void*)point_init, x, y, (void*)point_init->next, (void*)point_init->prev);
    return point_init;
}

// Main function to test point initialization
int main() {
    point *new_pt_x_max = init_point(10, 20);
    printf("Point new_pt_x_max at %p with x=%d, y=%d, next=%p, prev=%p\n",
           (void*)new_pt_x_max, new_pt_x_max->x, new_pt_x_max->y, 
           (void*)new_pt_x_max->next, (void*)new_pt_x_max->prev);

    // Free allocated memory
    free(new_pt_x_max);
    return 0;
}