/*
recordar ejecutar openmp con el flag -fopenmp

g++ -fopenmp c3_1_numero_de_hilos.cpp -o 1_hilos.exe

*/


#include "stdio.h"
#include "omp.h"

int main(){

    // se usa para compilar en paralelo el siguiente bloque
    #pragma omp parallel
    {
       int n;
       n = omp_get_num_threads();

       printf("Numero threads: %d\n",n);
    }

}