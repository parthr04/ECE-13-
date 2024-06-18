// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
//CMPE13 Support Library
#include "BOARD.h"


// Microchip libraries
#include <xc.h>

// User libraries
#include "MatrixMath.h"


int main()
{
    BOARD_Init();

    float result[3][3] = {};
    float result1[3][3] = {};
    float result2[3][3] = {};
    int total = 0;



    float multTest1[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    float multAnswer1[3][3] = {
        {3, 3, 3},
        {3, 3, 3},
        {3, 3, 3}
    };


    float multTest2[3][3] = {
        {2, 2, 2},
        {2, 2, 2},
        {2, 2, 2}
    };


    float multAnswer2[3][3] = {
        {12, 12, 12},
        {12, 12, 12},
        {12, 12, 12}
    };


    int multPass = 0; 
    MatrixMultiply(multTest2, multTest2, result);
    if (MatrixEquals(multAnswer2, result) == 1) {
        multPass++;
    }
  
    MatrixMultiply(multTest1, multTest1, result1); 
    if (MatrixEquals(multAnswer1, result1) == 1) {
        multPass++;
    }
    if (multPass == 2) {
        total++;
    }
    printf("\nPASSED (%d/2) : MatrixMultiply()", multPass);


    float addTest1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    float addAnswer1[3][3] = {
        {2, 4, 6},
        {8, 10, 12},
        {14, 16, 18}
    };


    float addTest2[3][3] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };


    float addAnswer2[3][3] = {
        {18, 16, 14},
        {12, 10, 8},
        {6, 4, 2}
    };



    int matAddPass = 0; 
    MatrixAdd(addTest1, addTest1, result);
    if (MatrixEquals(addAnswer1, result) == 1) {
        matAddPass++;
    }
    MatrixAdd(addTest2, addTest2, result);
    if (MatrixEquals(addAnswer2, result) == 1) {
        matAddPass++;
    }
    if (matAddPass == 2) {
        total++;
    }
    printf("\nPASSED (%d/2) : MatrixAdd()", matAddPass);



    int equalPass = 0; 
    if (MatrixEquals(multTest1, multTest1) == 1) {
        equalPass++;
    }
    if (MatrixEquals(multTest1, multTest2) == 0) {
        equalPass++;
    }
    if (equalPass == 2) {
        total++;
    }
    printf("\nPASSED (%d/2) : MatrixEquals()", equalPass);



    float multScalartest1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    float multScalarAnswer1[3][3] = {
        {2, 4, 6},
        {8, 10, 12},
        {14, 16, 18}
    };


    float multScalartest2[3][3] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };


    float multScalarAnswer2[3][3] = {
        {18, 16, 14},
        {12, 10, 8},
        {6, 4, 2}
    };

    int scMultPass = 0; 
    MatrixScalarMultiply(2, multScalartest1, result);
    if (MatrixEquals(multScalarAnswer1, result) == 1) {
        scMultPass++;
    }
    MatrixScalarMultiply(2, multScalartest2, result);
    if (MatrixEquals(multScalarAnswer2, result) == 1) {
        scMultPass++;
    }
    if (scMultPass == 2) {
        total++;
    }
    printf("\nPASSED (%d/2) : MatrixScalarMultiply()", scMultPass);



    float addScalartest1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    float addScalarAnswer1[3][3] = {
        {2, 3, 4},
        {5, 6, 7},
        {8, 9, 10}
    };


    float addScalartest2[3][3] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };


    float addScalarAnswer2[3][3] = {
        {10, 9, 8},
        {7, 6, 5},
        {4, 3, 2}
    };

    int scAddPass = 0; 
    MatrixScalarAdd(1, addScalartest1, result);
    if (MatrixEquals(addScalarAnswer1, result) == 1) {
        scAddPass++;
    }
    MatrixScalarAdd(1, addScalartest2, result);
    if (MatrixEquals(addScalarAnswer2, result) == 1) {
        scAddPass++;
    }
    if (scAddPass == 2) {
        total++;
    }
    printf("\nPASSED (%d/2) : MatrixScalarAdd()", scAddPass);


    int detPass = 0; 
    
    
    float addScalartest3[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    if ((MatrixDeterminant(addScalartest3)) == 0 ) {
        detPass++;
    }
    
    
    
    if ( MatrixDeterminant(addScalartest2) == 0) {
        detPass++;
    }
    if (detPass == 2) {
        total++;
    }
    printf("\nPASSED (%d/2) : MatrixDeterminant()", detPass);



    float tranposeT1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    float tranposeA1[3][3] = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}
    };


    float tranposeT2[3][3] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };


    float tranposeA2[3][3] = {
        {9, 6, 3},
        {8, 5, 2},
        {7, 4, 1}
    };


    int matTranPass = 0; 
    MatrixTranspose(tranposeT1, result);
    if (MatrixEquals(tranposeA1, result) == 1) {
        matTranPass++;
    }
    MatrixTranspose(tranposeT2, result);
    if (MatrixEquals(tranposeA2, result) == 1) {
        matTranPass++;
    }
    if (matTranPass == 2) {
        total++;
    }
    printf("\nPASSED (%d/2) : MatrixTranspose()", matTranPass);




    float testMat4 [3][3] = {
        {1, 2, 3},
        {1, 0, 1},
        {0, 0, 1}
    };
    
    float testMat5 [3][3] = {
        {9, 2, 3},
        {1, 0, 1},
        {0, 0, 6}
    };

    int matTracePass = 0; 

     if (MatrixTrace(testMat4) - 2 < FP_DELTA) {
        matTracePass++;
    }


     if (MatrixTrace(testMat5) - 15 < FP_DELTA) {
        matTracePass++;
    }
    if (matTracePass == 2) {
        total++;
    }
    printf("\nPASSED (%d/2) : MatrixTrace()", matTracePass);



    float invMat1 [3][3] = {
        {0, 2, 2},
        {1, 1, 1},
        {0, 1, 2}
    };

    float invAns1 [3][3] = {
        {-.50, 1.00, 0.00},
        {1.00, 0.00, -1.00},
        {-.50, 0.00, 1.00}
    };
    
    
    float invMat2 [3][3] = {
        {3, 0, 2},
        {2, 0, -2},
        {0, 1, 1}
    };

    float invAns2 [3][3] = {
        {.20, .20, 0.0},
        {-.20, .30, 1.0},
        {.2, -.3, 0.0}
    };


    int invPass = 0; 

    MatrixInverse(invMat1, result);
    if (MatrixEquals(invAns1, result) == 1) {
        invPass++;
    }


    MatrixInverse(invMat2, result);
    if (MatrixEquals(invAns2, result) == 1) {
        invPass++;
    }

    if (matTracePass == 2) {
        total++;
    }
    printf("\nPASSED (%d/2) : MatrixInverse()", invPass);
       
 

    float passPercent = (total) / (9.0)*(100.0);
    printf("\n\n%d out of 9 functions passes (%.1f%%)", total, (double) passPercent);
    
   
    float matPrint[3][3] = {
        {1.3, 2.2, 3.3},
        {4.6, 6.0, 6.6},
        {7.9, 8.7, 9.9}
    };

    printf("\n");
    printf("\nExpected output of MatrixPrint():");
    printf("\n ____________________");
    printf("\n| 1.30 | 2.20 | 3.30 |");
    printf("\n --------------------");
    printf("\n| 4.6  | 6.0  | 6.60 |");
    printf("\n --------------------");
    printf("\n| 7.90 | 8.70 | 9.90 |");
    printf("\n --------------------");
    MatrixPrint(matPrint);
    printf("\n");
   
    while (1);
}

