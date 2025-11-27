#include <stdio.h>
#include <omp.h>
#include <unistd.h>

void process_item(int id) {
    int tid = omp_get_thread_num();
    printf("Thread %d: Processing item %d...\n", tid, id);
    usleep(200000);  // 200ms
    printf("Thread %d: Finished item %d\n", tid, id);
}

int main() {
    printf("=== TASKS: Dynamic Task Creation ===\n\n");

    // ============================================================
    // Example 1: Creating tasks in a loop
    // ============================================================
    printf("--- Creating tasks dynamically in a loop ---\n");

    int items[] = {10, 20, 30, 40, 50, 60};
    int n = 6;

    double start = omp_get_wtime();

    #pragma omp parallel num_threads(3)
    {
        // Only one thread creates the tasks (to avoid duplicates)
        #pragma omp single
        {
            printf("Thread %d: Creating %d tasks...\n\n",
                   omp_get_thread_num(), n);

            for (int i = 0; i < n; i++) {
                #pragma omp task
                {
                    process_item(items[i]);
                }
            }

            printf("\nThread %d: All tasks created, waiting for completion...\n",
                   omp_get_thread_num());

            // Wait for all tasks to complete
            #pragma omp taskwait

            printf("Thread %d: All tasks completed!\n",
                   omp_get_thread_num());
        }
    }

    double elapsed = omp_get_wtime() - start;

    printf("\n=== RESULTS ===\n");
    printf("Processed %d items in %.3f seconds\n", n, elapsed);
    printf("With 3 threads working in parallel\n");
    printf("Sequential would take: %.3f seconds\n", n * 0.2);
    printf("Speedup: %.2fx\n", (n * 0.2) / elapsed);

    return 0;
}
