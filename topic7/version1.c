#include <stdio.h>
#include <omp.h>

int main() {
    long long sum = 0;  // Use long long to avoid overflow!
    int iterations = 100000;

    printf("Computing sum of 1 to %d in parallel...\n\n", iterations);

    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i <= iterations; i++) {
        sum += i;  // Safe! Each thread has private copy
    }

    // Expected: sum = n * (n+1) / 2
    long long expected = ((long long)iterations * (iterations + 1)) / 2;

    printf("Expected sum = %lld\n", expected);
    printf("Actual sum   = %lld\n", sum);

    if (sum == expected) {
        printf("\nSUCCESS! Reduction worked correctly!\n");
        printf("No race condition - all updates counted!\n");
    } else {
        printf("\nERROR! Something went wrong.\n");
    }

    return 0;
}
