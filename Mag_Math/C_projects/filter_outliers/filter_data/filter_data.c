#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "filter_data.h"
#include "create_array.c"
//#include <convert_to_arrays_C>

static int pointDistance(point* point1, point* point2){
    int x1 = point1->x;
    int y1 = point1->y;
    int x2 = point2->x;
    int y2 = point2->y;
    return (int) fabs(sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
}

static void filter_array(DynamicArray* input_array) {
    if (input_array == NULL || input_array->size == 0) return;

    int clearCount = 0;
    bool isLocked = false;
    point* curr;
    point* prev = NULL;
    point* curr_holder = NULL;

    
    for (int i = 0; i < input_array->size; i++) {

        curr = &input_array->data[i];

        free(curr_holder);
        curr_holder = init_point(curr->x, curr->y);

        printf("looped in %d\n", i);
        if (prev != NULL){
            printf("prev: %d, %d, curr: %d, %d\n", prev->x, prev->y, curr->x, curr->y);
            printf("point Distance = %d \n", pointDistance(curr, prev));
            } 
                  
        if (isLocked) {
            if (prev != NULL && pointDistance(curr, prev) > ACCEPTABLE_GIVE) {
                isLocked = false; // Unlock when a point outside the acceptable range is found
                curr->x = 0;
                curr->y = 0;
            }
            // else keep locking as we are within the range
        } else {
            if (prev != NULL && pointDistance(curr, prev) <= ACCEPTABLE_GIVE) {
                //printf("clearCount++\n");
                clearCount++;
                if (clearCount >= RECALC_POINT_COUNT - 1) { // We need one less than RECALC_POINT_COUNT because the count starts from the second point
                    isLocked = true; // Lock on once we find enough points in a row
                    clearCount = 0; // Reset clearCount
                } else {
                    curr->x = 0;
                    curr->y = 0;
                }
            } else {
                clearCount = 0; // Reset clearCount if the sequence is broken
                curr->x = 0;
                curr->y = 0;
            }
        }
        printf("breakpoint\n");
        free(prev);
        prev = init_point(curr_holder->x, curr_holder->y);

        printf("Updated Prev to: %d, %d (should match curr_holder: %d, %d)\n", prev->x, prev->y, curr_holder->x, curr_holder->y);
    }
}



int main(){
   /* 
    point* point1 = init_point(3,-249);
    point* point2 = init_point(1,-244);
    printf("%d\n", pointDistance(point1, point2));
    */


    const char* filename = "trial1output.csv";
    DynamicArray* points = loadPointsFromFile(filename);
    if (points) {
        filter_array(points);
    }
    for (int i = 0; i < points->size; i++) {
            printf("%d, %d\n", points->data[i].x, points->data[i].y);
        }
    freeArray(points);
    



   return 0;
}


