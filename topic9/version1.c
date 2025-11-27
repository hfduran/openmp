#include <stdio.h>
#include <omp.h>
#include <time.h>

int main() {
    int iterations = 1000000;
    int counter_atomic = 0;
    int counter_critical = 0;

    // Benchmark ATOMIC
    double start = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < iterations; i++) {
        #pragma omp atomic
        counter_atomic++;
    }
    double time_atomic = omp_get_wtime() - start;

    // Benchmark CRITICAL
    start = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < iterations; i++) {
        #pragma omp critical
        {
            counter_critical++;
        }
    }
    double time_critical = omp_get_wtime() - start;

    printf("=== PERFORMANCE COMPARISON ===\n\n");

    printf("ATOMIC:\n");
    printf("  Counter: %d / %d\n", counter_atomic, iterations);
    printf("  Time: %.4f seconds\n\n", time_atomic);

    printf("CRITICAL:\n");
    printf("  Counter: %d / %d\n", counter_critical, iterations);
    printf("  Time: %.4f seconds\n\n", time_critical);

    printf("Speedup: %.2fx faster with atomic!\n", time_critical / time_atomic);

    return 0;
}
