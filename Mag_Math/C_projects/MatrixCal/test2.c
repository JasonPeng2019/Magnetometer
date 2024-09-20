#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Routines/f2c.h"
#include "Routines/blaswrap.h"

/* LAPACK sggev prototype */
extern int sggev_(char *jobvl, char *jobvr, integer *n, real *a, integer *lda,
                   real *b, integer *ldb, real *alphar, real *alphai, real *beta,
                   real *vl, integer *ldvl, real *vr, integer *ldvr, real *work,
                   integer *lwork, integer *info);

void print_matrix(const char* name, float* mat, int n, int m) {
    printf("%s:\n", name);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%f ", mat[j*n + i]);
        }
        printf("\n");
    }
}

void print_vector(const char* name, float* vec, int n) {
    printf("%s:\n", name);
    for (int i = 0; i < n; i++) {
        printf("%f ", vec[i]);
    }
    printf("\n");
}

int main() {
    char jobvl = 'N';  // Do not compute the left eigenvectors
    char jobvr = 'V';  // Compute the right eigenvectors
    integer n = 2;     // Order of the matrices
    integer lda = 2;   // Leading dimension of A
    integer ldb = 2;   // Leading dimension of B
    integer ldvl = 1;  // Leading dimension of the left eigenvector matrix
    integer ldvr = 2;  // Leading dimension of the right eigenvector matrix
    integer info;

    // Define matrices A and B in column-major order
    float A[4] = {0, -2, 1, -3};  // A = [0, 1; -2, -3]
    float B[4] = {1, 0, 0, 1};    // B = [1, 0; 0, 1]

    // Copies of A and B for computations
    float Acopy[4], Bcopy[4];

    // Prepare arrays to store eigenvalues
    float alphar[2];  // Real part of the eigenvalues
    float alphai[2];  // Imaginary part of the eigenvalues
    float beta[2];    // Beta (part of the generalized eigenvalue)

    // Arrays to store eigenvectors
    float vr[4];      // Right eigenvectors (stored column-wise)
    float vl[1];      // Left eigenvectors (not computed)

    // Workspace query
    float work_query[1];
    integer lwork = -1;

    // Copy A and B for workspace query
    memcpy(Acopy, A, 4 * sizeof(float));
    memcpy(Bcopy, B, 4 * sizeof(float));

    // First call to query optimal workspace size
    sggev_(&jobvl, &jobvr, &n, Acopy, &lda, Bcopy, &ldb, alphar, alphai, beta, vl, &ldvl, vr, &ldvr, work_query, &lwork, &info);

    if (info != 0) {
        printf("Workspace query error, info = %d\n", info);
        return -1;
    }

    // Allocate optimal workspace size
    lwork = (integer)work_query[0];
    float* work = (float*)malloc(lwork * sizeof(float));

    // Copy A and B again for actual computation
    memcpy(Acopy, A, 4 * sizeof(float));
    memcpy(Bcopy, B, 4 * sizeof(float));

    // Call sggev_ to compute eigenvalues and eigenvectors
    sggev_(&jobvl, &jobvr, &n, Acopy, &lda, Bcopy, &ldb, alphar, alphai, beta, vl, &ldvl, vr, &ldvr, work, &lwork, &info);

    // Free allocated memory
    free(work);

    if (info != 0) {
        printf("Error computing eigenvalues and eigenvectors, info = %d\n", info);
        return -1;
    }

    // Print eigenvalues (alphar and alphai / beta)
    printf("Generalized Eigenvalues (alphar/beta, alphai/beta):\n");
    for (int i = 0; i < n; i++) {
        if (alphai[i] == 0) {
            printf("Real: %f\n", alphar[i] / beta[i]);
        } else {
            printf("Complex: %f + %fi\n", alphar[i] / beta[i], alphai[i] / beta[i]);
        }
    }

    // Print right eigenvectors (stored column-wise in vr)
    print_matrix("Right Eigenvectors (column-wise)", vr, n, n);
    for (int i = 0; i<4; i++){
        printf("%f",vr[i]);
    }

    return 0;
}