#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<immintrin.h> // memory management intel
#include<time.h>

void poblar_farray(float* arr, int N){
    for (int i = 0; i < N; i++){
        arr[i] = (float) i;
    }
}

void despoblar_farray(float* arr, int N){
    for (int i = 0; i < N; i++){
        arr[i] = 0.0;
    }
}

void vectorized_horizontal_add(float* M, float*B, uint64_t N) {
	for(uint64_t i = 0; i<N-1; i++) {
            // cargamos los 4 floats de cada fila
            __m128 MV = _mm_load_ps(M+i*4);
            // sumamos horizontalmente 2 veces
            // de forma que tengamos los 4 elementos sumados
            MV = _mm_hadd_ps(MV, MV);
            MV = _mm_hadd_ps(MV, MV);

            // guarda el valor inferior en memoria
            // ss single single (floats)
            // sd single doubles 
            _mm_store_ss(B+i+1, MV);
                // i+1 pues comenzamos con B[1]
                // notar que no llenamos B[0] o B[N]
                // pues el algoritmo original no lo incluye directamente en el metodo de suma
    }
}

void lineal_stencil(float* A, float* B, size_t N) {
	for(uint64_t i=1; i<N-1; i++)
		B[i] = 0.33 * (A[i-1] + A[i] + A[i+1]);
	for(uint64_t i=1; i<N-1; i++)
		A[i] = B[i];
    B[0] = A[0]; // para la siguiente iteración
    B[N-1] = A[N-1];
}

void data_transform(float* A, float* M, size_t N) {

	for(uint64_t offset = 0; offset < 3; offset++) {
	    for(uint64_t i=0; (i+offset)<N; i++) {
            M[(offset+1)*N + i] = A[i + offset];
    }}
}



void data_transform_to_transpose(float* A, float* Mt, size_t N) {
    for(uint64_t k = 1; k < 4; k++) {
        for(uint64_t i=0; (i+k-1)<N; i++) {
            Mt[i*4 + k] = A[i+k-1];
    }}
}


// funciones de printing:
void jump(int i, int b);
void print_array(float* arr, int N, int carriage);


// Seccion principal
int main(int argc, char** argv) {
    clock_t t;

    int N = 8;

    /*Metodo Lineal*/
    float* A = (float*) malloc(sizeof(float)*N);
    float* B = (float*) malloc(sizeof(float)*N);
    poblar_farray(A, N);

    printf("A:");
    print_array(A,N,4);
    printf("\n \n");

    t = clock();
    printf("Stencil Lineal \n");
    lineal_stencil(A,B,N);
    t = clock() - t;
    double T1 = ( (double)t /CLOCKS_PER_SEC);

    printf("B:");
    print_array(B, N, 4);
    printf("\n \n");

    printf("\truntime: %f [s]\n", T1);


    /*----- Jacobi Stencil 1D -----------*/
    printf("Jacobi Vectorizado\n");

    poblar_farray(A, N);
    despoblar_farray(B,N);

    t = clock();
    /*Metodo Data Transformation*/
    float* M = (float*) malloc(sizeof(float)*N*4);

    // Limpiar M a 0.'s
    despoblar_farray(M, 4*N);
    data_transform_to_transpose(A, M, N);

    printf("Mt:");
    print_array(M, N*4, 4);
    printf("\n \n");

    vectorized_horizontal_add(M,B, N);
    B[0] = A[0]; // para la siguiente iteración
    B[N-1] = A[N-1];

    // finalizando así el algoritmo de Jacobi Stencil
    for (int i = 1; i < N-1; i++)
        B[i] = 0.33 * B[i];

    t = clock() - t;
    double T2 = ( (double)t /CLOCKS_PER_SEC);

    printf("B:");
    print_array(B, N, 4);
    printf("\n \n");

    printf("\truntime: %f [s]\n", T2);
    /*-----fin Jacobi Stencil 1D -----------*/

    printf("Comparasion de tiempos: %f\n", T1/T2);
    return 0; 
}


// funciones de printing
void jump(int i, int b) {
    if(i%b == 0){
        printf("\n");
    }
}

void print_array(float* arr, int N, int carriage) {
    for (int i = 0; i < N; i++) {
        jump(i, carriage);
        printf("%f ", arr[i]);
    }
    printf("\n");
}
