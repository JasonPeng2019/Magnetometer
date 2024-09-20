#include <stdio.h>
#include "filter.h"
#define MATRIX_SIZE 6

typedef struct {
    double m[2][2];
} Matrix2x2;

typedef struct {
    double x;
    double y;
} Vector2;

void swapRows(double M[MATRIX_SIZE][MATRIX_SIZE], double B[MATRIX_SIZE], int row1, int row2);
void swapColumns(double M[MATRIX_SIZE][MATRIX_SIZE], int col1, int col2);
void findTransformation(point p[3], point q[3], Matrix2x2 *A, Vector2 *T);
int findBestPivot(double M[MATRIX_SIZE][MATRIX_SIZE], int size, int startRow, int startCol, int *bestRow, int *bestCol);
void findInverse(Matrix2x2 *A, Matrix2x2 *A_inv);
void findTransformationAndInverse(point p[3], point q[3], Matrix2x2 *A, Vector2 *T, Matrix2x2 *A_inv);
point* readCSVAndStorePoints(const char* filename, int* validPointCount);
point* reverseTransformPoints(point* points, int pointCount, Matrix2x2* A_inv, Vector2* T);
void writePointsToCSV(const char* filename, point* points, int pointCount);
