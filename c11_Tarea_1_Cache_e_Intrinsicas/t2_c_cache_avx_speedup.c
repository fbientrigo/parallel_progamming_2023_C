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

    __m256 temp; // variable temporal para guardar antes de pasar a C
	for(uint64_t i = 0; i<N; i++) {
        for(uint64_t j = 0; j<N; j += 8){

            //guardamos datos en los registros vectorizados
            __m256 AV = _mm256_load_ps(A+i*N+j);
            __m256 BV = _mm256_load_ps(B+i*N+j);
            temp = _mm256_add_ps(AV,BV); // y los sumamos en el tmp
            //para entonces guardarlos en C
            _mm256_store_ps(C+i*N+j, temp);
        }}
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
    double T1 = ( (double)t /CLOCKS_PER_SEC);
    printf("\truntime: %f [s]\n", T1);

    t = clock();
    printf("avx_row_wise_add: \n");
    avx_row_wise_add(A,B,C,N);
    t = clock() - t;
    double Tavx = ( (double)t /CLOCKS_PER_SEC);
    printf("\truntime: %f [s]\n", Tavx);

    printf("Speedup: %f \n", T1/Tavx );

    return 0; 
}