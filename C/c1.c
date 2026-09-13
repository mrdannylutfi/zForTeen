void multiply_optimized(int *a, int *b, int *result) {
    // 1. Initialize result matrix to 0 first
    for (int i = 0; i < SIZE * SIZE; i++) *(result + i) = 0;

    // 2. IKJ Loop structure for cache locality
    for (int i = 0; i < SIZE; i++) {
        for (int k = 0; k < SIZE; k++) {
            int a_val = *(a + i * SIZE + k); // Cached row element
            for (int j = 0; j < SIZE; j++) {
                *(result + i * SIZE + j) += a_val * *(b + k * SIZE + j);
            }
        }
    }
}
