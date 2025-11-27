#include <stdio.h>
#include <omp.h>
#include <unistd.h>

int main() {
    printf("Demonstrating barrier synchronization\n");
    printf("======================================\n\n");

    #pragma omp parallel num_threads(4)
    {
        int tid = omp_get_thread_num();

        // PHASE 1: Different threads take different time
        printf("[Phase 1] Thread %d starting work...\n", tid);
        sleep(tid);  // Thread 0 sleeps 0s, Thread 1 sleeps 1s, etc.
        printf("[Phase 1] Thread %d FINISHED work (slept %d seconds)\n", tid, tid);

        // BARRIER: Everyone waits here until all threads finish Phase 1
        #pragma omp barrier

        // PHASE 2: Only starts when ALL threads finished Phase 1
        printf("[Phase 2] Thread %d starting (all threads finished Phase 1!)\n", tid);

        #pragma omp barrier

        // PHASE 3: Final phase
        if (tid == 0) {
            printf("\n[Phase 3] All threads synchronized! Proceeding together.\n");
        }
    }

    printf("\nAll threads finished!\n");

    return 0;
}
