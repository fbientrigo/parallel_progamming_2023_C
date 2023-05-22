/*
como escribir programas en paralelo
esto se dejo en continuar despues en otra clase usando librerias

Es necesario compilar de manera:

g++ -fopenmp .\c2_8_librerias.cpp -o 8_librerias.exe
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