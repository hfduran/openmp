#include <stdio.h>
#include <omp.h>

#define ROWS 4
#define COLS 6

int main() {
    int matrix[ROWS][COLS];
    int count_without = 0;
    int count_with = 0;

    printf("=== LOOP COLLAPSE ===\n");
    printf("Matrix size: %d × %d = %d total iterations\n\n", ROWS, COLS, ROWS * COLS);

    // ============================================================
    // WITHOUT collapse: Only outer loop parallelized
    // ============================================================
    printf("--- WITHOUT collapse (only outer loop parallel) ---\n");

    #pragma omp parallel for num_threads(4)
    for (int i = 0; i < ROWS; i++) {
        int tid = omp_get_thread_num();

        for (int j = 0; j < COLS; j++) {
            matrix[i][j] = i * 10 + j;

            #pragma omp atomic
            count_without++;

            printf("Thread %d: matrix[%d][%d] = %d\n", tid, i, j, matrix[i][j]);
        }
    }

    printf("\nLoad distribution WITHOUT collapse:\n");
    printf("- Only %d outer iterations (i=0 to %d)\n", ROWS, ROWS-1);
    printf("- With 4 threads: each thread gets 1 outer iteration\n");
    printf("- Inner loop (j) runs sequentially within each thread\n\n");

    // Reset matrix
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            matrix[i][j] = 0;

    // ============================================================
    // WITH collapse(2): Both loops parallelized together
    // ============================================================
    printf("--- WITH collapse(2) (both loops parallel) ---\n");

    #pragma omp parallel for collapse(2) num_threads(4)
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int tid = omp_get_thread_num();
            matrix[i][j] = i * 10 + j;

            #pragma omp atomic
            count_with++;

            printf("Thread %d: matrix[%d][%d] = %d\n", tid, i, j, matrix[i][j]);
        }
    }

    printf("\nLoad distribution WITH collapse(2):\n");
    printf("- Flattened to %d total iterations\n", ROWS * COLS);
    printf("- With 4 threads: each gets ~%d iterations\n", (ROWS * COLS) / 4);
    printf("- Better load balancing across all iterations\n\n");

    // ============================================================
    // COMPARISON
    // ============================================================
    printf("=== SUMMARY ===\n");
    printf("WITHOUT collapse: 4 threads, 4 outer iterations → 1 iteration per thread\n");
    printf("                  Each thread does 6 inner iterations sequentially\n\n");
    printf("WITH collapse(2): 4 threads, 24 total iterations → ~6 iterations per thread\n");
    printf("                  Better parallelization and load balance!\n");

    return 0;
}
