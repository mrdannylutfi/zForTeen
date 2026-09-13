#pragma options(argparse, ccode(1047))
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Box-Muller transform for standard normal variables */
double randn() {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    if (u1 < 1e-15) u1 = 1e-15; 
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

int main(int argc, char *argv[]) {
    double A[3][3] = {{2.0, -1.0, 3.0}, {0.0, 4.0, 1.0}, {-2.0, 1.0, 5.0}};
    double B[3][3], C[3][3] = {0};
    
    srand(42); 
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            B[i][j] = randn();

    /* Matrix multiplication C = A * B */
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                C[i][j] += A[i][k] * B[k][j];

    /* MVS Native File I/O: Specifying exact dataset organization attributes */
    const char *pds_member = "//'USER.REXX.LIB(MATOUT)'";
    FILE *rexxFile = fopen(pds_member, "w, recfm=fb, lrecl=80, blksize=27920");
    
    if (rexxFile == NULL) {
        perror("ERROR: Could not open target PDS/E member");
        return 1;
    }

    /* Injecting Advanced REXX Properties & Architecture Requirements */
    fprintf(rexxFile, "/* REXX - Production Matrix Output */\n");
    fprintf(rexxFile, "signal on novalue\n"); /* Trap uninitialized variables */
    fprintf(rexxFile, "numeric digits 12\n"); /* Enforce high floating precision */
    fprintf(rexxFile, "parse arg mode .\n\n"); /* Accept execution arguments */
    
    fprintf(rexxFile, "matrix.rows = 3\nmatrix.cols = 3\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            fprintf(rexxFile, "matrix.C.%d.%d = %11.6f\n", i + 1, j + 1, C[i][j]);
        }
    }

    /* Dynamic runtime evaluation inside REXX based on user arguments */
    fprintf(rexxFile, "\nif mode = 'VERBOSE' then do\n");
    fprintf(rexxFile, "  say '>>> Matrix execution complete. Outputting Stem: '\n");
    fprintf(rexxFile, "  do i = 1 to matrix.rows\n");
    fprintf(rexxFile, "    row_str = '[ '\n");
    fprintf(rexxFile, "    do j = 1 to matrix.cols\n");
    fprintf(rexxFile, "      row_str = row_str || format(matrix.C.i.j, 4, 4) || '  '\n");
    fprintf(rexxFile, "    end\n");
    fprintf(rexxFile, "    say row_str || ']'\n");
    fprintf(rexxFile, "  end\n");
    fprintf(rexxFile, "end\n");
    fprintf(rexxFile, "exit 0\n");
    fprintf(rexxFile, "novalue: say 'ERROR: Uninitialized REXX variable detected!'; exit 12\n");

    fclose(rexxFile);
    return 0;
}
