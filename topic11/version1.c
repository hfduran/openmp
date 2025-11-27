#include <stdio.h>
#include <omp.h>

int main() {
    int shared_data = 0;

    printf("=== Demonstrating 'single' directive ===\n\n");

    #pragma omp parallel num_threads(4)
    {
        int tid = omp_get_thread_num();

        // PHASE 1: All threads do work
        printf("Thread %d: Working in parallel...\n", tid);

        // Only ONE thread initializes shared data
        #pragma omp single
        {
            int executor = omp_get_thread_num();
            printf("\n>>> Thread %d: I'm the ONE initializing shared_data\n", executor);
            shared_data = 100;
            printf(">>> Thread %d: shared_data = %d\n\n", executor, shared_data);
        }
        // IMPLICIT BARRIER here - all threads wait

        // PHASE 2: All threads can now use the initialized data
        printf("Thread %d: shared_data is ready, value = %d\n", tid, shared_data);

        #pragma omp barrier

        // Another single block
        #pragma omp single
        {
            printf("\n>>> Thread %d: Printing final status (only me!)\n",
                   omp_get_thread_num());
        }

        printf("Thread %d: Finishing\n", tid);
    }

    printf("\nAll done!\n");
    return 0;
}
