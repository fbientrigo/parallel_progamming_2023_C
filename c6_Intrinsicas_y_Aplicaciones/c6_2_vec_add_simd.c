#include <stdint.h>
#include <stdio.h>
#include <immintrin.h>
#include <time.h>

/*
Uso de vectorizacion para la suma de vectores



*/



void plain_vecadd(float* A, float* B, float* C, uint64_t N){
    // suma utilizando un for
    for (uint64_t i = 0; i < N; i++){
        C[i] = A[i] + B[i];
    }

}

void avx2_vecadd(float* A, float* B, float* C, uint64_t N){
    // suma utilizando vectorizacion
    __m256 X;
    for (uint64_t i = 0; i < N; i += 8){
        __m256 AV = _mm256_load_ps(A+i);
        __m256 BV = _mm256_load_ps(B+i);
        X = _mm256_add_ps(AV,BV);
        _mm256_store_ps(C+i, X);
    }

}

void populate_vectors(float* A, float* B, uint64_t N){

    for (uint64_t i = 0; i < N; i++){
        A[i] = i*1.0;
        B[i] = i*2.0;
    }

}

int main(){

    clock_t begin, end;
    double time_spent;

    uint64_t N = 1 << 22;

    printf("Vector elements: %lu\n",N);

    uint64_t error_flag = 0;

    float* A = _mm_malloc(sizeof(float)*N, 32); //align on 32-byte boundaries for AVX registers
    float* B = _mm_malloc(sizeof(float)*N, 32);
    float* C = _mm_malloc(sizeof(float)*N, 32);
    float check;

    populate_vectors(A, B, N);

    begin = clock();
    plain_vecadd(A, B, C, N);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Serial add: %f\n",time_spent);

    begin = clock();
    avx2_vecadd(A, B, C, N);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Vector add: %f\n",time_spent);

    for (uint64_t i = 0; i < N; i++){
        check = i*3.0 - C[i];
        if (check != 0.0){
           error_flag = 1;
        }
    }

    if (error_flag == 1){
        printf("Error in sum.\n");
    }
}
