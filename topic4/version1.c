#include <stdio.h>
#include <omp.h>

int main() {
    int sum = 0;
    int array[16];

    // Initialize array
    for (int i = 0; i < 16; i++) {
        array[i] = i + 1;  // [1, 2, 3, ..., 16]
    }

    printf("Processing array in parallel:\n");

    // Combined parallel for - cleaner syntax!
    #pragma omp parallel for
    for (int i = 0; i < 16; i++) {
        int thread_id = omp_get_thread_num();
        printf("Thread %d processing array[%d] = %d\n",
               thread_id, i, array[i]);
    }

    printf("\nProcessing complete!\n");

    return 0;
}
