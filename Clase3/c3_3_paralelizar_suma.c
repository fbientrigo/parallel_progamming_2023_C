#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "omp.h"


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


int main(int argc, char** argv) {
    clock_t t;
    int N = 8*4;
    int* A = (int*) malloc( N * sizeof(N) );
    int num_threads = omp_get_max_threads();

    poblar_array(A, N);

    // dividir el arreglo en distintos sectors
    int sector_size = N / num_threads;
    int* lastys = (int*) malloc(num_threads * sizeof(int));

    printf("Tenemos un arreglo que parte de 0 a %d\n", N-1);
    printf("Esto da un size de: %d\n", N);

    printf("Step 1; sumar dentro de un sub arreglo\n");
    #pragma omp parallel
    {
        int tid = omp_get_thread_num(); // thread  id
        int start = tid * sector_size;
        int end = start + sector_size;
        if (tid == num_threads - 1){ 
            end = N;
        }

        // operacion de suma dentro de cada seccion del arreglo
        for (int i = start; i < end - 1; i++){
            A[i+1] += A[i];
        }

        // agregar a lastys el ultimo elemento de nuestro arreglo
        lastys[tid] = A[end-1];

        #pragma omp barrier
        printf("\n>>-------tid: %d---------\n", tid);
        printf("Trabajando con \n start: %d, end: %d\n", start, end-1);
        for (int i = start; i < end; i++) {
            jump(i, 4);
            printf("%d \t", A[i]);
        }

        printf("\nEl ultimo elemento de %d es %d\n", tid, A[end-1]);
        printf("\n---------------------<<\n");
        #pragma omp barrier
    }


    printf("\nstep 2, suma de ultimos elementos\n");
    printf("\n lastys: \n");
    print_array(lastys, num_threads, 4);

    for (int k = 1 ; k < num_threads; k+=2) {
        lastys[k] += lastys[k-1];
        lastys[k+1] += lastys[k];
    }
    printf("lastys: \n");
    print_array(lastys, num_threads, 4);


    // ahora viene sumar a cada sublista los elementos que tenemos
    #pragma omp parallel
    {
        int tid = omp_get_thread_num(); // thread  id
        int start = tid * sector_size;
        int end = start + sector_size;
        if (tid == num_threads - 1){ 
            end = N;
        }


        // agregar a lastys el ultimo elemento de nuestro arreglo
        if (tid != 0) {
            for (int i = start; i < end; i++){
                A[i] += lastys[tid-1];
            } 
        }

    // end of parallel
    }


    // imprimir A para ver que tal le fue
    printf("\nA final: \n");
    for (int i = 0; i < N; i++) {
        jump(i, 4);
        printf("%d ", A[i]);
    }



    printf("\n por algebra deberia ser, de tener %d elementos\n", N);
    printf("la suma total, osea el ultimo elemento deberia ser: %d", N*(N-1)/2);
    return 0; 
}