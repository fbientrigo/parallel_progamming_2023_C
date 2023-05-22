#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "omp.h"


int main(int argc, char** argv) {
    clock_t t;
    t = clock();
    printf("Comenzando Programa: \n");


    int N = 8;
    #pragma omp parallel 
    {
        int nthread = omp_get_thread_num();
        // al realizar un pragma parallel for adentro; se reparte
        // de no estar esto, todos los threads ejecutarian el mismo for    
        #pragma omp parallel for
        for (int i = 0; i <N; i++) {
            printf("Ejecutando for con %d en thread %d\n", i, nthread);
        }
    }
    

    t = clock() - t;
    printf("\nTiempo del programa: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));
    return 0; 
}