#include <stdio.h>
#include <stdbool.h>

typedef struct {
    float x;
    float y;
} Point;

float* createMatrixArray(Point *points);
void freeMatrixArray(float *matrix);
bool issymmetric(const float A[], const size_t row, const size_t column);
Point* createPointsArray(const char *filename, int *numPoints);
void freeMatrixArray(float *matrix);
void rowToColumnMajor(const float* rowMajorMatrix, float* colMajorMatrix, size_t rows, size_t cols);
void columnToRowMajor(const float* colMajor, float* rowMajor, size_t rows, size_t cols);

