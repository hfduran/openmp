#include <stdio.h>
#include <omp.h>
#include <unistd.h>

// Simulates variable workload - iteration i takes i microseconds
void variable_work(int i) {
    usleep(i * 1000);  // Sleep for i milliseconds
}

int main() {
    int n = 16;

    printf("=== SCHEDULE COMPARISON ===\n");
    printf("Workload: iteration i takes %d ms (highly imbalanced!)\n\n", 0);

    // ============================================================
    // STATIC: Iterations divided equally at compile time
    // ============================================================
    printf("--- STATIC SCHEDULE ---\n");
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(static) num_threads(4)
    for (int i = 0; i < n; i++) {
        int tid = omp_get_thread_num();
        printf("Thread %d: iteration %d (will take %d ms)\n", tid, i, i);
        variable_work(i);
    }

    double time_static = omp_get_wtime() - start;
    printf("Time: %.3f seconds\n\n", time_static);

    // ============================================================
    // DYNAMIC: Iterations assigned on-demand
    // ============================================================
    printf("--- DYNAMIC SCHEDULE ---\n");
    start = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic) num_threads(4)
    for (int i = 0; i < n; i++) {
        int tid = omp_get_thread_num();
        printf("Thread %d: iteration %d (will take %d ms)\n", tid, i, i);
        variable_work(i);
    }

    double time_dynamic = omp_get_wtime() - start;
    printf("Time: %.3f seconds\n\n", time_dynamic);

    // ============================================================
    // GUIDED: Adaptive chunk sizes
    // ============================================================
    printf("--- GUIDED SCHEDULE ---\n");
    start = omp_get_wtime();

    #pragma omp parallel for schedule(guided) num_threads(4)
    for (int i = 0; i < n; i++) {
        int tid = omp_get_thread_num();
        printf("Thread %d: iteration %d (will take %d ms)\n", tid, i, i);
        variable_work(i);
    }

    double time_guided = omp_get_wtime() - start;
    printf("Time: %.3f seconds\n\n", time_guided);

    // ============================================================
    // RESULTS
    // ============================================================
    printf("=== PERFORMANCE COMPARISON ===\n");
    printf("Static:  %.3f seconds\n", time_static);
    printf("Dynamic: %.3f seconds (%.1fx faster)\n",
           time_dynamic, time_static / time_dynamic);
    printf("Guided:  %.3f seconds (%.1fx faster)\n",
           time_guided, time_static / time_guided);

    return 0;
}
