#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "omp.h"


void jump(int i, int b);
void poblar_array(int* arr, int N);
void print_array(int* arr, int N, int carriage);

/*
Se trata de realizar mas rapida la suma

A[i+1] += A[i]

luego de reescribirla de varias maneras consegui reducir un 40%
el tiempo de computo, al aprovechar funciona el for; es bastante simple
*/

int main(){

    clock_t t;
    int N = 100000;
    int* phi = (int*) malloc( N * sizeof(N) );
    poblar_array(phi,N);


    printf("Metodo lineal: \n");
    t = clock();
    for (int i = 0; i < N; i++) {
        phi[i+1] += phi[i];
    }
    t = clock() - t;
    printf("La suma demoro: %d [s]\n", t);
    printf("El valor final fue: %d\n", phi[N-1]);
    //print_array(phi, N, 4);

    printf("-------------\n");

    poblar_array(phi,N);
    //printf("\n phi: \n");
    //print_array(phi, N, 4);

    printf("Metodo de suma por adelanto: \n");
    t = clock();
    for (int k = 1 ; k < N; k+=2) {
        phi[k] += phi[k-1];
        phi[k+1] += phi[k];
    }
    t = clock() - t;
    printf("La suma demoro: %d [s]\n", t);
    printf("El valor final fue: %d\n", phi[N-1]);
    //printf("phi: \n");
    //print_array(phi, N, 4);

    printf("\nDe acuerdo al algebra deberia ser: %d", N*(N-1)/2);


}

void jump(int i, int b) {
    if (i != 0) {
        if(i%b == 0){
            printf("\n");
        }
    }
}

void poblar_array(int* arr, int N){
    for (int i = 0; i < N; i++){
        arr[i] = i;
    }
}

void print_array(int* arr, int N, int carriage) {
    for (int i = 0; i < N; i++) {
        jump(i, carriage);
        printf("%d \t", arr[i]);
    }
    printf("\n");
}

