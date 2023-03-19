#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    printf("La cantidad de argumentos %d\n",argc);
    printf("Hola Mundo! El numero es %c\n",*argv[1]);

    int numero;
    if (argc < 2 || argc > 2){
        printf("Para correr el programa escribir: ./name.exe <numero>\n");
        exit(1);
    }

    numero = atoi(argv[1]);

    printf("El numero es %d\n",numero);
}