#include "stdio.h"
#include "stdlib.h"

int actualizar_x(int x){
    return x+1;
}

void actualizar_x2(int* p){
    *p += 1;
}

int main(){

    // pasar por valor
    int x = 10;
    int y;
    // modificamos y =11; pero el x se mantiene x = 10
    y = actualizar_x(x);

    printf("pasando por valor \n");
    printf("x: %d\t y: %d\n", x, y);

    // pasar por referencia
    int x2 = 10;
    int* p;

    p = &x; // pasamos la referencia al pointer
    actualizar_x2(p);

    printf("pasando por referencia (punteros) \n");
    printf("x: %d \n", x);

    // ==========
    // pasar por valor
    int x3 = 10;

    // modificamos y =11; pero el x se mantiene x = 10
    actualizar_x2(&x3);

    printf("pasando por referencia (&x) \n");
    printf("x: %d\n", x3); 

}
