#include <stdbool.h>
#ifndef FILTER_DATA
#define FILTER_DATA

typedef struct point {
    int x;
    int y;
} point;

typedef struct {
    point* data;
    int size;
} DynamicArray;

#endif


