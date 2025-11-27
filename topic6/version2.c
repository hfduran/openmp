#include <stdio.h>
#include <omp.h>

int main() {
    int shared_counter = 0;  // Shared by default (declared outside)
    int iterations = 100000;

    printf("Initial shared_counter = %d\n", shared_counter);
    printf("Running %d iterations in parallel...\n\n", iterations);

    #pragma omp parallel for shared(shared_counter)
    for (int i = 0; i < iterations; i++) {
        // Each thread increments the shared counter
        // This creates MASSIVE race conditions!
        shared_counter++;
    }

    printf("Expected shared_counter = %d\n", iterations);
    printf("Actual shared_counter = %d\n", shared_counter);
    printf("Lost updates = %d\n", iterations - shared_counter);

    if (shared_counter != iterations) {
        printf("\nRACE CONDITION! Many updates were lost!\n");
    } else {
        printf("\nGot extremely lucky (almost impossible)!\n");
    }

    return 0;
}
