#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "omp.h"


int main(int argc, char** argv) {
    clock_t t;
    t = clock();
    printf("Comenzando Programa: \n");


    int N = 10;
    // se usa para compilar en paralelo el siguiente for
    #pragma omp parallel for
    for (int i = 0; i <N; i++) {

    }
    
    /*
    Restricciones para parallel for:
        - el programa necesita saber cuantas iteraciones habran
            - esto es dificil pues pueden haber breaks adentro
        - ademas no podemos manipular los indices de iteracion en el cuerpo del ciclo
    
    
    
    */

    t = clock() - t;
    printf("\nTiempo del programa: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));
    return 0; 
}