#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define SIZE 8

void print_array(int arr[SIZE], const char* label) {
    printf("%s: ", label);
    for (int i = 0; i < SIZE; i++) {
        printf("%3d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int original[SIZE] = {100, 10, 90, 20, 80, 30, 70, 40};
    int smoothed[SIZE];
    int iterations = 3;

    printf("=== PARALLEL SMOOTHING (each value = average of neighbors) ===\n\n");
    print_array(original, "Original");
    printf("\n");

    // Copy to smoothed for first iteration
    for (int i = 0; i < SIZE; i++) {
        smoothed[i] = original[i];
    }

    for (int iter = 0; iter < iterations; iter++) {
        printf("--- Iteration %d ---\n", iter + 1);

        #pragma omp parallel num_threads(4)
        {
            int tid = omp_get_thread_num();

            // PHASE 1: Each thread copies its portion from smoothed to original
            // (so we read from a consistent state)
            #pragma omp for
            for (int i = 0; i < SIZE; i++) {
                original[i] = smoothed[i];
            }
            // IMPLICIT BARRIER after 'omp for' - all threads finish copying

            printf("  Thread %d finished copying\n", tid);

            // PHASE 2: Compute new smoothed values
            // Each cell = average of (left neighbor, self, right neighbor)
            #pragma omp for
            for (int i = 0; i < SIZE; i++) {
                int left = (i > 0) ? original[i-1] : original[i];
                int right = (i < SIZE-1) ? original[i+1] : original[i];

                // CRITICAL: We MUST read from 'original' which is stable
                // If we read from 'smoothed' while others are writing to it,
                // we get race conditions and wrong results!
                smoothed[i] = (left + original[i] + right) / 3;

                printf("  Thread %d: smoothed[%d] = (%d + %d + %d) / 3 = %d\n",
                       tid, i, left, original[i], right, smoothed[i]);
            }
            // IMPLICIT BARRIER - all threads finish smoothing before next iteration
        }

        print_array(smoothed, "After iter");
        printf("\n");
    }

    printf("=== FINAL RESULT ===\n");
    print_array(original, "Original");
    print_array(smoothed, "Smoothed");

    return 0;
}
