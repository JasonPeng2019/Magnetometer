/*
 * eig_generalized.c
 *
 *  Created on: 26 mars 2023
 *      Author: Daniel M�rtensson
 */

#include "linearalgebra.h"
#include "issymmetric.c"

 /* Include LAPACK routines */
#ifdef CLAPACK_USED
#include "CLapack/clapack.h"
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
bool eig_generalized(float A[], float B[], size_t row, float dr[], float di[], float wr[], float wi[]) {
#ifdef CLAPACK_USED
	/* Check if it's symmetric */
	bool issymmetricA = issymmetric(A, row, row);
	bool issymmetricB = issymmetric(B, row, row);
	
	if (issymmetricA && issymmetricB) {
		/* Settings */
		integer itype = 1, n = row, lda = row, ldb = row, lwork, iwkopt, liwork, info;
		real wkopt;

		/* Copy */
		float* Bcopy = (float*)malloc(row * row * sizeof(float));
		memcpy(Bcopy, B, row * row * sizeof(float));
		memcpy(wr, A, row * row * sizeof(float));

		/* Allocate memory */
		lwork = -1;
		liwork = -1;
		ssygvd_(&itype, "V", "U", &n, wr, &lda, Bcopy, &ldb, dr, &wkopt, &lwork, &iwkopt, &liwork, &info);

		/* Compute */
		lwork = (integer)wkopt;
		real* work = (real*)malloc(lwork * sizeof(real));
		liwork = iwkopt;
		integer* iwork = (integer*)malloc(liwork * sizeof(integer));
		ssygvd_(&itype, "V", "U", &n, wr, &lda, Bcopy, &ldb, dr, work, &lwork, iwork, &liwork, &info);

		/* Transpose of the eigenvectors */
		tran(wr, row, row);

		/* Free */
		free(Bcopy);
		free(work);
		free(iwork);

		/* Return status */
		return info == 0;
	}
	else {
		/* Settings */
		integer n = row, lda = row, ldb = row, info, lwork;
		real* beta = (real*)malloc(n * sizeof(real));
		real wkopt;

		/* Eigenvectors */
		real* vl = (real*)malloc(n * lda * sizeof(real));
		real* vr = (real*)malloc(n * lda * sizeof(real));

		/* Copy */
		float* Bcopy = (float*)malloc(row * row * sizeof(float));
		memcpy(Bcopy, B, row * row * sizeof(float));
		float* Acopy = (float*)malloc(row * row * sizeof(float));
		memcpy(Acopy, A, row * row * sizeof(float));

		/* Allocate memory */
		lwork = -1;
		sggev_("V", "N", &n, Acopy, &lda, Bcopy, &ldb, dr, di, beta, vl, &n, vr, &n, &wkopt, &lwork, &info);

		/* Compute */
		lwork = (integer)wkopt;
		real* work = (real*)malloc(lwork * sizeof(real));
		sggev_("V", "N", &n, Acopy, &lda, Bcopy, &ldb, dr, di, beta, vl, &n, vr, &n, work, &lwork, &info);

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
		free(beta);
		free(vl);
		free(vr);
		free(Bcopy);
		free(Acopy);

		/* Return status */
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
#endif
}

int main(){
	float A[4] = {0, 1, -2 ,3};
	float B[4] = {1, 0, 0, 1};
	size_t row = 2;
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
		printf("eigenvalue is %f + %f\n", wr[i], wi[i]);
	}
	return 0;
}
