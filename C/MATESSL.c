#pragma options(argparse, ccode(1047))
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Include the native IBM ESSL header for mathematical subroutines */
#include <essl.h>

/* Box-Muller transform to generate standard normal random variables N(0,1) */
double randn() {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    if (u1 < 1e-15) u1 = 1e-15; 
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

int main(int argc, char *argv[]) {
    /* 
     * Note: ESSL is written in Fortran, which expects column-major array layouts.
     * We define our matrices to align with this structural constraint.
     */
    double A[3][3] = {
        {2.0,  0.0, -2.0},  /* Column 1 */
        {-1.0, 4.0,  1.0},  /* Column 2 */
        {3.0,  1.0,  5.0}   /* Column 3 */
    };
    
    double B[3][3];
    double C[3][3] = {0};

    /* Generate Random Matrix B (Standard Normal Distribution) */
    srand(42); 
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            B[i][j] = randn();
        }
    }

    /* ----------------------------------------------------------------- */
    /* ESSL DGEMM Parameter Setup                                        */
    /* ----------------------------------------------------------------- */
    char   transa = 'N';    /* No transpose for Matrix A                        */
    char   transb = 'N';    /* No transpose for Matrix B                        */
    int    m      = 3;      /* Number of rows in Matrix A and C                 */
    int    n      = 3;      /* Number of columns in Matrix B and C              */
    int    k      = 3;      /* Number of columns in A / rows in B               */
    double alpha  = 1.0;    /* Scaling factor multiplier for A*B                */
    double beta   = 0.0;    /* Scaling factor multiplier for initial Matrix C   */
    int    lda    = 3;      /* Leading dimension of Array A                     */
    int    ldb    = 3;      /* Leading dimension of Array B                     */
    int    ldc    = 3;      /* Leading dimension of Array C                     */

    /* Call the high-performance ESSL Matrix Multiplication Subroutine */
    dgemm(&transa, &transb, &m, &n, &k, &alpha, (double *)A, &lda, 
          (double *)B, &ldb, &beta, (double *)C, &ldc);

    /* ----------------------------------------------------------------- */
    /* Write Outputs Directly into MVS Partitioned Data Set Member       */
    /* ----------------------------------------------------------------- */
    const char *pds_member = "//'USER.REXX.LIB(MATOUT)'";
    FILE *rexxFile = fopen(pds_member, "w, recfm=fb, lrecl=80, blksize=27920");
    
    if (rexxFile == NULL) {
        perror("MVS Open Failure: Unable to locate target PDS library");
        return 8;
    }

    fprintf(rexxFile, "/* REXX - ESSL Accelerated Matrix Output */\n");
    fprintf(rexxFile, "matrix.rows = 3\nmatrix.cols = 3\n");
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            /* Adjust indexes back to 1-indexed format for REXX output compatibility */
            fprintf(rexxFile, "matrix.C.%d.%d = %11.6f\n", i + 1, j + 1, C[j][i]);
        }
    }
    
    fprintf(rexxFile, "say 'Matrix loaded successfully via ESSL execution layer.'\n");
    fprintf(rexxFile, "return 0\n");
    fclose(rexxFile);

    printf("ESSL Computation and PDS update completed successfully.\n");
    return 0;
}
