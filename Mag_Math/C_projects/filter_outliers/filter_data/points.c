#ifndef POINTS
#define POINTS
#include "points.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static point * init_point(int x, int y){
    point* point_init = malloc(sizeof(point));
    point_init->x = x;
    point_init->y = y;
    return point_init;
}

static void free_point(point * Point){
    printf("freed point %d %d \n", Point->x, Point->y);
    free(Point);
    return;
}

static DynamicArray * createArray(int size) {
    DynamicArray *ptr_Array = malloc(sizeof(DynamicArray));
    if (ptr_Array == NULL) {
        return NULL;  // Handle allocation failure
    }
    ptr_Array->data = calloc(size, sizeof(point));
    ptr_Array->size = size;
    if (ptr_Array->data == NULL) {
        // Handle allocation failure
        ptr_Array->size = 0;  // Indicate failure
        return ptr_Array;
    }
    return ptr_Array;
}

static DynamicArray * clearArray(DynamicArray* arr){
    memset(arr->data, 0, arr->size*sizeof(point));
}

static void freeArray(DynamicArray* arr){
     if (arr == NULL) return; 
    free(arr->data);  
    arr->data = NULL; 
    arr->size = 0;   
}

#endif