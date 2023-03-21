#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "omp.h"

void jump(int i, int b) {
    if(i%b == 0){
        printf("\n");
    }
}

void poblar_array(int* arr, int N){
    for (int i = 0; i < N; i++){
        arr[i] = i;
    }
}

int main(int argc, char** argv) {
    clock_t t;
    t = clock();
    printf("Comenzando Programa: \n");

    int N = 1000;
    int* A = (int*) malloc( N * sizeof(N) );

    poblar_array(A, N);

    printf("Suma sin paralelizar")
    // for con la suma cumulativa
    for (int i=1; i<N; i++) {
        A[i] = A[i] + A[i-1];
        printf("A[%d] = %d \t", i, A[i]);
        jump(i, 5);

    }

    printf("Suma paralelizada")
    // escribe aqui el codigo    




    t = clock() - t;
    printf("\nTiempo del programa: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));
    return 0; 
}