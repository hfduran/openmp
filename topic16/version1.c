#include <stdio.h>
#include <omp.h>
#include <unistd.h>

int main() {
    printf("=== NOWAIT CLAUSE ===\n\n");

    int arrayA[8] = {0};
    int arrayB[8] = {0};

    // ============================================================
    // WITHOUT nowait: Threads wait at barrier (slower)
    // ============================================================
    printf("--- WITHOUT nowait (with implicit barrier) ---\n");

    double start = omp_get_wtime();

    #pragma omp parallel num_threads(4)
    {
        int tid = omp_get_thread_num();

        // Process arrayA
        #pragma omp for
        for (int i = 0; i < 8; i++) {
            printf("Thread %d: processing arrayA[%d]\n", tid, i);
            usleep(100000);  // 100ms of work
            arrayA[i] = i * 10;
        }
        // IMPLICIT BARRIER - all threads wait here

        printf("Thread %d: passed barrier, continuing to arrayB\n", tid);

        // Process arrayB (independent of arrayA)
        #pragma omp for
        for (int i = 0; i < 8; i++) {
            printf("Thread %d: processing arrayB[%d]\n", tid, i);
            usleep(100000);  // 100ms of work
            arrayB[i] = i * 20;
        }
    }

    double time_with_barrier = omp_get_wtime() - start;
    printf("Time WITH barrier: %.3f seconds\n\n", time_with_barrier);

    // Reset arrays
    for (int i = 0; i < 8; i++) {
        arrayA[i] = 0;
        arrayB[i] = 0;
    }

    // ============================================================
    // WITH nowait: No barrier, threads continue immediately (faster)
    // ============================================================
    printf("--- WITH nowait (no barrier) ---\n");

    start = omp_get_wtime();

    #pragma omp parallel num_threads(4)
    {
        int tid = omp_get_thread_num();

        // Process arrayA
        #pragma omp for nowait
        for (int i = 0; i < 8; i++) {
            printf("Thread %d: processing arrayA[%d]\n", tid, i);
            usleep(100000);  // 100ms of work
            arrayA[i] = i * 10;
        }
        // NO BARRIER - threads continue immediately!

        printf("Thread %d: NO barrier, continuing immediately to arrayB\n", tid);

        // Process arrayB (independent of arrayA)
        #pragma omp for
        for (int i = 0; i < 8; i++) {
            printf("Thread %d: processing arrayB[%d]\n", tid, i);
            usleep(100000);  // 100ms of work
            arrayB[i] = i * 20;
        }
    }

    double time_no_barrier = omp_get_wtime() - start;
    printf("Time WITHOUT barrier (nowait): %.3f seconds\n\n", time_no_barrier);

    // ============================================================
    // COMPARISON
    // ============================================================
    printf("=== PERFORMANCE COMPARISON ===\n");
    printf("With barrier:    %.3f seconds\n", time_with_barrier);
    printf("With nowait:     %.3f seconds\n", time_no_barrier);
    printf("Speedup:         %.2fx faster\n",
           time_with_barrier / time_no_barrier);

    return 0;
}
