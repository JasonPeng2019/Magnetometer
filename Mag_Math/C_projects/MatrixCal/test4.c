#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ROW         4

int main(){
    
    int i, j, s;
    i = 0;
    j = 0;
    s = 0;
    int row = ROW;
    int col = row;
    int n = ROW;
    float wi[ROW*ROW];
    float wr[ROW*ROW];

	float* vr = (float*)malloc(2* n * n * sizeof(float));
    float source[] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 1, 1, 1, 9, 10, 11, 12, 13, 14, 15, 16};
    // Copy the array into vr
    for (int i = 0; i < 2 * n * n; ++i) {
        vr[i] = source[i];
    }
    float beta[ROW] = {0,1,0,0};

    memset(wi, 0, row * row * sizeof(float));
    for (i = 0; i < row*row; i++){
        printf("%f,", wi[i]);
    }
    printf("\n");

    for (i = 0; i < col; i++) {
        if (beta[i] == 0){
            for (j = 0; j < row; j ++){
                printf("%d,%d,\n", i, j);
                wr[row*i+j] = vr[s];
                s++;
            }
        }
        else if (beta[i] == 1){
            for (j = 0; j < row; j++) {
                wr[row*i + j] = vr[s++];
            }
            for (j = 0; j < row; j++) {
                wi[row*i + j] = vr[s++];
            }
        }
    }
    for (i = 0 ; i < row*row; i++){
        printf("%f,", vr[i]);
    }
    printf("\n");
    for (i = 0; i < row*row; i++){
        printf("%f,", wr[i]);
    }
    printf("\n");
    for (i = 0; i < row*row; i++){
        printf("%f,", wi[i]);
    }
}