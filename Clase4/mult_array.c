#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

int main() {

    const uint64_t m = 1 << 10;
    const uint64_t n = 1 << 10;
    const uint64_t l = 1 << 10;

    clock_t t;

    t = clock();
    // matrices de float
    float* A = malloc(sizeof(float)*m*l);
    float* B = malloc(sizeof(float)*l*n);
    float* Bt = malloc(sizeof(float)*n*l);
    float* C = malloc(sizeof(float)*m*n);

    for (uint64_t i = 0; i < m*l; i ++){
      A[i] = 0.0;
      B[i] = 0.0;
      Bt[i] = 0.0;
      C[i] = 0.0;
    }

    t = clock() - t;
    
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("init %f\n",time_taken);

    t = clock();
    for (uint64_t i = 0; i < m; i++) {
      for (uint64_t j = 0; j < n; j++) {
          float accum = 0;
          for (uint64_t k = 0; k < l; k++) {
              accum += A[i*l+k]*B[k*n+j];
          }
          C[i*n+j] = accum;
       }
    }
    t = clock() - t;

    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("naive %f\n",time_taken);

    t = clock();
    for (uint64_t k = 0; k < l; k++) {
        for (uint64_t j = 0; j < n; j++) {   
            Bt[j*l+k] = B[k*n+j];
        }
    }
    t = clock() - t;

    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("transpose %f\n",time_taken);

    t = clock();
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            float accum = 0;
            for (uint64_t k = 0; k < l; k++) {
                accum += A[i*l+k]*Bt[j*l+k];
            }
            C[i*n+j] = accum;
        }
    }
    t = clock() - t;

    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("transpose_mult %f\n",time_taken);

    return 0;
}