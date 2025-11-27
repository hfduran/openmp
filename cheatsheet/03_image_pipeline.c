/* EXAMPLE 3: IMAGE PROCESSING PIPELINE
 * Concepts: sections, barrier, single, master, nowait, critical
 */
#include <stdio.h>
#include <omp.h>
#define SIZE 1000

double image[SIZE], blurred[SIZE], edges[SIZE];

int main() {
    // Initialize
    for (int i = 0; i < SIZE; i++) image[i] = i * 0.5;

    #pragma omp parallel num_threads(4)
    {
        // MASTER: Setup (only thread 0)
        #pragma omp master
        { printf("Thread 0: Starting pipeline...\n"); }
        // No barrier after master - others continue

        // SECTIONS: Different pipeline stages
        #pragma omp sections
        {
            #pragma omp section
            { printf("Thread %d: Loading image\n", omp_get_thread_num()); }

            #pragma omp section
            { printf("Thread %d: Preprocessing\n", omp_get_thread_num()); }

            #pragma omp section
            { printf("Thread %d: Color correction\n", omp_get_thread_num()); }
        }
        // Implicit barrier - all sections finish

        // PARALLEL FOR: Blur filter
        #pragma omp for nowait
        for (int i = 1; i < SIZE-1; i++) {
            blurred[i] = (image[i-1] + image[i] + image[i+1]) / 3.0;
        }
        // nowait: don't wait, continue to next loop

        // PARALLEL FOR: Edge detection
        #pragma omp for
        for (int i = 1; i < SIZE-1; i++) {
            edges[i] = blurred[i+1] - blurred[i-1];
        }
        // Implicit barrier

        // SINGLE: Save result (only one thread)
        #pragma omp single
        { printf("Thread %d: Saving processed image\n", omp_get_thread_num()); }
        // Implicit barrier after single
    }

    printf("Pipeline complete!\n");
    return 0;
}
