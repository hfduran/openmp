/* EXAMPLE 4: MONTE CARLO PI ESTIMATION
 * Concepts: reduction, private, atomic, firstprivate, num_threads
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define SAMPLES 10000000

int main() {
    long long inside_circle = 0;
    unsigned int seed = 12345;  // Random seed

    omp_set_num_threads(4);  // Set thread count

    // METHOD 1: Using REDUCTION (best performance)
    #pragma omp parallel for reduction(+:inside_circle) private(seed) firstprivate(seed)
    for (long long i = 0; i < SAMPLES; i++) {
        // firstprivate: each thread gets copy of original seed
        // private: each has own seed that evolves differently
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;

        if (x*x + y*y <= 1.0) {
            inside_circle++;  // Safe: reduction handles this
        }
    }
    // reduction(+:inside_circle): each thread has private counter,
    // automatically summed at the end

    double pi = 4.0 * inside_circle / SAMPLES;
    printf("Pi ≈ %.6f (with %d samples)\n", pi, SAMPLES);

    // METHOD 2: Using ATOMIC (slower but works for any operation)
    inside_circle = 0;
    #pragma omp parallel for private(seed)
    for (long long i = 0; i < SAMPLES; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;

        if (x*x + y*y <= 1.0) {
            #pragma omp atomic
            inside_circle++;  // Atomic increment (slower than reduction)
        }
    }

    pi = 4.0 * inside_circle / SAMPLES;
    printf("Pi ≈ %.6f (atomic version)\n", pi);

    return 0;
}
/* Key: Use reduction when possible (faster), atomic for simple ops, critical for complex */
