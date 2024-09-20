#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "Routines/f2c.h"
#include "Routines/blaswrap.h"
#include "Routines/clapack.h"
#include "customheaders.h"
#include "linearalgebra.h"
#define EIG_GEN2_TEST

/* LAPACK sggev prototype */



bool eig_generalized(float A[], float B[], size_t row, float dr[], float di[], float wr[], float wi[]) {
    
    bool issymmetricA = issymmetric(A, row, row);
	bool issymmetricB = issymmetric(B, row, row);
	
	if (issymmetricA && issymmetricB) {
        integer itype = 1;
        char jobz = 'V';  // Compute eigenvalues and eigenvectors
        char uplo = 'U';  // Upper triangle of A is stored
        integer n = row, lda=row, ldb=row, info;    

        float work_query[1];
        integer lwork = -1;
        integer liwork_query[1];
        integer liwork = -1;

		float* Acopy = (float*)malloc(row * row * sizeof(float));
        memcpy(wr, A, row * row * sizeof(float));
        memcpy(Acopy, A, row*row*sizeof(float));

        float* Bcopy = (float*)malloc(row * row * sizeof(float));
        memcpy(Bcopy, B, row * row * sizeof(float));
        float* B2copy = (float*)malloc(row * row * sizeof(float));
        memcpy(B2copy, B, row * row * sizeof(float));

        ssygvd_(&itype, &jobz, &uplo, &n, Acopy, &lda, Bcopy, &ldb, wr, work_query, &lwork, liwork_query, &liwork, &info);
        if (info != 0) {
            printf("Error during workspace query, info = %d\n", info);
            return 0;
        }

        lwork = (integer)work_query[0];
        liwork = liwork_query[0];
        float* work = (float*)malloc(lwork * sizeof(float));
        integer* iwork = (integer*)malloc(liwork * sizeof(integer));

        ssygvd_(&itype, &jobz, &uplo, &n, wr, &lda, B2copy, &ldb, dr, work, &lwork, iwork, &liwork, &info);
        if (info != 0) {
            printf("Error computing eigenvalues and eigenvectors, info = %d\n", info);
            return 0;
        }

        free(Acopy);
        free(Bcopy);
        free(B2copy);
		free(work);
		free(iwork);

		/* Return status */
		return info == 0;
    } else {
        char jobvl = 'N';  // Do not compute the left eigenvectors
        char jobvr = 'V';  // Compute the right eigenvectors
        integer n = row;     // Order of the matrices
        integer lda = row;   // Leading dimension of A
        integer ldb = row;   // Leading dimension of B
        integer ldvl = 1;  // Leading dimension of the left eigenvector matrix
        integer ldvr = row;  // Leading dimension of the right eigenvector matrix
        integer info;

        // Define matrices A and B in column-major order
        float* Bcopy = (float*)malloc(row * row * sizeof(float));
        memcpy(Bcopy, B, row * row * sizeof(float));
        float* Acopy = (float*)malloc(row * row * sizeof(float));
        memcpy(Acopy, A, row * row * sizeof(float));

        // Copies of A and B for computations

        // Prepare arrays to store eigenvalues
        float * alphar = dr;  // Real part of the eigenvalues
        float * alphai = di;  // Imaginary part of the eigenvalues
        float* beta = (float*)malloc(n * sizeof(float));
    // Beta (part of the generalized eigenvalue)

        // Arrays to store eigenvectors
        float* vl = (float*)malloc(sizeof(float));
        float* vr = (float*)malloc(2* n * lda * sizeof(float));

        // Workspace query
        float work_query[1];
        integer lwork = -1;

        // Copy A and B for workspace query
        memcpy(Acopy, A, row * row * sizeof(float));
        memcpy(Bcopy, B, row * row * sizeof(float));

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
        memcpy(Acopy, A, row * row * sizeof(float));
        memcpy(Bcopy, B, row * row * sizeof(float));

        // Call sggev_ to compute eigenvalues and eigenvectors
        sggev_(&jobvl, &jobvr, &n, Acopy, &lda, Bcopy, &ldb, alphar, alphai, beta, vl, &ldvl, vr, &ldvr, work, &lwork, &info);

        // Free allocated memory
        free(work);
        free(Acopy);
        free(Bcopy);

        if (info != 0) {
            printf("Error computing eigenvalues and eigenvectors, info = %d\n", info);
            return -1;
        }
    
        size_t i, j;
        for (i = 0; i < row; i++) {
            // Check if it's a real eigenvalue 
            if (fabsf(di[i]) < MIN_VALUE) {
                dr[i] = dr[i] / beta[i];
                di[i] = 0.0f;
            }
            else {
                dr[i] = dr[i] / beta[i];
                di[i] = di[i] / beta[i];
            }
        }
        
        i = 0;
        j = 0;
        memset(wi, 0, row * row * sizeof(float));
        size_t s = 0; size_t t = 0;
        for (i = 0; i < row; i++) {
            if (fabsf(di[i]) < MIN_VALUE){
                for (j = 0; j < row; j ++){
                    printf("%d,%d,\n", i, j);
                    wr[row * i+j] = vr[s];
                    s++;
                }
            }
            else {
                t = s;
                for (j = 0; j < row; j++) {
                    wr[row * i + j] = vr[t++];
                }
                for (j = 0; j < row; j++) {
                    wi[row * i + j] = vr[t++];
                }
            }
        }
        free(vr);
        free(vl);
        return (info == 0);
    }
}

#ifdef EIG_GEN2_TEST
int main(){
	float A[4] = {0, -2, 1 ,-3};
	float B[4] = {1, 0, 0, 1};
	integer row = 2;
	float dr[2];
	float wr[4];
	float di[2];
	float wi[4];

	bool isSuccess = eig_generalized(A, B, row, dr, di, wr, wi);
    
	if (!isSuccess) {
		printf("Failed to compute eigenvalues and eigenvectors.\n");
		return -1; // Exit if computation failed
	}
    printf("\neigenvalues:\n");
	for (int i = 0; i < row; i ++){
		printf("%f + %f\n", dr[i], di[i]);
	}
	printf("\neigenvectors:\n");
    for (int i = 0; i < row * row; i += row){
        printf("[%f + %f, %f + %f]", wr[i], wi[i], wr[i+1], wi[i+1]);
    }
	return 0;
    
}
#endif /*EIG GEN 2*/


