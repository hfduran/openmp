#include <stdio.h>
#include <omp.h>

int main() {
    long long sum = 0;

    printf("Initial sum = %lld\n\n", sum);

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < 12; i++) {
        int thread_id = omp_get_thread_num();

        // Each thread adds to its PRIVATE copy
        sum += i;

        // Print the PRIVATE copy value (what this thread sees)
        printf("Thread %d: iteration %d, my private sum = %lld\n",
               thread_id, i, sum);
    }

    printf("\nFinal sum after reduction = %lld\n", sum);
    printf("Expected (0+1+2+...+11) = %d\n", (11 * 12) / 2);

    return 0;
}
