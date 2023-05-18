#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<immintrin.h> // memory management intel
#include<time.h>

void poblar_farray(float* arr, int N){
    for (int i = 0; i < N; i++){
        arr[i] = (float) i;
    }
}

// row-major-order addition
void row_wise_add(float* A, float* B, float*C, size_t N) {
	for(int i = 0; i<N; i++)
		for(int j = 0; j<N; j++)
			C[i*N+j] = A[i*N+j] + B[i*N+j];
}

// row-major-order addition VECTORIZADA 
void avx_row_wise_add(float* A, float* B, float*C, uint64_t N) {

    __m256 temp;
	for(uint64_t i = 0; i<N; i++) {
        for(uint64_t j = 0; j<N; j += 8){
            __m256 AV = _mm256_load_ps(A+i*N+j);
            __m256 BV = _mm256_load_ps(B+i*N+j);
            temp = _mm256_add_ps(AV,BV);
            _mm256_store_ps(C+i*N+j, temp);
        }
    }

    
}



int main(int argc, char** argv) {
    clock_t t;

    int N = 2*2*2;

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
    t = clock() - t;
    printf("runtime: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));

    return 0; 
}