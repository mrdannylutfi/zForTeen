import java.util.Random;

public class MatrixSim {
    public static void main(String[] args) {
        // Fixed Matrix A
        double[][] a = {
            {2.0, -1.0,  3.0},
            {0.0,  4.0,  1.0},
            {-2.0, 1.0,  5.0}
        };

        // Random Matrix B using Standard Gaussian N(0,1)
        double[][] b = new double[3][3];
        Random rand = new Random(42); // Consistent seed

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                b[i][j] = rand.nextGaussian(); // Generates standard normal distribution
            }
        }

        // Multiply C = A * B
        double[][] c = new double[3][3];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }

        // Calculate Determinant of B
        double detB = b[0][0]*(b[1][1]*b[2][2] - b[1][2]*b[2][1]) -
                      b[0][1]*(b[1][0]*b[2][2] - b[1][2]*b[2][0]) +
                      b[0][2]*(b[1][0]*b[2][1] - b[1][1]*b[2][0]);

        // Output Results
        System.out.println("--- IBM Z Semeru Java Simulation ---");
        System.out.println("Resulting Matrix C:");
        for (int i = 0; i < 3; i++) {
            System.out.printf("[ %7.4f  %7.4f  %7.4f ]\n", c[i][0], c[i][1], c[i][2]);
        }
        System.out.printf("\nDeterminant of Random Matrix B: %7.4f\n", detB);
        System.out.println("Status: " + ((Math.abs(detB) > 1e-5) ? "Full Rank (3) & Invertible" : "Singular"));
    }
}
