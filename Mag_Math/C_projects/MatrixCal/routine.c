#include "linearalgebra.h"
#include "Routines/clapack.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <customheaders.h>

int main(){
    int numPoints;
    Point* pointsArray = createPointsArray("Data/trial1output.csv", &numPoints);
    int numValues = numPoints * 6;

    float * D = createMatrixArray(createPointsArray);
    
    free(pointsArray);
    
    float * DTD = malloc(6 * 6 * sizeof(float));
    float * dr = malloc(6 * sizeof(float));
    float * di = malloc(6 * sizeof(float));

    float *D_tran = malloc(numPoints * sizeof(float));
    memcpy(D_tran, D, numValues * sizeof(float));
    tran(D_tran, numPoints, 6);

    mul(D_tran, D, DTD, 6, numPoints, 6);
    eig_generalized()

}


