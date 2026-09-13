import java.util.Random;

public class MatrixMultiplication {
    public static void main(String[] args) {
        int size = 3;
        Random rand = new Random();

        // Define fixed 3x3 matrix A
        double[][] a = {
            {1.0, 2.0, 3.0},
            {4.0, 5.0, 6.0},
            {7.0, 8.0, 9.0}
        };

        // Initialize random 3x3 matrix B and result matrix C
        double[][] b = new double[size][size];
        double[][] c = new double[size][size];

        // Populate Matrix B with random numbers between 1 and 10
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                b[i][j] = 1 + (10 - 1) * rand.nextDouble();
            }
        }

        // Matrix Multiplication (A * B = C)
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                for (int k = 0; k < size; k++) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }

        // Print Results
        System.out.println("Matrix A (Fixed):");
        printMatrix(a);

        System.out.println("\nMatrix B (Random):");
        printMatrix(b);

        System.out.println("\nResult Matrix C (A * B):");
        printMatrix(c);
    }

    private static void printMatrix(double[][] matrix) {
        for (double[] row : matrix) {
            for (double val : row) {
                System.out.printf("%.2f\t", val);
            }
            System.out.println();
        }
    }
}
