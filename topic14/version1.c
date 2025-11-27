#include <stdio.h>
#include <omp.h>

int main() {
    printf("=== CONTROLLING NUMBER OF THREADS ===\n\n");

    // Check system default
    printf("System default max threads: %d\n\n", omp_get_max_threads());

    // ============================================================
    // Method 1: Using num_threads clause
    // ============================================================
    printf("--- Method 1: num_threads clause ---\n");

    #pragma omp parallel num_threads(2)
    {
        #pragma omp single
        printf("Running with %d threads\n", omp_get_num_threads());

        printf("  Thread %d: Hello!\n", omp_get_thread_num());
    }

    printf("\n");

    // ============================================================
    // Method 2: Using omp_set_num_threads()
    // ============================================================
    printf("--- Method 2: omp_set_num_threads() ---\n");

    omp_set_num_threads(3);

    #pragma omp parallel
    {
        #pragma omp single
        printf("Running with %d threads\n", omp_get_num_threads());

        printf("  Thread %d: Hello!\n", omp_get_thread_num());
    }

    printf("\n");

    // ============================================================
    // Method 3: Clause overrides function
    // ============================================================
    printf("--- Clause overrides function ---\n");
    printf("(omp_set_num_threads(3) is still set, but clause wins)\n");

    #pragma omp parallel num_threads(5)
    {
        #pragma omp single
        printf("Running with %d threads\n", omp_get_num_threads());

        printf("  Thread %d: Hello!\n", omp_get_thread_num());
    }

    printf("\n");

    // ============================================================
    // Different regions can have different thread counts
    // ============================================================
    printf("--- Different regions, different counts ---\n");

    #pragma omp parallel num_threads(1)
    {
        printf("Serial region: %d thread(s)\n", omp_get_num_threads());
    }

    #pragma omp parallel num_threads(8)
    {
        #pragma omp single
        printf("Parallel region: %d thread(s)\n", omp_get_num_threads());
    }

    return 0;
}
