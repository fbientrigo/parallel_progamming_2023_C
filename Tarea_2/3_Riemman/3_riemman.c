#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "omp.h"
#include "math.h"


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

int to_csv(double* X, int N, char* filename){
    // Exportar X como archivo CSV
    FILE* archivo = fopen(filename, "w");
    if (archivo == NULL) {
        printf("Error abriendo el archivo para escribir.\n");
        return 1;
    }

    // Escribir encabezado del archivo CSV
    fprintf(archivo, "k,Valor\n");

    // Escribir datos en el archivo CSV
    for (int k = 0; k < N; k++) {
        fprintf(archivo, "%d,%lf\n", k, X[k]);
    }

    fclose(archivo);
    printf("Datos exportados correctamente como datos.csv.\n");
    return 0;
}

double Riemann_Zeta(double s, int k) { // O(k^2)
    double result = 0.0;

    for (int i = 1; i < k; i++) {
        for (int j = 1; j <k; j++) {
            result += (2*(i&1)-1)/pow(i+j,s);
        }
    }
    return result * pow(2,s);
}

double TryK(double* X,unsigned int N) {
    for (int k=0; k<N; k++) {
        X[k] = Riemann_Zeta(2,k); // = pi^2/6
    }
}

int main(int argc, char** argv) {
    clock_t t;


    // ---- (i) paraleliza el loop TryK con OpenMP
    // existe alguna dependencia de datos o variables compartidas?
    // ----

    /*no existe dependencia de datos o variables compartidas,
    pues cada llamada a la zeta de Riemman es independiente de las otras,
    por tanto la paralelización es directa
    */ 


    int N = 100; // max k
    double* X = (double*) malloc( N * sizeof(double) );

    t = clock();
    printf("Sección paralelizada: \n");
    
    int num_threads = omp_get_max_threads();
    int loop_section = N / num_threads;
    
    #pragma omp parallel
    {
        clock_t t_thread = clock();

        int tid = omp_get_thread_num(); // thread  id
        int start = tid * loop_section;
        int end = start + loop_section;
        if (tid == num_threads - 1){ //al ser el ultimo thread
            end = N;
        }

        // operacion de suma dentro de cada seccion del arreglo
        for (int k = start; k < end; k++){
            X[k] = Riemann_Zeta(2, k);
        }

        t_thread = clock() - t_thread;
        printf("\t %d ", tid);
        printf("t: %f [s] \n", (double)t_thread /CLOCKS_PER_SEC);
    }
    
    t = clock() - t;
    printf("\nTiempo: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));

    to_csv(X, N, "riemann_pp.csv");

    t = clock();
    printf("Sección sin paralelizar: \n");
    TryK(X, N);
    t = clock() - t;
    printf("\nTiempo: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));

    to_csv(X, N, "riemann_npp.csv"); 

    free(X);


}