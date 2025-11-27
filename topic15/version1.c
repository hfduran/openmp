#include <stdio.h>
#include <omp.h>

int main() {
    printf("=== FIRSTPRIVATE AND LASTPRIVATE ===\n\n");

    // ============================================================
    // FIRSTPRIVATE: Initialized with original value
    // ============================================================
    printf("--- FIRSTPRIVATE ---\n");

    int base_value = 100;
    printf("Original base_value = %d\n\n", base_value);

    #pragma omp parallel for firstprivate(base_value) num_threads(4)
    for (int i = 0; i < 8; i++) {
        int tid = omp_get_thread_num();

        // Each thread starts with base_value = 100
        printf("Thread %d, iter %d: base_value starts at %d\n",
               tid, i, base_value);

        base_value += i;  // Modify private copy

        printf("Thread %d, iter %d: base_value after += %d is %d\n",
               tid, i, i, base_value);
    }

    printf("\nAfter parallel region: base_value = %d (unchanged)\n\n",
           base_value);

    // ============================================================
    // LASTPRIVATE: Final value from last iteration
    // ============================================================
    printf("--- LASTPRIVATE ---\n");

    int last_value = 0;
    printf("Original last_value = %d\n\n", last_value);

    #pragma omp parallel for lastprivate(last_value) num_threads(4)
    for (int i = 0; i < 8; i++) {
        int tid = omp_get_thread_num();

        // Each thread has private copy
        last_value = i * 10;

        printf("Thread %d, iter %d: last_value = %d\n",
               tid, i, last_value);
    }

    printf("\nAfter parallel region: last_value = %d (from last iteration i=7)\n\n",
           last_value);

    // ============================================================
    // COMBINING BOTH: firstprivate + lastprivate
    // ============================================================
    printf("--- BOTH: firstprivate + lastprivate ---\n");

    int combo = 50;
    printf("Original combo = %d\n\n", combo);

    #pragma omp parallel for firstprivate(combo) lastprivate(combo) num_threads(4)
    for (int i = 0; i < 8; i++) {
        int tid = omp_get_thread_num();

        printf("Thread %d, iter %d: combo starts at %d, ",
               tid, i, combo);

        combo = combo + i;  // Start from 50, add iteration number

        printf("becomes %d\n", combo);
    }

    printf("\nAfter parallel region: combo = %d (50 + 7 from last iteration)\n",
           combo);

    return 0;
}
