#ifdef USE_EASY_2

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "filter.h"





// Define structures for Matrix and Vector
typedef struct {
    double m[2][2];
} Matrix2x2;

typedef struct {
    double x;
    double y;
} Vector2;

typedef struct {
    double x;
    double y;
} point;

// Function prototypes
void swapRows(double M[MATRIX_SIZE][MATRIX_SIZE], double B[MATRIX_SIZE], int row1, int row2);
void swapColumns(double M[MATRIX_SIZE][MATRIX_SIZE], int col1, int col2);
void findTransformation(point p[3], point q[3], Matrix2x2 *A, Vector2 *T);
int findBestPivot(double M[MATRIX_SIZE][MATRIX_SIZE], int size, int startRow, int startCol, int *bestRow, int *bestCol);
void findInverse(Matrix2x2 *A, Matrix2x2 *A_inv);
void findTransformationAndInverse(point p[3], point q[3], Matrix2x2 *A, Vector2 *T, Matrix2x2 *A_inv);
point* readCSVAndStorePoints(const char* filename, int* validPointCount);
point* reverseTransformPoints(point* points, int pointCount, Matrix2x2* A_inv, Vector2* T);
void writePointsToCSV(const char* filename, point* points, int pointCount);

/*
int main() {
    // Define original points on the circle

    // Define corresponding points on the ellipse
    point p[3] = {{0, 300}, {300, 0}, {0, -300}};
    point q[3] = {{26, 575}, {-60, 830}, {135, 1225}};


    // Variables to hold the results
    Matrix2x2 A, A_inv;
    Vector2 T;

    // Find the transformation matrix A, translation vector T, and inverse matrix A_inv
    findTransformationAndInverse(p, q, &A, &T, &A_inv);

    // Print the results
    printf("Transformation Matrix A:\n");
    printf("[%f %f]\n", A.m[0][0], A.m[0][1]);
    printf("[%f %f]\n", A.m[1][0], A.m[1][1]);

    printf("Translation Vector T:\n");
    printf("[%f, %f]\n", T.x, T.y);

    printf("Inverse Transformation Matrix A_inv:\n");
    printf("[%f %f]\n", A_inv.m[0][0], A_inv.m[0][1]);
    printf("[%f %f]\n", A_inv.m[1][0], A_inv.m[1][1]);

    int validPointCount;
    point* points = readCSVAndStorePoints("calibPlate3CSV.csv", &validPointCount);

    if (points) {
        printf("Number of valid points: %d\n", validPointCount);
        point* transformedPoints = reverseTransformPoints(points, validPointCount, &A_inv, &T);
        writePointsToCSV("AAAcalib3Platetransformed.csv", transformedPoints, validPointCount);
        free(transformedPoints);
        free(points);
    }


    return 0;
}
*/

// Function to swap two rows in the matrix and vector
void swapRows(double M[MATRIX_SIZE][MATRIX_SIZE], double B[MATRIX_SIZE], int row1, int row2) {
    double temp;
    for (int j = 0; j < MATRIX_SIZE; j++) {
        temp = M[row1][j];
        M[row1][j] = M[row2][j];
        M[row2][j] = temp;
    }

    temp = B[row1];
    B[row1] = B[row2];
    B[row2] = temp;
}

// Function to swap two columns in the matrix
void swapColumns(double M[MATRIX_SIZE][MATRIX_SIZE], int col1, int col2) {
    double temp;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        temp = M[i][col1];
        M[i][col1] = M[i][col2];
        M[i][col2] = temp;
    }
}

// Function to find the best pivot element in the matrix
int findBestPivot(double M[MATRIX_SIZE][MATRIX_SIZE], int size, int startRow, int startCol, int *bestRow, int *bestCol) {
    double maxVal = 0;
    *bestRow = startRow;
    *bestCol = startCol;

    for (int i = startRow; i < size; i++) {
        for (int j = startCol; j < size; j++) {
            if (fabs(M[i][j]) > maxVal) {
                maxVal = fabs(M[i][j]);
                *bestRow = i;
                *bestCol = j;
            }
        }
    }

    return maxVal > 1e-9;  // Return 1 if a valid pivot was found, 0 otherwise
}

void findTransformation(point p[3], point q[3], Matrix2x2 *A, Vector2 *T) {
    // Set up the system of linear equations in matrix form
    double M[MATRIX_SIZE][MATRIX_SIZE] = {
        { p[0].x, p[0].y, 0, 0, 1, 0 },
        { 0, 0, p[0].x, p[0].y, 0, 1 },
        { p[1].x, p[1].y, 0, 0, 1, 0 },
        { 0, 0, p[1].x, p[1].y, 0, 1 },
        { p[2].x, p[2].y, 0, 0, 1, 0 },
        { 0, 0, p[2].x, p[2].y, 0, 1 }
    };

    // Right-hand side (known points on the ellipse):
    double B[MATRIX_SIZE] = { q[0].x, q[0].y, q[1].x, q[1].y, q[2].x, q[2].y };

    double X[MATRIX_SIZE];
    int columnOrder[MATRIX_SIZE];  // To track column swaps

    // Initialize columnOrder to the identity (no swaps yet)
    for (int i = 0; i < MATRIX_SIZE; i++) {
        columnOrder[i] = i;
    }

    // Gaussian elimination with full pivoting
    for (int i = 0; i < MATRIX_SIZE; i++) {
        int bestRow, bestCol;

        // Find the best pivot element in the remaining submatrix
        if (!findBestPivot(M, MATRIX_SIZE, i, i, &bestRow, &bestCol)) {
            printf("Matrix is singular or near singular.\n");
            return;
        }

        // Swap rows to bring the best pivot to the diagonal
        if (bestRow != i) {
            swapRows(M, B, i, bestRow);
        }

        // Swap columns and track column swaps
        if (bestCol != i) {
            swapColumns(M, i, bestCol);
            int temp = columnOrder[i];
            columnOrder[i] = columnOrder[bestCol];
            columnOrder[bestCol] = temp;
        }

        // Normalize the pivot row
        double pivot = M[i][i];
        if (fabs(pivot) < 1e-9) {
            printf("Matrix is singular or near singular.\n");
            return;
        }
        for (int j = i; j < MATRIX_SIZE; j++) {
            M[i][j] /= pivot;
        }
        B[i] /= pivot;

        // Eliminate other rows
        for (int k = i + 1; k < MATRIX_SIZE; k++) {
            double factor = M[k][i];
            for (int j = i; j < MATRIX_SIZE; j++) {
                M[k][j] -= factor * M[i][j];
            }
            B[k] -= factor * B[i];
        }
    }

    // Back substitution
    for (int i = MATRIX_SIZE - 1; i >= 0; i--) {
        X[i] = B[i];
        for (int j = i + 1; j < MATRIX_SIZE; j++) {
            X[i] -= M[i][j] * X[j];
        }
    }

    // Reorder the solution vector based on column swaps
    double reorderedX[MATRIX_SIZE];
    for (int i = 0; i < MATRIX_SIZE; i++) {
        reorderedX[columnOrder[i]] = X[i];
    }

    // Extract the transformation matrix A and translation vector T from reorderedX
    A->m[0][0] = reorderedX[0];
    A->m[0][1] = reorderedX[1];
    A->m[1][0] = reorderedX[2];
    A->m[1][1] = reorderedX[3];
    T->x = reorderedX[4];
    T->y = reorderedX[5];
}

// Function to find the inverse of a 2x2 matrix
void findInverse(Matrix2x2 *A, Matrix2x2 *A_inv) {
    double det = A->m[0][0] * A->m[1][1] - A->m[0][1] * A->m[1][0];
    if (fabs(det) < 1e-9) {
        printf("Matrix is singular or near singular.\n");
        return;
    }
    double invDet = 1.0 / det;
    A_inv->m[0][0] =  A->m[1][1] * invDet;
    A_inv->m[0][1] = -A->m[0][1] * invDet;
    A_inv->m[1][0] = -A->m[1][0] * invDet;
    A_inv->m[1][1] =  A->m[0][0] * invDet;
}

// Function to find the transformation matrix, translation vector, and inverse matrix
void findTransformationAndInverse(point p[3], point q[3], Matrix2x2 *A, Vector2 *T, Matrix2x2 *A_inv) {
    // Find the transformation matrix A and translation vector T
    findTransformation(p, q, A, T);
    // Find the inverse of the transformation matrix A
    findInverse(A, A_inv);
}

point* reverseTransformPoints(point* points, int pointCount, Matrix2x2* A_inv, Vector2* T) {
    point* transformedPoints = malloc(pointCount * sizeof(point));
    if (!transformedPoints) {
        printf("Memory allocation error.\n");
        return NULL;
    }

    for (int i = 0; i < pointCount; i++) {
        // Subtract the translation vector T from the point q
        double qx = points[i].x - T->x;
        double qy = points[i].y - T->y;

        // Apply the inverse transformation matrix A_inv
        transformedPoints[i].x = A_inv->m[0][0] * qx + A_inv->m[0][1] * qy;
        transformedPoints[i].y = A_inv->m[1][0] * qx + A_inv->m[1][1] * qy;
    }

    return transformedPoints;
}


point* readCSVAndStorePoints(const char* filename, int* validPointCount) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
        return NULL;
    }

    char line[256];
    int count = 0;
    point* points = NULL;

    // Read the header line
    if (fgets(line, sizeof(line), file)) {
        // Skip the header
    }

    // Read each line of the file
    while (fgets(line, sizeof(line), file)) {
        double magX, magY, magZ;
        if (sscanf(line, "%lf,%lf,%lf", &magX, &magY, &magZ) == 3) {
            // Allocate memory for the points array
            points = realloc(points, (count + 1) * sizeof(point));
            if (!points) {
                printf("Memory allocation error.\n");
                fclose(file);
                return NULL;
            }
            // Store the valid point
            points[count].x = magX;
            points[count].y = magZ;
            count++;
        }
    }

    fclose(file);
    *validPointCount = count;
    return points;
}

void writePointsToCSV(const char* filename, point* points, int pointCount) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Write the header
    fprintf(file, "Transformed X,Transformed Y\n");

    // Write each point
    for (int i = 0; i < pointCount; i++) {
        fprintf(file, "%f,%f\n", points[i].x, points[i].y);
    }

    fclose(file);
}

#endif