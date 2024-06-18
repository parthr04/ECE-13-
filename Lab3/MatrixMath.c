#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "MatrixMath.h"
#include <math.h>


void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    int x, y, z;
    for (x = 0; x <= 2; x++) {
        for (y = 0; y <= 2; y++) {
            for (z = 0; z <= 2; z++) {
                result[x][y] += mat1[x][z] * mat2[z][y];
            }
        }
    }
}

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]) {

    int x, y;

    for (x = 0; x <= 2; x++) {
        for (y = 0; y <= 2; y++) {
            result[x][y] = mat1[x][y] + mat2[x][y];
        }
    }

}

int MatrixEquals(float mat1[3][3], float mat2[3][3]) {
    int x, y;
    int result = 1;

    for (x = 0; x <= 2; x++) {
        for (y = 0; y <= 2; y++) {

            if (fabs(mat1[x][y] - mat2[x][y]) > FP_DELTA) {
                result = 0;
            }
        }
    }
    return result;

}

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]) {
    int z, y;

    for (z = 0; z <= 2; z++) {
        for (y = 0; y <= 2; y++) {
            result[z][y] = x * mat[z][y];
        }
    }
}

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]) {
    int z, y;

    for (z = 0; z <= 2; z++) {
        for (y = 0; y <= 2; y++) {
            result[z][y] = mat[z][y] + x;
        }
    }
}

float MatrixDeterminant(float mat[3][3]) {
    float det1, det2, det3 = 0; 
    float det4, det5, det6 = 0; 
    float detTotal = 0;

   
    det1 = mat[0][0] * mat[1][1] * mat[2][2];
    det2 = mat[1][0] * mat[2][1] * mat[0][2];
    det3 = mat[2][0] * mat[0][1] * mat[1][2];

    det4 = mat[0][2] * mat[1][1] * mat[2][0];
    det5 = mat[1][2] * mat[2][1] * mat[0][0];
    det6 = mat[2][2] * mat[0][1] * mat[1][0];

    detTotal = det1 + det2 + det3 - (det4 + det5 + det6);
    return detTotal;

}




float MatrixTrace(float mat[3][3]) {
    return mat[0][0] + mat[1][1] + mat[2][2];
}


void MatrixTranspose(float mat[3][3], float result[3][3]) {
    int x, y;

    for (x = 0; x <= 2; x++) {
        for (y = 0; y <= 2; y++) {
            result[y][x] = mat[x][y];
        }
    }
}


void MatrixInverse(float mat[3][3], float result[3][3]) {
    
   
    result[0][0] = (mat[1][1] * mat[2][2] - (mat[1][2] * mat[2][1]));
    result[0][1] = -((mat[0][1] * mat[2][2]) - (mat[0][2] * mat[2][1]));
    result[0][2] = ((mat[0][1] * mat[1][2]) - (mat[0][2] * mat[1][1]));
    result[1][0] = -((mat[1][0] * mat[2][2]) - (mat[1][2] * mat[2][0]));
    result[1][1] = ((mat[0][0] * mat[2][2]) - (mat[0][2] * mat[2][0]));
    result[1][2] = -((mat[0][0] * mat[1][2]) - (mat[0][2] * mat[1][0]));
    result[2][0] = ((mat[1][0] * mat[2][1]) - (mat[1][1] * mat[2][0]));
    result[2][1] = -((mat[0][0] * mat[2][1]) - (mat[0][1] * mat[2][0]));
    result[2][2] = ((mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]));


    MatrixScalarMultiply(1 / MatrixDeterminant(mat), result, result);

}



void MatrixPrint(float mat[3][3]) {
    int a, b;
    printf("\n");
    printf("\nOutput of MatrixPrint(): ");
    printf("\n ____________________\n");
    for (a = 0; a < 3; a++) {
        printf("|");
        for (b = 0; b < 3; b++) {
            printf(" %.2f |", mat[a][b]);
        }
        printf("\n");
        if (a < 2) {
            printf(" ---------------------\n");
        }
    }
    printf(" --------------------\n");
}
