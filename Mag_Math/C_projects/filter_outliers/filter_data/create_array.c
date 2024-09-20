#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "filter_data.h"

DynamicArray* loadPointsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    char line[256];
    float xf, zf;
    int count = 0;

    // Skip the header line
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return NULL;
    }

    // First pass: count valid points
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%f,%*f,%f", &xf, &zf) == 2) {
            count++;
        }
    }

    // Rewind the file to read the points again
    rewind(file);

    // Skip the header line again
    fgets(line, sizeof(line), file);

    DynamicArray* arr = createArray(count);
    if (!arr) {
        fclose(file);
        return NULL;
    }

    int index = 0;
    // Second pass: read the points
    while (fgets(line, sizeof(line), file) && index < count) {
        if (sscanf(line, "%f,%*f,%f", &xf, &zf) == 2) {
            arr->data[index].x = (int)xf;
            arr->data[index].y = (int)zf;
            index++;
        }
    }

    fclose(file);
    return arr;
}


/**
int main() {
    const char* filename = "trial1output.csv";
    DynamicArray* points = loadPointsFromFile(filename);
    if (points) {
        for (int i = 0; i < points->size; i++) {
            printf("%d, %d\n", points->data[i].x, points->data[i].y);
        }
        freeArray(points);
    } else {
        printf("Failed to load points.\n");
    }

    return 0;
}
*/