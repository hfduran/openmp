#include <stdio.h>
#include <omp.h>
#include <unistd.h>

int main() {
    printf("=== Demonstrating 'master' directive ===\n\n");

    #pragma omp parallel num_threads(4)
    {
        int tid = omp_get_thread_num();

        printf("Thread %d: Starting work\n", tid);

        // Only master thread (thread 0) executes this
        #pragma omp master
        {
            printf("\n>>> MASTER (thread 0): Doing special master work...\n");
            sleep(2);  // Simulate slow work
            printf(">>> MASTER (thread 0): Finished master work\n\n");
        }
        // NO BARRIER - other threads don't wait!

        printf("Thread %d: Continuing (didn't wait for master)\n", tid);

        #pragma omp barrier  // Explicit barrier if we want all to sync

        printf("Thread %d: Now all synchronized after barrier\n", tid);
    }

    printf("\nDone!\n");
    return 0;
}
