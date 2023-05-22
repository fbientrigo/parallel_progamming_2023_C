#include "stdio.h"

int main(){
    // forma facil pero poco manual de definir un arreglo
    int x[5];

    for (int i = 0; i < 5; i++) {
        x[i] = i+1;
    }

    for (int i = 0; i < 5; i++) {
        printf("%d \n", x[i]);
    }

    // forma alternativa usada por otros progamadores:
    int Nz = 10; // es siempre mejor especificar el size
    int* z = new int[Nz]; // el new keyword es exclusivo de c++

    for (int i = 0; i < Nz; i++) {
        z[i] = i+1;
    }

    for (int i = 0; i < Nz; i++) {
        printf("%d \n", z[i]);
    }
}
