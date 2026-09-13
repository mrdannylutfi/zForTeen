#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Box-Muller transform to generate standard normal random variables N(0,1)
double randn() {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    // Prevent log(0)
    if (u1 < 1e-15) u1 = 1e-15; 
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

int main() {
    // Fixed Matrix A
    double A[3][3] = {
        {2.0, -1.0,  3.0},
        {0.0,  4.0,  1.0},
        {-2.0, 1.0,  5.0}
    };
    
    // Generate Random Matrix B (Standard Normal)
    double B[3][3];
    srand(42); // Seed for replication
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            B[i][j] = randn();
        }
    }

    // Initialize and Compute C = A * B
    double C[3][3] = {0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Simple Determinant calculation for 3x3 B to prove invertibility
    double det_B = B[0][0]*(B[1][1]*B[2][2] - B[1][2]*B[2][1]) -
                   B[0][1]*(B[1][0]*B[2][2] - B[1][2]*B[2][0]) +
                   B[0][2]*(B[1][0]*B[2][1] - B[1][1]*B[2][0]);

    // Print Outputs
    printf("--- IBM z14 C Simulation ---\nResulting Matrix C:\n");
    for (int i = 0; i < 3; i++) {
        printf("[ %7.4f  %7.4f  %7.4f ]\n", C[i][0], C[i][1], C[i][2]);
    }
    printf("\nDeterminant of Random Matrix B: %7.4f\n", det_B);
    printf("Status: %s\n", (fabs(det_B) > 1e-5) ? "Full Rank (3) & Invertible" : "Singular");

    return 0;
}
