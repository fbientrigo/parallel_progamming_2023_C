#include <stdio.h>

int main() {
    char *nombreArchivo = "2_datos.csv";
    char *modo = "w";
    FILE *archivo = fopen(nombreArchivo, modo);
    if (archivo == NULL) {
        printf("Error abriendo archivo %s", nombreArchivo);
        return 1;
    }

    // Arreglo de tiempo
    double tiempo[] = {0.0, 0.1, 0.2, 0.3, 0.4};
    int numTiempo = sizeof(tiempo) / sizeof(tiempo[0]);

    // Arreglo de posiciones
    double posiciones[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    int numPosiciones = sizeof(posiciones) / sizeof(posiciones[0]);

    // Escribir encabezado
    fprintf(archivo, "Tiempo,Posicion\n");

    // Escribir datos en el archivo CSV
    for (int i = 0; i < numTiempo; i++) {
        fprintf(archivo, "%lf,%lf\n", tiempo[i], posiciones[i]);
    }

    fclose(archivo);
    puts("Datos escritos correctamente en el archivo CSV.");
    return 0;
}
