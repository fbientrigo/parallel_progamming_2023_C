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



    // se usa para compilar en paralelo el siguiente bloque
    #pragma omp parallel num_threads(4)
    {
        int i = omp_get_thread_num(); //numero del thread
        int n = omp_get_num_threads(); // total de threads

       printf("Thread %d, con numero total threads: %d\n",i,n);

    }

    /*
    
    
    */

    t = clock() - t;
    printf("\nTiempo del programa: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));
    return 0; 
}