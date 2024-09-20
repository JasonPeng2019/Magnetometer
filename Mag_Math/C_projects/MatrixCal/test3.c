#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Routines/f2c.h"
#include "Routines/blaswrap.h"

// LAPACK ssygvd_ prototype
extern void ssygvd_(integer *itype, char *jobz, char *uplo, integer *n, 
                    real *a, integer *lda, real *b, integer *ldb, real *w, 
                    real *work, integer *lwork, integer *iwork, 
                    integer *liwork, integer *info);

// Helper function to print a matrix
void print_matrix(const char* name, float* mat, int rows, int cols) {
    printf("%s:\n", name);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f ", mat[i * cols + j]);
        }
        printf("\n");
    }
}

// Helper function to print a vector
void print_vector(const char* name, float* vec, int n) {
    printf("%s:\n", name);
    for (int i = 0; i < n; i++) {
        printf("%f ", vec[i]);
    }
    printf("\n");
}

int main() {
    integer itype = 1;  // Problem type (1: A*x = lambda*B*x)
    char jobz = 'V';    // Compute eigenvalues and eigenvectors
    char uplo = 'U';    // Upper triangle of A and B are stored
    integer n = 2;      // Order of the matrices A and B
    integer lda = 2;    // Leading dimension of A
    integer ldb = 2;    // Leading dimension of B
    integer info;       // Output info

    // Define symmetric matrices A and B (column-major order)
    float A[4] = {1.0, 2, 2, 3};  // A = [1.0, 0.5; 0.5, 2.0]
    float B[4] = {1,0,0,1};  // B = [3.0, 0.5; 0.5, 4.0]

    // Arrays for eigenvalues and workspace query
    float w[2];         // Eigenvalues
    float work_query[1];
    integer lwork = -1;
    integer liwork_query[1];
    integer liwork = -1;

    // Workspace query
    ssygvd_(&itype, &jobz, &uplo, &n, A, &lda, B, &ldb, w, work_query, &lwork, liwork_query, &liwork, &info);
    if (info != 0) {
        printf("Error during workspace query, info = %d\n", info);
        return -1;
    }

    // Allocate optimal workspace
    lwork = (integer)work_query[0];
    liwork = liwork_query[0];
    float* work = (float*)malloc(lwork * sizeof(float));
    integer* iwork = (integer*)malloc(liwork * sizeof(integer));

    // Copy A and B as they will be overwritten
    float A_copy[4];
    float B_copy[4];
    memcpy(A_copy, A, sizeof(A));
    memcpy(B_copy, B, sizeof(B));

    // Compute eigenvalues and eigenvectors
    ssygvd_(&itype, &jobz, &uplo, &n, A_copy, &lda, B_copy, &ldb, w, work, &lwork, iwork, &liwork, &info);
    if (info != 0) {
        printf("Error computing eigenvalues and eigenvectors, info = %d\n", info);
        return -1;
    }

    // Output results
    for (int i = 0; i<4; i++){
        printf("Eigenvalues: %f\n", w[i]);
        printf("Eigenvectors: %f\n", A_copy[i]);
    }
    printf("\n");
    print_vector("Eigenvalues", w, n);
    print_matrix("Eigenvectors", A_copy, n, n);

    // Free allocated memory
    free(work);
    free(iwork);

    return 0;
}