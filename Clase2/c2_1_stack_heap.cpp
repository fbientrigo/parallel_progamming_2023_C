#include <stdio.h> // 
#include <stdlib.h>

#define N 100000  // como definiremos los arrays

int main(){
    double* x;
    x = (double*) malloc(N * sizeof(double));
    printf("%lu bytes\n", sizeof(x));
}
    