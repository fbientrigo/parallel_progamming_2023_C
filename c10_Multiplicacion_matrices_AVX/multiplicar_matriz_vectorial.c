#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <omp.h>

/*
Multiplicar matrices usando el paralelismo de OpenMP

*/



void init(int A[], int x[], int m, int n) {

    for (int row = 0; row < m; row++)
        for (int col = 0; col < n; col++)
            A[row*n+col] = row >= col ? 1 : 0;

    for (int col = 0; col < n; col++)
        x[col] = col;
}

void mult(int A[], int x[], int b[], int m, int n, bool parallel){

    #pragma omp parallel for if(parallel)
    for (int row = 0; row < m; row++) {
        int accum = 0;
        for (int col = 0; col < n; col++)
            accum += A[row*n+col]*x[col];
        b[row] = accum;
    }
}

int main () {
    int n = 40000;
    int m = 40000;

    int* A = malloc(m*n * sizeof(int));
    int* x = malloc(n * sizeof(int));
    int* b = malloc(m * sizeof(int));

    double start_time, end_time, time_spent;

    init(A, x, m, n);

    start_time = omp_get_wtime();

    for (int k = 0; k < 3; k++) {
        mult(A, x, b, m, n, false);
    }

    end_time = omp_get_wtime();
    time_spent = end_time - start_time;
    printf("Time in serial sum: %lf\n",time_spent);

    start_time = omp_get_wtime();

    for (int k = 0; k < 3; k++) {
        mult(A, x, b, m, n, true);
    }

    end_time = omp_get_wtime();
    time_spent = end_time - start_time;
    printf("Time in parallel sum: %lf\n",time_spent);

    for (int index = 0; index < m; index++)
        if (b[index] != index*(index+1)/2)
            printf("Error at position %d %d\n",index,b[index]);
}