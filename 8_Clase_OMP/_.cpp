#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "omp.h"


void poblar_array(int* arr, int N){
    for (int i = 0; i < N; i++){
        arr[i] = i;
    }
}

int main(int argc, char** argv) {
    clock_t t;
    t = clock();
    printf("Comenzando Programa: \n");






    t = clock() - t;
    printf("\nTiempo del programa: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));
    return 0; 
}