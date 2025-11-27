/* EXAMPLE 2: PARALLEL QUICKSORT
 * Concepts: tasks, taskwait, recursion, single
 */
#include <stdio.h>
#include <omp.h>

void swap(int* a, int* b) { int t = *a; *a = *b; *b = t; }

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) swap(&arr[++i], &arr[j]);
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void parallel_quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        // Create TASKS for recursive calls (dynamic parallelism!)
        #pragma omp task shared(arr)
        { parallel_quicksort(arr, low, pi - 1); }  // Left partition

        #pragma omp task shared(arr)
        { parallel_quicksort(arr, pi + 1, high); }  // Right partition

        #pragma omp taskwait  // Wait for both partitions to finish
    }
}

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90, 88, 45, 50};
    int n = 10;

    printf("Original: "); for(int i=0; i<n; i++) printf("%d ", arr[i]); printf("\n");

    #pragma omp parallel
    {
        #pragma omp single  // Only one thread starts the recursion
        { parallel_quicksort(arr, 0, n - 1); }
        // Other threads steal tasks from the queue
    }

    printf("Sorted:   "); for(int i=0; i<n; i++) printf("%d ", arr[i]); printf("\n");
    return 0;
}
/* Key: tasks allow irregular parallelism (recursion, trees, graphs) */
