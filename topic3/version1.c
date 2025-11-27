#include <stdio.h>
#include <omp.h>

int main() {
    printf("Dividing loop iterations among threads:\n\n");

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < 12; i++) {
            int thread_id = omp_get_thread_num();
            printf("Iteration %d executed by thread %d\n", i, thread_id);
        }
    }

    printf("\nAll iterations completed!\n");

    return 0;
}
