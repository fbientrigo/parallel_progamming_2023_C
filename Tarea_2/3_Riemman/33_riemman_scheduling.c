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

    // (ii) arreglando el load balancing


    int N = 300; // max k
    double* X = (double*) malloc( N * sizeof(double) );

    t = clock();
   
    // chunk directo
    t = clock();
    printf("Dynamic paralelize: \n"); 
    printf("\t chunk size: %d \n", 1);
    #pragma omp parallel for schedule(dynamic, 1)
    for (int k = 0; k < N; k++){
            X[k] = Riemann_Zeta(2, k);
    }
    t = clock() - t;
    printf("\t t: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));

    // static
    t = clock();
    printf("Static paralelize: \n"); 
    printf("\t chunk size: %d \n", 1);
    #pragma omp parallel for schedule(static, 1)
    for (int k = 0; k < N; k++){
            X[k] = Riemann_Zeta(2, k);
    }
    t = clock() - t;
    printf("\t t: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));

    // guided
    t = clock();
    printf("Guided paralelize: \n"); 
    #pragma omp parallel for schedule(guided)
    for (int k = 0; k < N; k++){
            X[k] = Riemann_Zeta(2, k);
    }
    t = clock() - t;
    printf("\t t: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));

    // automatic
    t = clock();
    printf("Automatic paralelize: \n"); 
    #pragma omp parallel for schedule(auto)
    for (int k = 0; k < N; k++){
            X[k] = Riemann_Zeta(2, k);
    }
    t = clock() - t;
    printf("\t t: %f [s]\n", ( (double)t /CLOCKS_PER_SEC));



    free(X);

}