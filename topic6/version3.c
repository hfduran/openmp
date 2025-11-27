#include <stdio.h>
#include <omp.h>

int main() {
    int outside_var = 100;  // DEFAULT: SHARED (declared outside)

    printf("Before parallel region:\n");
    printf("  outside_var = %d\n\n", outside_var);

    #pragma omp parallel for
    for (int i = 0; i < 8; i++) {  // 'i' is DEFAULT: PRIVATE
        int inside_var = 999;      // DEFAULT: PRIVATE (declared inside)
        int thread_id = omp_get_thread_num();

        // Modify the inside variable (each thread has its own copy)
        inside_var = thread_id * 100;

        // Modify the outside variable (SHARED - race condition!)
        outside_var++;

        printf("Thread %d: i=%d, inside_var=%d, outside_var=%d\n",
               thread_id, i, inside_var, outside_var);
    }

    printf("\nAfter parallel region:\n");
    printf("  outside_var = %d (should be 108, but race condition!)\n",
           outside_var);

    return 0;
}
