#ifdef USE_EASY
#include <stdio.h>
#include "inc/matrix.h"
#include "inc/filter.h"

Vector2 multiplyMatrixVector2x2(Matrix2x2 A, point p);
double determinant2x2(Matrix2x2 A);
Matrix2x2 inverse2x2(Matrix2x2 A);
void findTransformation(point p[3], point q[3], Matrix2x2 *A, Vector2 *T);



int main() {
    // Define original points on the circle
    point p[3] = {{1.0, 0.0}, {0.0, 1.0}, {-1.0, 0.0}};

    // Define corresponding points on the ellipse
    point q[3] = {{2.0, 1.0}, {0.0, 3.0}, {-2.0, 1.0}};

    // Variables to hold the results
    Matrix2x2 A;
    Vector2 T;

    // Find the transformation matrix A and translation vector T
    findTransformation(p, q, &A, &T);

    // Print the results
    printf("Transformation Matrix A:\n");
    printf("[%f %f]\n", A.m[0][0], A.m[0][1]);
    printf("[%f %f]\n", A.m[1][0], A.m[1][1]);

    printf("Translation Vector T:\n");
    printf("[%f, %f]\n", T.x, T.y);

    return 0;
}


// Function to multiply a 2x2 matrix with a 2D vector
Vector2 multiplyMatrixVector2x2(Matrix2x2 A, point p) {
    Vector2 result;
    result.x = A.m[0][0] * p.x + A.m[0][1] * p.y;
    result.y = A.m[1][0] * p.x + A.m[1][1] * p.y;
    return result;
}

// Function to calculate the determinant of a 2x2 matrix
double determinant2x2(Matrix2x2 A) {
    return A.m[0][0] * A.m[1][1] - A.m[0][1] * A.m[1][0];
}

// Function to calculate the inverse of a 2x2 matrix
Matrix2x2 inverse2x2(Matrix2x2 A) {
    Matrix2x2 inv;
    double det = determinant2x2(A);
    if (det == 0) {
        printf("Matrix is singular and cannot be inverted.\n");
        inv.m[0][0] = inv.m[0][1] = inv.m[1][0] = inv.m[1][1] = 0;
        return inv;
    }
    inv.m[0][0] = A.m[1][1] / det;
    inv.m[0][1] = -A.m[0][1] / det;
    inv.m[1][0] = -A.m[1][0] / det;
    inv.m[1][1] = A.m[0][0] / det;
    return inv;
}

void findTransformation(point p[3], point q[3], Matrix2x2 *A, Vector2 *T) {
    // Set up the system of linear equations in matrix form
    // Ax = b, where x = [a11, a12, a21, a22, tx, ty]

    // Matrix to solve [6x6 system]:
    double M[6][6] = {
        { p[0].x, p[0].y, 0, 0, 1, 0 },   // q1.x = A * p1 + T
        { 0, 0, p[0].x, p[0].y, 0, 1 },   // q1.y = A * p1 + T
        { p[1].x, p[1].y, 0, 0, 1, 0 },   // q2.x = A * p2 + T
        { 0, 0, p[1].x, p[1].y, 0, 1 },   // q2.y = A * p2 + T
        { p[2].x, p[2].y, 0, 0, 1, 0 },   // q3.x = A * p3 + T
        { 0, 0, p[2].x, p[2].y, 0, 1 }    // q3.y = A * p3 + T
    };

    // Right-hand side (known points on the ellipse):
    double B[6] = { q[0].x, q[0].y, q[1].x, q[1].y, q[2].x, q[2].y };

    // Solve the linear system M * x = B for x using Gaussian elimination or matrix inversion

    // Result will be stored in the array X
    double X[6];

    // Gaussian elimination (simple implementation without pivoting)
    for (int i = 0; i < 6; i++) {
        // Normalize row i
        double diag = M[i][i];
        for (int j = 0; j < 6; j++) {
            M[i][j] /= diag;
        }
        B[i] /= diag;

        // Eliminate column i in subsequent rows
        for (int k = i + 1; k < 6; k++) {
            double factor = M[k][i];
            for (int j = 0; j < 6; j++) {
                M[k][j] -= factor * M[i][j];
            }
            B[k] -= factor * B[i];
        }
    }

    // Back substitution
    for (int i = 5; i >= 0; i--) {
        X[i] = B[i];
        for (int j = i + 1; j < 6; j++) {
            X[i] -= M[i][j] * X[j];
        }
    }

    // Extract the transformation matrix A and translation vector T from X
    A->m[0][0] = X[0];
    A->m[0][1] = X[1];
    A->m[1][0] = X[2];
    A->m[1][1] = X[3];
    T->x = X[4];
    T->y = X[5];
}
#endif