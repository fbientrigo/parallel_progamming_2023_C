#include "stdio.h"
#include "stdlib.h"
#include "limits.h"


int main(int argc, char** argcv) {
    //tipos basicos
    int numero1 = -3;
    unsigned int numero2 = 4;
    float numero3 = 3.1415;
    double numero4 = 3.1415;

    printf("%d %u %.10f %lf \n", numero1, numero2, numero3, numero4);


    return 0;
}