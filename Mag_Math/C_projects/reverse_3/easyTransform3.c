#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MATRIX_SIZE 7

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Define a structure for points
typedef struct {
    double x;
    double y;
} Point;

// Function prototypes
Point* reverseTransformPoints(Point* ellipsePoints, int pointCount, double A[2][2], double T[2], double theta0);
void solveSystem(Point ellipsePoints[4], double angles[3], double A[2][2], double T[2], double *theta0);
void gaussian_elimination(double M[7][MATRIX_SIZE], double B[7], double X[MATRIX_SIZE]);
int findBestPivot(double M[7][MATRIX_SIZE], int size, int startRow, int startCol, int *bestRow, int *bestCol);
void swapRows(double M[7][MATRIX_SIZE], double B[7], int row1, int row2);
void swapColumns(double M[7][MATRIX_SIZE], int col1, int col2);

int main() {
    // Points on the transformed ellipse (p1, p2, p3, p4)
    Point ellipsePoints[4] = {
        {34.5, 561},   // p1
        {-55.5, 828},   // p2
        {160.5, 1224},   // p3
        {253.5, 982.5}    // p4
    };

    double angles[4] = {0, M_PI / 2, M_PI, 3 * M_PI / 2};

    // Matrix and translation vector
    double A[2][2];  // Transformation matrix
    double T[2];     // Translation vector
    double theta0;   // Initial orientation

    // Solve the system to find the scaling factors, translation vector, and theta0
    solveSystem(ellipsePoints, angles, A, T, &theta0);

    // Output the transformation matrix and translation vector
    printf("Transformation Matrix (A):\n");
    printf("| %f %f |\n", A[0][0], A[0][1]); 
    printf("| %f %f |\n", A[1][0], A[1][1]);
    printf("Translation Vector (T):\n");
    printf("tx = %f, ty = %f\n", T[0], T[1]);
    printf("Initial orientation (theta0): %f\n", theta0);

    // Perform the reverse transformation to recover the original points on the circle
    Point* recoveredCirclePoints = reverseTransformPoints(ellipsePoints, 4, A, T, theta0);

    // Output the recovered points (should be close to the original points on the circle)
    printf("Recovered Circle Points:\n");
    for (int i = 0; i < 4; i++) {
        printf("Recovered Point %d: (%.2f, %.2f)\n", i + 1, recoveredCirclePoints[i].x, recoveredCirclePoints[i].y);
    }

    free(recoveredCirclePoints);
    return 0;
}

// Perform reverse transformation from ellipse back to original points on the circle
Point* reverseTransformPoints(Point* ellipsePoints, int pointCount, double A[2][2], double T[2], double theta0) {
    Point* circlePoints = malloc(pointCount * sizeof(Point));
    if (!circlePoints) {
        printf("Memory allocation error.\n");
        return NULL;
    }

    for (int i = 0; i < pointCount; i++) {
        // Subtract the translation vector T from the ellipse point q
        double qx = ellipsePoints[i].x - T[0];
        double qy = ellipsePoints[i].y - T[1];

        // Apply the inverse of the transformation matrix A (with initial orientation theta0)
        double cos_theta0 = cos(theta0);
        double sin_theta0 = sin(theta0);

        double denom = A[0][0] * A[1][1] - A[0][1] * A[1][0]; // determinant of A
        circlePoints[i].x = (A[1][1] * (qx * cos_theta0 - qy * sin_theta0) - A[0][1] * (qx * sin_theta0 + qy * cos_theta0)) / denom;
        circlePoints[i].y = (-A[1][0] * (qx * cos_theta0 - qy * sin_theta0) + A[0][0] * (qx * sin_theta0 + qy * cos_theta0)) / denom;
    }

    return circlePoints;
}

// Solve system to find the transformation matrix (A), translation vector (T), and theta0
void solveSystem(Point ellipsePoints[4], double angles[3], double A[2][2], double T[2], double *theta0) {
    double M[7][MATRIX_SIZE];  // The design matrix with 7 equations
    double B[7];               // Right-hand side vector
    double result[MATRIX_SIZE]; // Solution vector for a, b, c, d, tx, ty, theta0

    // Fill the system of equations
    for (int i = 0; i < 3; i++) {
        double theta = *theta0 + angles[i];

        double cos_theta = cos(theta);
        double sin_theta = sin(theta);

        // x equation
        M[2 * i][0] = cos_theta;  // a
        M[2 * i][1] = sin_theta;  // b
        M[2 * i][2] = 0;          // c
        M[2 * i][3] = 0;          // d
        M[2 * i][4] = 1;          // tx
        M[2 * i][5] = 0;          // ty
        M[2 * i][6] = -sin_theta * ellipsePoints[i].x + cos_theta * ellipsePoints[i].y; // theta0
        B[2 * i] = ellipsePoints[i].x;

        // y equation
        M[2 * i + 1][0] = 0;      // a
        M[2 * i + 1][1] = 0;      // b
        M[2 * i + 1][2] = cos_theta;  // c
        M[2 * i + 1][3] = sin_theta;  // d
        M[2 * i + 1][4] = 0;          // tx
        M[2 * i + 1][5] = 1;          // ty
        M[2 * i + 1][6] = sin_theta * ellipsePoints[i].x + cos_theta * ellipsePoints[i].y; // theta0
        B[2 * i + 1] = ellipsePoints[i].y;
    }

    // For the 4th point, only use the x equation (7th equation in total)
    double theta = *theta0 + angles[2] + M_PI / 2;  // Assuming the last angle wraps around
    double cos_theta = cos(theta);
    double sin_theta = sin(theta);

    M[6][0] = cos_theta;  // a
    M[6][1] = sin_theta;  // b
    M[6][2] = 0;          // c
    M[6][3] = 0;          // d
    M[6][4] = 1;          // tx
    M[6][5] = 0;          // ty
    M[6][6] = -sin_theta * ellipsePoints[3].x + cos_theta * ellipsePoints[3].y; // theta0
    B[6] = ellipsePoints[3].x;

    // Solve the system using Gaussian elimination
    gaussian_elimination(M, B, result);

    // Extract the solution
    A[0][0] = result[0];  // a
    A[0][1] = result[1];  // b
    A[1][0] = result[2];  // c
    A[1][1] = result[3];  // d
    T[0] = result[4];     // tx
    T[1] = result[5];     // ty
    *theta0 = result[6];  // theta0
}

// Gaussian elimination with partial pivoting
void gaussian_elimination(double M[7][MATRIX_SIZE], double B[7], double X[MATRIX_SIZE]) {
    int columnOrder[MATRIX_SIZE];  // To keep track of column swaps

    // Initialize column order to be 0, 1, 2, ..., MATRIX_SIZE-1
    for (int i = 0; i < MATRIX_SIZE; i++) {
        columnOrder[i] = i;
    }

    // Perform Gaussian elimination
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

    // Copy reordered solution back into X
    for (int i = 0; i < MATRIX_SIZE; i++) {
        X[i] = reorderedX[i];
    }
}

// Function to find the best pivot element in the matrix
int findBestPivot(double M[7][MATRIX_SIZE], int size, int startRow, int startCol, int *bestRow, int *bestCol) {
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

// Function to swap rows in the matrix and vector B
void swapRows(double M[7][MATRIX_SIZE], double B[7], int row1, int row2) {
    double temp;
    // Swap rows in the matrix
    for (int i = 0; i < MATRIX_SIZE; i++) {
        temp = M[row1][i];
        M[row1][i] = M[row2][i];
        M[row2][i] = temp;
    }

    // Swap corresponding elements in vector B
    temp = B[row1];
    B[row1] = B[row2];
    B[row2] = temp;
}

// Function to swap columns in the matrix
void swapColumns(double M[7][MATRIX_SIZE], int col1, int col2) {
    double temp;
    // Swap columns in the matrix
    for (int i = 0; i < MATRIX_SIZE; i++) {
        temp = M[i][col1];
        M[i][col1] = M[i][col2];
        M[i][col2] = temp;
    }
}