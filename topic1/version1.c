#include <stdio.h>
#include <omp.h>

int main() {
    printf("Before parallel region (single thread)\n");

    #pragma omp parallel
    {
        printf("Hello from parallel region!\n");
    }

    printf("After parallel region (single thread again)\n");

    return 0;
}
