#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<time.h>

void poblar_farray(float* arr, int N){
    for (int i = 0; i < N; i++){
        arr[i] = (float) i;
    }
}

// row-major-order addition
void row_wise_add(float* A, float* B, float*C, size_t N) {
	// i: fila
	// j: columna
	for(int i = 0; i<N; i++)
		for(int j = 0; j<N; j++)
			C[i*N+j] = A[i*N+j] + B[i*N+j];
}

// col-major-order addition
void col_wise_add(float* A, float* B, float*C, size_t N) {
	// i: columna
	// j: fila
	for(int i = 0; i<N; i++)
		for(int j = 0; j<N; j++)
			C[j*N+i] = A[j*N+i] + B[j*N+i];
}

int main(int argc, char** argv) {
    clock_t t;

    int N = 1024;

    float* A = (float*) malloc(sizeof(float)*N*N);
    float* B = (float*) malloc(sizeof(float)*N*N);
    float* C = (float*) malloc(sizeof(float)*N*N);

    poblar_farray(A, N);
    poblar_farray(B, N);

    t = clock();
    printf("row_wise_add: \n");
    row_wise_add(A,B,C,N);
    t = clock() - t;
    printf("runtime: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));

    t = clock();
    printf("col_wise_add: \n");
    col_wise_add(A,B,C,N);
    t = clock() - t;
    printf("runtime: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));

    return 0; 
}