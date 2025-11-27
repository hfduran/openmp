#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define ROWS 6
#define COLS 4

void print_matrix(double matrix[ROWS][COLS], const char* label) {
    printf("\n%s:\n", label);
    for (int i = 0; i < ROWS; i++) {
        printf("  ");
        for (int j = 0; j < COLS; j++) {
            printf("%8.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_array(double arr[], int size, const char* label) {
    printf("\n%s: ", label);
    for (int i = 0; i < size; i++) {
        printf("%8.2f ", arr[i]);
    }
    printf("\n");
}

int main() {
    double matrix[ROWS][COLS];
    double col_sums[COLS] = {0};
    double normalized[ROWS][COLS];

    printf("=== PARALLEL MATRIX COLUMN NORMALIZATION ===\n");
    printf("Each element will be divided by its column sum\n");

    #pragma omp parallel num_threads(4)
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // ============================================================
        // PHASE 1: Initialize matrix (each thread does some rows)
        // ============================================================
        printf("Thread %d: Starting Phase 1 (initialize matrix)\n", tid);

        #pragma omp for
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                matrix[i][j] = (i + 1) * 10 + (j + 1);
            }
        }
        // IMPLICIT BARRIER HERE - all rows must be initialized
        // before any thread can compute column sums in Phase 2

        printf("Thread %d: Finished Phase 1\n", tid);

        // ============================================================
        // PHASE 2: Compute column sums
        // Each thread handles specific columns
        // ============================================================
        printf("Thread %d: Starting Phase 2 (compute column sums)\n", tid);

        // Manual work distribution: thread handles its columns
        for (int j = tid; j < COLS; j += num_threads) {
            double sum = 0;
            for (int i = 0; i < ROWS; i++) {
                sum += matrix[i][j];
            }
            col_sums[j] = sum;
            printf("Thread %d: col_sums[%d] = %.2f\n", tid, j, sum);
        }

        // *** EXPLICIT BARRIER REQUIRED HERE! ***
        // All threads MUST finish computing column sums
        // before anyone can use them for normalization
        #pragma omp barrier

        printf("Thread %d: Passed barrier after Phase 2\n", tid);

        // ============================================================
        // PHASE 3: Normalize matrix (divide each element by column sum)
        // REQUIRES: col_sums must be complete (guaranteed by barrier)
        // ============================================================
        printf("Thread %d: Starting Phase 3 (normalize matrix)\n", tid);

        #pragma omp for nowait
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                // This would be WRONG without the barrier above!
                // col_sums[j] might not be computed yet
                normalized[i][j] = matrix[i][j] / col_sums[j];
            }
        }

        printf("Thread %d: Finished Phase 3\n", tid);

        // *** EXPLICIT BARRIER REQUIRED HERE! ***
        // Ensure all threads finish before we compute verification
        #pragma omp barrier

        // ============================================================
        // PHASE 4: Verification - compute new column sums (should be 1.0)
        // ============================================================
        #pragma omp for
        for (int j = 0; j < COLS; j++) {
            double sum = 0;
            for (int i = 0; i < ROWS; i++) {
                sum += normalized[i][j];
            }
            printf("Thread %d: Normalized col_sums[%d] = %.4f (should be 1.0)\n",
                   tid, j, sum);
        }

    } // End parallel region

    // Print results
    print_matrix(matrix, "Original Matrix");
    print_array(col_sums, COLS, "Column Sums");
    print_matrix(normalized, "Normalized Matrix (each element / column sum)");

    printf("\n=== VERIFICATION ===\n");
    printf("Each column in normalized matrix should sum to 1.0\n");
    for (int j = 0; j < COLS; j++) {
        double sum = 0;
        for (int i = 0; i < ROWS; i++) {
            sum += normalized[i][j];
        }
        printf("Column %d sum: %.6f\n", j, sum);
    }

    return 0;
}
