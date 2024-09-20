#include <stdio.h>
#include <stdlib.h>
#include "customheaders.h"


void rowToColumnMajor(const float* rowMajorMatrix, float* colMajorMatrix, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            // Calculate the correct index for column-major order
            colMajorMatrix[j * rows + i] = rowMajorMatrix[i * cols + j];
        }
    }
}

void columnToRowMajor(const float* colMajor, float* rowMajor, size_t rows, size_t cols) {
    for (size_t j = 0; j < cols; ++j) {
        for (size_t i = 0; i < rows; ++i) {
            rowMajor[i * cols + j] = colMajor[j * rows + i];
        }
    }
}