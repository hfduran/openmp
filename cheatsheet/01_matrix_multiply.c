/* EXAMPLE 1: MATRIX MULTIPLICATION
 * Concepts: parallel for, collapse, private, shared, reduction, scheduling
 */
#include <stdio.h>
#include <omp.h>
#define N 100

int main() {
    double A[N][N], B[N][N], C[N][N];
    double sum;

    // Initialize matrices
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
            C[i][j] = 0;
        }

    // MATRIX MULTIPLICATION: C = A × B
    #pragma omp parallel for collapse(2) schedule(dynamic, 10) private(sum) shared(A, B, C)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sum = 0;  // private: each thread has its own
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];  // shared: read-only is safe
            }
            C[i][j] = sum;  // shared: each thread writes different location
        }
    }
    // collapse(2): parallelize both i and j loops
    // schedule(dynamic,10): dynamic load balancing, chunks of 10
    // private(sum): each thread needs own accumulator
    // shared(A,B,C): matrices shared among threads

    printf("Matrix multiplication complete!\n");
    return 0;
}
