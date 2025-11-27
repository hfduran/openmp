#include <stdio.h>
#include <omp.h>

int main() {
    int shared_counter = 0;  // Shared by default (declared outside)
    int array[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    printf("Initial shared_counter = %d\n\n", shared_counter);

    #pragma omp parallel for shared(shared_counter, array)
    for (int i = 0; i < 8; i++) {
        int thread_id = omp_get_thread_num();

        // Reading from shared array is safe
        printf("Thread %d reading array[%d] = %d\n",
               thread_id, i, array[i]);

        // Writing to shared counter - RACE CONDITION!
        shared_counter++;
    }

    printf("\nExpected shared_counter = 8\n");
    printf("Actual shared_counter = %d\n", shared_counter);

    if (shared_counter != 8) {
        printf("RACE CONDITION! Result is wrong!\n");
    } else {
        printf("Got lucky this time, but still unsafe!\n");
    }

    return 0;
}
