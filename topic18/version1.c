#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <math.h>

// Different independent tasks
void process_audio() {
    printf("Thread %d: Processing audio...\n", omp_get_thread_num());
    usleep(500000);  // 500ms
    printf("Thread %d: Audio processing complete!\n", omp_get_thread_num());
}

void process_video() {
    printf("Thread %d: Processing video...\n", omp_get_thread_num());
    usleep(700000);  // 700ms
    printf("Thread %d: Video processing complete!\n", omp_get_thread_num());
}

void process_subtitles() {
    printf("Thread %d: Processing subtitles...\n", omp_get_thread_num());
    usleep(300000);  // 300ms
    printf("Thread %d: Subtitle processing complete!\n", omp_get_thread_num());
}

void generate_thumbnails() {
    printf("Thread %d: Generating thumbnails...\n", omp_get_thread_num());
    usleep(400000);  // 400ms
    printf("Thread %d: Thumbnail generation complete!\n", omp_get_thread_num());
}

int main() {
    printf("=== SECTIONS: Parallel Independent Tasks ===\n");
    printf("Processing a media file with 4 independent tasks\n\n");

    double start = omp_get_wtime();

    #pragma omp parallel sections num_threads(4)
    {
        #pragma omp section
        {
            process_audio();
        }

        #pragma omp section
        {
            process_video();
        }

        #pragma omp section
        {
            process_subtitles();
        }

        #pragma omp section
        {
            generate_thumbnails();
        }
    }
    // Implicit barrier - all sections finish before continuing

    double elapsed = omp_get_wtime() - start;

    printf("\n=== RESULTS ===\n");
    printf("All tasks completed in %.3f seconds (parallel)\n", elapsed);
    printf("Sequential would take: 0.500 + 0.700 + 0.300 + 0.400 = 1.900 seconds\n");
    printf("Speedup: %.2fx\n", 1.9 / elapsed);

    return 0;
}
