#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

/*
Operaciones entre arrays
y sus tiempos

*/

int main() {

    // correr 10 bits a la izquierda el uno, osea
    // 2^10 = 1024
    const uint64_t m = 1 << 10;
    const uint64_t n = 1 << 10;
    const uint64_t l = 1 << 10;

    printf("m: %d, n: %d, l: %d \n", m,n,l);
    printf("m * l = %d \n", m*l);
    printf("n * l = %d \n", n*l);

    clock_t t;

    t = clock();
    // matrices de float
    float* A = malloc(sizeof(float)*m*l);
    float* B = malloc(sizeof(float)*l*n);
    float* Bt = malloc(sizeof(float)*n*l);
    float* C = malloc(sizeof(float)*m*n);

    // inicializar componente por componente
    for (uint64_t i = 0; i < m*l; i ++){
      A[i] = 0.0;
      B[i] = 0.0;
      Bt[i] = 0.0;
      C[i] = 0.0;
    }

    t = clock() - t;
    
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("inicializar matrices A,B,C: %f [s]\n",time_taken);

    // comenzamos a sumar
    t = clock();
    for (uint64_t i = 0; i < m; i++) {
      for (uint64_t j = 0; j < n; j++) {
          float accum = 0;
          for (uint64_t k = 0; k < l; k++) {
							// k y k+1 se separan por l*sizeof(float)
							// osea que no estan en la misma linea
							// osea no son contiguos
              accum += A[i*l+k]*B[k*n+j];
          }
          C[i*n+j] = accum;
       }
    }
    t = clock() - t;

    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("C = A . B; metodo ingenuo: %f [s]\n",time_taken);

		/*
		si primero definimos una matriz traspuesta;
		entonces la multiplicacion de matrices sera contigua
		*/

    t = clock();
    for (uint64_t k = 0; k < l; k++) {
        for (uint64_t j = 0; j < n; j++) {   
            Bt[j*l+k] = B[k*n+j];
        }
    }
    t = clock() - t;

    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Calculando B^T , la transpuesta: %f [s]\n",time_taken);

		/*
		Ahora realizamos la operacion
		C = A * BT, de manera que los arrays se recorren igual

		lo importante esta en que internamente los elementos estan uno junto a otro
		entonces la CPU tendra en el cache gran cantidad de filas

		esto sucede internamente, y hemos de entender el comportamiento
		para ser capaces de utilizarlo
		
		*/
    t = clock();
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            float accum = 0;
            for (uint64_t k = 0; k < l; k++) {
							// tener [.. + k], con el iterador mas rapido
							// es sinonimo de contiguidad en la memoria
							// por tanto el cache sera capaz de guardar mas partes
                accum += A[i*l+k]*Bt[j*l+k];
            }
            C[i*n+j] = accum;
        }
    }
    t = clock() - t;

    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("C = A * BT: %f [s] \n",time_taken);

    return 0;
}