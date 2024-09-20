#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Routines/f2c.h"
#include "Routines/blaswrap.h"

/* LAPACK ssyevd prototype */
extern void ssyevd_(char *jobz, char *uplo, integer *n, real *a, integer *lda, 
                    real *w, real *work, integer *lwork, integer *iwork, 
                    integer *liwork, integer *info);

void print_matrix(const char* name, float* mat, int n) {
    printf("%s:\n", name);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
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
    char jobz = 'V';  // Compute eigenvalues and eigenvectors
    char uplo = 'U';  // Upper triangle of A is stored
    integer n = 2;    // Order of the matrix
    integer lda = 2;  // Leading dimension of A
    integer info;

    // Define a symmetric matrix A in column-major order (upper triangle)
    float A1[4] = {1,2,2,3};  // A = [0, 1; -2, -3]
    float A2[4] = {1,2,2,3};
    float A3[4] = {1,2,2,3};

    // Prepare array to store eigenvalues
    float w[2];

    // Workspace query
    float work_query[1];
    integer lwork = -1;
    integer iwork_query[1];
    integer liwork = -1;

    // First call to query optimal workspace size
    ssyevd_(&jobz, &uplo, &n, A1, &lda, w, work_query, &lwork, iwork_query, &liwork, &info);

    if (info != 0) {
        printf("Error during workspace query, info = %d\n", info);
        return -1;
    }

    // Allocate optimal workspace size
    lwork = (integer)work_query[0];
    liwork = iwork_query[0];
    float* work = (float*)malloc(lwork * sizeof(float));
    integer* iwork = (integer*)malloc(liwork * sizeof(integer));

    // Call ssyevd_ to compute eigenvalues and eigenvectors
    ssyevd_(&jobz, &uplo, &n, A2, &lda, w, work, &lwork, iwork, &liwork, &info);

    // Free allocated memory
    free(work);
    free(iwork);

    if (info != 0) {
        printf("Error computing eigenvalues and eigenvectors, info = %d\n", info);
        return -1;
    }
    for (int i=0; i< 4; i++){
       printf("%f\n",A3[i]); 
    }
    // Print eigenvalues
    print_vector("Eigenvalues", w, n);

    // Print eigenvectors
    print_matrix("Eigenvectors", A2, n);

    return 0;
}