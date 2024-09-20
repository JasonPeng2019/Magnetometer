/*
 * eig_generalized.c
 *
 *  Created on: 26 mars 2023
 *      Author: Daniel M�rtensson
 */

#ifdef EIG_NEW

#include "linearalgebra.h"
#include "issymmetric.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


 /* Include LAPACK routines */
#ifdef CLAPACK_USED
//#include "Routines/clapack.h"
#include "Routines/f2c.h"
#include "Routines/blaswrap.h"
#include "Routines/clapack.h"
#endif


/*
 * tran.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

/*
 * Turn A into transponse A^T
 * A[m*n]
 */
void tran(float A[], const size_t row, const size_t column) {
	/* Declare */
	float* B = (float*)malloc(row * column * sizeof(float));
	size_t i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			B[j * row + i] = A[i * column + j];
		}
	}

	/* Copy over */
	memcpy(A, B, row * column * sizeof(float));

	/* Free */
	free(B);
}

/* Old code
// Decleration
float* B = (float*)malloc(row * column * sizeof(float));
float* transpose = NULL;
float* ptr_A = A;
size_t i, j;

for (i = 0; i < row; i++) {
	transpose = &B[i];
	for (j = 0; j < column; j++) {
		*transpose = *ptr_A;
		ptr_A++;
		transpose += row;
	}
}

// Copy!
memcpy(A, B, row * column * sizeof(float));

// Free
free(B);
*/

/*
 * GNU Octave code:
 * >> A = [4 23 5; 2 45 5]
	A =

		4   23    5
		2   45    5

	>> A'
	ans =

		4    2
	   23   45
		5    5

	>>
 */


 /*
  * Compute eigenvalues and eigenvectors from matrix A and B
  * A[m*n]
  * B[m*n]
  * n == m
  * A^T = A
  * B^T = B
  * dr[m] = Eigenvalues real
  * di[m] = Eigenvalues imaginary
  * wr[m*n] = Eigenvectors real
  * wi[m*n] = Eigenvectors imaginary
  */

extern int sggev_(char *jobvl, char *jobvr, integer *n, real *a, 
	integer *lda, real *b, integer *ldb, real *alphar, real *alphai, real 
	*beta, real *vl, integer *ldvl, real *vr, integer *ldvr, real *work, 
	integer *lwork, integer *info);

extern int ssygvd_(integer *itype, char *jobz, char *uplo, integer *
	n, real *a, integer *lda, real *b, integer *ldb, real *w, real *work, 
	integer *lwork, integer *iwork, integer *liwork, integer *info);

bool eig_generalized(float A[], float B[], size_t row, float dr[], float di[], float wr[], float wi[]) {

#ifdef CLAPACK_USED
	/* Check if it's symmetric */
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

/*
        char jobvl = 'N';  // Do not compute the left eigenvectors
        char jobvr = 'V';  // Compute the right eigenvectors
        integer n = row;     // Order of the matrices
        integer lda = row;   // Leading dimension of A
        integer ldb = row;   // Leading dimension of B
        integer ldvl = 1;  // Leading dimension of the left eigenvector matrix
        integer ldvr = row;  // Leading dimension of the right eigenvector matrix
        integer info;
        float vr[n*n];      // Right eigenvectors (stored column-wise)
        float vl[1];      // Left eigenvectors (not computed)
        float beta[2];

        float work_query[1];
        integer lwork = -1;
    	
        float* Acopy = (float*)malloc(row * row * sizeof(float));
        memcpy(Acopy, A, 4 * sizeof(float));
       
        
        float* Bcopy = (float*)malloc(row * row * sizeof(float));
        memcpy(Bcopy, B, row * row * sizeof(float));
        

        sggev_(&jobvl, &jobvr, &n, Acopy, &lda, Bcopy, &ldb, dr, di, beta, vl, &ldvl, wr, &ldvr, work_query, &lwork, &info);
        if (info != 0) {
            printf("Workspace query error, info = %d\n", info);
            return 0;
        }

        lwork = (integer)work_query[0];
        float* work = (float*)malloc(lwork * sizeof(float));

        memcpy(Acopy, A, 4 * sizeof(float));
        memcpy(Bcopy, B, row * row * sizeof(float));

        sggev_(&jobvl, &jobvr, &n, Acopy, &lda, Bcopy, &ldb, dr, di, beta, vl, &ldvl, wr, &ldvr, work_query, &lwork, &info);

        free(Bcopy);
		free(Acopy);
        free(work);

        if (info != 0) {
            printf("Error computing eigenvalues and eigenvectors, info = %d\n", info);
            return 0;
        }
        */

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

        size_t i, j;
		for (i = 0; i < row; i++) {
			/* Check if it's a real eigenvalue */
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
                    wr[i+j] = vr[s++];
                }
            }
            else {
				t = s;
				for (j = 0; j < row; j++) {
					wr[i + j] = vr[t++];
				}
				for (j = 0; j < row; j++) {
					wi[i + j] = vr[t++];
				}
			}
        }
        return info == 0;
    }
#elif defined(MKL_LAPACK_USED)
	/* Check if it's symmetric */
	bool issymmetricA = issymmetric(A, row, row);
	bool issymmetricB = issymmetric(B, row, row);
	
	if (issymmetricA && issymmetricB) {
		float* Bcopy = (float*)malloc(row * row * sizeof(float));
		memcpy(Bcopy, B, row * row * sizeof(float));
		memcpy(wr, A, row * row * sizeof(float));
		bool status = LAPACKE_ssygvd(LAPACK_ROW_MAJOR, 1, 'V', 'U', row, wr, row, Bcopy, row, dr) == 0;
		free(Bcopy);
		return status;
	}
	else {
		/* Eigenvectors */
		float* vl = (float*)malloc(row * row * sizeof(float));
		float* vr = (float*)malloc(row * row * sizeof(float));

		/* Beta */
		float* beta = (float*)malloc(row * sizeof(float));

		/* Copy */
		float* Bcopy = (float*)malloc(row * row * sizeof(float));
		memcpy(Bcopy, B, row* row * sizeof(float));
		float* Acopy = (float*)malloc(row * row * sizeof(float));
		memcpy(Acopy, A, row* row * sizeof(float));

		/* Compute */
		bool status = LAPACKE_sggev(LAPACK_COL_MAJOR, 'V', 'N', row, Acopy, row, Bcopy, row, dr, di, beta, vl, row, vr, row) == 0;

		/* Compute the eigenvalues */
		size_t i;
		for (i = 0; i < row; i++) {
			/* Check if it's a real eigenvalue */
			if (fabsf(di[i]) < MIN_VALUE) {
				dr[i] = dr[i] / beta[i];
				di[i] = 0.0f;
			}
			else {
				dr[i] = dr[i] / beta[i];
				di[i] = di[i] / beta[i];
			}
		}

		/* Fill the eigenvectors in row major */
		size_t j, s = 0, t = 0;
		memset(wi, 0, row * row * sizeof(float));
		for (i = 0; i < row; i++) {
			if (fabsf(di[i]) < MIN_VALUE) {
				s = t;
				for (j = 0; j < row; j++) {
					wr[j * row + i] = vl[s++];
				}
				t = s;
			}
			else {
				t = s;
				for (j = 0; j < row; j++) {
					wr[j * row + i] = vl[t++];
				}
				for (j = 0; j < row; j++) {
					wi[j * row + i] = vl[t++];
				}
			}
		}

		/* Free */
		free(vl);
		free(vr);
		free(beta);
		free(Bcopy);
		free(Acopy);

		/* Return status */
		return status;
	}

#else
	/* Copy B */
	float* Bcopy = (float*)malloc(row * row * sizeof(float));
	memcpy(Bcopy, B, row * row * sizeof(float));

	/* Check determinant */
	if (fabsf(det(Bcopy, row)) < MIN_VALUE) {
		/* Do regularization */
		size_t i;
		float* Bcopy0 = Bcopy;
		for (i = 0; i < row; i++) {
			Bcopy0[i] += FLT_EPSILON;
			Bcopy0 += row;
		}
	}

	/* Take inverse on Bcopy */
	inv(Bcopy, row);

	/* Multiply Bcopy * A = X */
	float* X = (float*)malloc(row * row * sizeof(float));
	mul(Bcopy, A, X, row, row, row);

	/* Do eigendecomposition */
	bool status = eig(X, dr, di, wr, wi, row);

	/* Free */
	free(Bcopy);
	free(X);

	/* Return status */
	return status;
#endif /*MKL LAPACK USED*/
}

int main(){
	float A[4] = {1,2,2,3};
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
	for (int i = 0; i < (sizeof(dr)/sizeof(float)); i ++){
		printf("eigenvalue is %f + %f\n", dr[i], di[i]);
	}
	for (int i = 0; i < (sizeof(wr)/sizeof(float)); i ++){
		printf("eigenvector is %f %f\n", wr[i], wi[i]);
	}
	return 0;
}
#endif /**NEW_EIG GEN */

