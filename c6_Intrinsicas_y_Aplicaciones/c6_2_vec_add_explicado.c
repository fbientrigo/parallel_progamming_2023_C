#include<stdio.h>
#include<stdint.h>
#include<immintrin.h> // memory management intel
#include<time.h>

void plain_vecadd(float *A, float *B, float *C, uint64_t N){
    /*
    Adds dos vectores en un vector C; especificandose el tamaño N

    todos los vectores deben ser iguales
    */
    for (uint64_t i = 0; i<N; i++ ){
        C[i] = A[i] + B[i]; 
    }
}

void avx2_vecadd(float *A, float *B, float *C, uint64_t N){
    /*instrucciones avx2 dados por intel; permite agregar elementos en paralelo
    AVX2 -> advanced vector extension 2
    */
    __m256 X;
    for (uint64_t i=0; i<N; i += 8){
        __m256 AV = _mm256_load_ps(A+i);
        __m256 BV = _mm256_load_ps(B+i);
        X = _mm256_add_ps(AV,BV);
        _mm256_store_ps(C+i, X);

    }
}

void populate_vectors(float *A, float *B, uint64_t N){

        for(uint64_t i = 0; i<N; i++){
            A[i] = i*1.0;
            B[i] = i*2.0;
        }
}

int main(){

    clock_t begin, end;
    double time_spent;

    uint64_t N = 1 << 20;

    printf("Vector elements: %lu\n", N);

    uint64_t error_flag = 0 ;

    float *A = _mm_malloc(sizeof(float)*N, 32); //Align on 32-byte boundaries for AVX registers
    float *B = _mm_malloc(sizeof(float)*N, 32);
    float *C = _mm_malloc(sizeof(float)*N, 32);
    float check;

    populate_vectors(A, B, N);

    begin = clock();
    plain_vecadd(A, B, C, N);
    end = clock();
    time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
    printf("Serial add; %f\n", time_spent); 

    begin = clock();
    avx2_vecadd(A, B, C, N);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Vector add; %f\n", time_spent); 

    for (uint64_t i = 0; i<N; i++){
        check = i*3.0 - C[i];
        if (check != 0.0){
            error_flag = 1;
        }
    }

    if (error_flag == 1){
        printf("Error in sum.\n");
    }

} 

//correr utilizando 
//gcc -march=skylake parasyte.c -o parasyte.x