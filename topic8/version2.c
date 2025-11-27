#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n = 1000000;
    int *array = malloc(n * sizeof(int));

    // Initialize array with random values
    srand(42);
    for (int i = 0; i < n; i++) {
        array[i] = rand() % 10000;
    }
    // Plant a known maximum
    array[777777] = 99999;

    // We need to track BOTH max value AND its index
    // Can't use reduction for this - need critical!
    int max_value = -1;
    int max_index = -1;
    int checks = 0;  // Count how many times we update

    printf("Finding maximum value and its index in array of %d elements...\n\n", n);

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int local_value = array[i];

        // Check if this might be the max (outside critical - fast check)
        if (local_value > max_value) {
            // Need to update BOTH max_value AND max_index together
            // This requires critical section!
            #pragma omp critical
            {
                // Re-check inside critical (another thread might have updated)
                if (local_value > max_value) {
                    max_value = local_value;
                    max_index = i;
                    checks++;

                    int tid = omp_get_thread_num();
                    printf("Thread %d found new max: %d at index %d\n",
                           tid, max_value, max_index);
                }
            }
        }
    }

    printf("\n=== RESULTS ===\n");
    printf("Maximum value: %d\n", max_value);
    printf("Found at index: %d\n", max_index);
    printf("Verification: array[%d] = %d\n", max_index, array[max_index]);
    printf("Critical section entered %d times\n", checks);

    free(array);
    return 0;
}
