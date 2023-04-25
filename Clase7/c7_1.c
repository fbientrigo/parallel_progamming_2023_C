#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "omp.h"

/*
complementar con el codiog de poisson_mm
*/


void poblar_array(int* arr, int N){
    for (int i = 0; i < N; i++){
        arr[i] = i;
    }
}

float hsum_avx(__m256 X) {
    X = _mm256_hadd_ps(X,X);
    X = _mm256_hadd_ps(X,X);
    X = _mm256_hadd_ps(X,X);
    // extraer el valor, hacer un cast a floats
    //float = c = _mm256
}

int main(int argc, char** argv) {
    clock_t t;
    t = clock();







    t = clock() - t;
    printf("\nTiempo del programa: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));
    return 0; 
}