#include <stdio.h>
#include <omp.h>

int main() {
    int array[16] = {10, 25, 15, 30, 5, 40, 20, 35,
                     12, 28, 18, 33, 8, 45, 22, 38};

    int max_value = -1;
    int max_index = -1;

    printf("Array: ");
    for (int i = 0; i < 16; i++) {
        printf("%d ", array[i]);
    }
    printf("\n\nSearching with 4 threads...\n\n");

    #pragma omp parallel for num_threads(4)
    for (int i = 0; i < 16; i++) {
        int tid = omp_get_thread_num();

        printf("Thread %d checking index %d (value = %d)\n",
               tid, i, array[i]);

        if (array[i] > max_value) {
            #pragma omp critical
            {
                if (array[i] > max_value) {
                    max_value = array[i];
                    max_index = i;
                    printf("  >>> Thread %d updated max to %d at index %d\n",
                           tid, max_value, max_index);
                }
            }
        }
    }

    printf("\n=== FINAL RESULT ===\n");
    printf("Maximum: %d at index %d\n", max_value, max_index);

    return 0;
}
