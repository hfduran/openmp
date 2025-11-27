#include <stdio.h>
#include <omp.h>

int main() {
    printf("Starting program with 1 thread\n\n");

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();

        printf("Hello from thread %d of %d\n", thread_id, total_threads);
    }

    printf("\nBack to 1 thread\n");

    return 0;
}
