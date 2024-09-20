#include <stdio.h>
#include <stdlib.h>
#include "customheaders.h"
//#define CREATE_MATRIX_TEST

/*creates in row-major form*/

float* createMatrixArray(Point *points) {
    // Calculate the number of points by iterating until the sentinel value is found
    int numPoints = 0;
    while (points[numPoints].x != 0 || points[numPoints].y != 0) {
        numPoints++;
    }

    // Allocate memory for the matrix array in row-major order (numPoints * 6)
    float *matrix = malloc(numPoints * 6 * sizeof(float));
    if (!matrix) {
        fprintf(stderr, "Memory allocation failed for matrix.\n");
        return NULL;
    }

    // Fill the matrix array
    for (int i = 0; i < numPoints; i++) {
        float x = points[i].x;
        float y = points[i].y;

        matrix[i * 6 + 0] = x * x;   // point.x^2
        matrix[i * 6 + 1] = x * y;   // point.x * point.y
        matrix[i * 6 + 2] = y * y;   // point.y^2
        matrix[i * 6 + 3] = x;       // point.x
        matrix[i * 6 + 4] = y;       // point.y
        matrix[i * 6 + 5] = 1.0;     // constant 1
    }

    return matrix;
}

void freeMatrixArray(float *matrix) {
    free(matrix);
}

#ifdef CREATE_MATRIX_TEST

int main() {
    // Example usage of the createMatrixArray function

    // Sample data with a sentinel value at the end
    Point points[] = {
        {1.0, 2.0},
        {3.0, 4.0},
        {5.0, 6.0},
        {0.0, 0.0}  // Sentinel value to indicate end of data
    };

    // Create the matrix array
    float *matrix = createMatrixArray(points);

    // Calculate the number of points (exclude the sentinel)
    int numPoints = 0;
    while (points[numPoints].x != 0 || points[numPoints].y != 0) {
        numPoints++;
    }

    // Print the matrix (optional)
    if (matrix) {
        for (int i = 0; i < numPoints * 6 ; i++) {
            printf("%f, ", matrix[i]);
        }
        // Free the matrix array
        freeMatrixArray(matrix);
    }

    return 0;
}
#endif /*CREATE_MATRIX_TEST*/