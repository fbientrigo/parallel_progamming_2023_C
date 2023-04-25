/*
recordar ejecutar openmp con el flag -fopenmp

g++ -fopenmp c3_1_numero_de_hilos.cpp -o 1_hilos.exe

*/


#include "stdio.h"
#include "omp.h"

int main(){

    // se usa para compilar en paralelo el siguiente bloque
    #pragma omp parallel num_threads(4)
    {
        int i = omp_get_thread_num(); //numero del thread
        int n = omp_get_num_threads(); // total de threads

       printf("Thread %d, con numero total threads: %d\n",i,n);
    }

}