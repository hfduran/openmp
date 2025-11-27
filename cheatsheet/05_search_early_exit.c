/* EXAMPLE 5: PARALLEL SEARCH WITH EARLY EXIT
 * Concepts: shared flags, atomic, critical, dynamic scheduling, lastprivate
 */
#include <stdio.h>
#include <omp.h>
#define N 1000000

int expensive_check(int val, int target) {
    // Simulate expensive computation
    int sum = 0;
    for (int i = 0; i < 1000; i++) sum += val * i;
    return (val == target);
}

int main() {
    int data[N];
    int target = 777777;
    int found = 0;         // shared flag
    int found_index = -1;  // where we found it

    // Initialize
    for (int i = 0; i < N; i++) data[i] = i;

    // PARALLEL SEARCH with early exit optimization
    #pragma omp parallel for schedule(dynamic, 1000) shared(found, found_index)
    for (int i = 0; i < N; i++) {
        // Check flag (atomic read is safe)
        if (!found) {  // Skip work if someone found it

            if (expensive_check(data[i], target)) {
                // CRITICAL: update both variables atomically together
                #pragma omp critical
                {
                    if (!found) {  // Double-check inside critical
                        found = 1;
                        found_index = i;
                        printf("Thread %d found target at index %d!\n",
                               omp_get_thread_num(), i);
                    }
                }
            }
        }
    }
    // schedule(dynamic): threads grab chunks on-demand (better load balance)
    // shared(found, found_index): all threads see same flag

    if (found) {
        printf("Result: Found %d at index %d\n", target, found_index);
    } else {
        printf("Not found\n");
    }

    // BONUS: Finding MAX with index (needs critical, not just reduction)
    int max_val = data[0];
    int max_idx = 0;

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        if (data[i] > max_val) {
            #pragma omp critical
            {
                if (data[i] > max_val) {  // Double-check pattern
                    max_val = data[i];
                    max_idx = i;
                }
            }
        }
    }

    printf("Max value: %d at index %d\n", max_val, max_idx);
    return 0;
}
/* Key: Use shared flags for early exit, critical for updating multiple vars together */
