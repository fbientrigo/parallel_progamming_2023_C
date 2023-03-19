/*
escribir de la presentacion
*/
#include "stdlib.h" // recordar la libreria estandar de C
#include "stdio.h"

// en C solemos pasar el size del array, pues es dificil conseguirlo
void poblar_array(int* arr, int N){
    for (int i = 0; i < N; i++){
        arr[i] = i;
    }
}

int main(){

    int N = 10;
    int* x = (int*) malloc(N*sizeof(int));

    printf("Array sin inicializar:\n");
    for (int i = 0; i < N; i++){
        printf("%d ",x[i]);
    }
    printf("\n");

    // pasando el arreglo a la función
    poblar_array(x, N);
    printf("Array inicializado:\n");
    for (int i = 0; i < N; i++){
        printf("%d ",x[i]);
    }
    printf("\n");

}