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
    int sum = 0;

    poblar_array(A, N);

    printf("Suma sin paralelizar\n");
    // for con la suma cumulativa
    for (int i=1; i<N; i++) {
        sum += A[i];
    }
    t = clock() - t;
    printf("\nTiempo sin paralelizar: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));
    printf("Suma total: %d\n", sum);

    printf("=================\n");

    sum =0;
    printf("Suma paralelizada\n");
    /*
    #pragma: es lo que especificamos para paralelizar
    omp parallel for: especifica que distribuiremos el loop en los threads
    reduction(+:sum): especfiica que queremos finalmente sumar todas las variables sum
    */
    #pragma omp parallel for reduction(+:sum)
    for (int i=0; i<N; i++){
        sum += A[i];
    }
    printf("Suma total: %d\n", sum);

    t = clock() - t;
    printf("\nTiempo del programa paralelizado: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));
    return 0; 
}