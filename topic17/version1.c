#include <stdio.h>
#include <omp.h>

int main() {
    printf("=== NESTED PARALLELISM ===\n\n");

    // ============================================================
    // WITHOUT nested parallelism (default)
    // ============================================================
    printf("--- WITHOUT nested parallelism (default) ---\n");
    printf("Nested is: %s\n\n", omp_get_nested() ? "ENABLED" : "DISABLED");

    #pragma omp parallel num_threads(2)
    {
        int outer_tid = omp_get_thread_num();
        int level = omp_get_level();

        printf("OUTER: Thread %d, Level %d, Team size %d\n",
               outer_tid, level, omp_get_num_threads());

        // Inner parallel region (will run with 1 thread when nested disabled)
        #pragma omp parallel num_threads(3)
        {
            int inner_tid = omp_get_thread_num();
            int inner_level = omp_get_level();

            printf("  INNER: Outer thread %d → Inner thread %d, Level %d, Team size %d\n",
                   outer_tid, inner_tid, inner_level, omp_get_num_threads());
        }
    }

    printf("\n");

    // ============================================================
    // WITH nested parallelism (enabled)
    // ============================================================
    printf("--- WITH nested parallelism (enabled) ---\n");
    omp_set_nested(1);
    printf("Nested is: %s\n\n", omp_get_nested() ? "ENABLED" : "DISABLED");

    #pragma omp parallel num_threads(2)
    {
        int outer_tid = omp_get_thread_num();
        int level = omp_get_level();

        printf("OUTER: Thread %d, Level %d, Team size %d\n",
               outer_tid, level, omp_get_num_threads());

        // Inner parallel region (will spawn 3 threads per outer thread)
        #pragma omp parallel num_threads(3)
        {
            int inner_tid = omp_get_thread_num();
            int inner_level = omp_get_level();

            printf("  INNER: Outer thread %d → Inner thread %d, Level %d, Team size %d\n",
                   outer_tid, inner_tid, inner_level, omp_get_num_threads());
        }

        printf("OUTER: Thread %d finished inner region\n", outer_tid);
    }

    printf("\n");

    // ============================================================
    // SUMMARY
    // ============================================================
    printf("=== SUMMARY ===\n");
    printf("WITHOUT nesting: 2 outer threads × 1 inner thread = 2 total threads\n");
    printf("WITH nesting:    2 outer threads × 3 inner threads = 6 total threads\n");

    return 0;
}
