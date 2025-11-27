#include <stdio.h>
#include <omp.h>

int main() {
    int shared_counter = 0;
    int iterations = 10000;

    printf("Using critical section to safely increment counter...\n");
    printf("Running %d iterations\n\n", iterations);

    #pragma omp parallel for
    for (int i = 0; i < iterations; i++) {
        // Critical section: only one thread at a time
        #pragma omp critical
        {
            shared_counter++;
        }
    }

    printf("Expected counter = %d\n", iterations);
    printf("Actual counter   = %d\n", shared_counter);

    if (shared_counter == iterations) {
        printf("\nSUCCESS! Critical section prevented race condition!\n");
        printf("All %d updates were counted.\n", iterations);
    } else {
        printf("\nERROR! Lost updates: %d\n", iterations - shared_counter);
    }

    return 0;
}
