#include <stdio.h>
#include <omp.h>

int main() {
    int temp = 100;  // This will be private to each thread

    printf("Before parallel region: temp = %d\n\n", temp);

    #pragma omp parallel for private(temp)
    for (int i = 0; i < 8; i++) {
        int thread_id = omp_get_thread_num();

        // Each thread has its own 'temp' (uninitialized!)
        // Let's set it to something unique
        temp = thread_id * 10;

        printf("Thread %d: iteration %d, temp = %d\n",
               thread_id, i, temp);
    }

    printf("\nAfter parallel region: temp = %d\n", temp);
    printf("(Original value preserved because 'temp' was private)\n");

    return 0;
}
