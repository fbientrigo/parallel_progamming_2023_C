#include <stdio.h>
#include <wchar.h>

int main() {
    // Definir el nombre del archivo y el modo de apertura
    char *nombreArchivo = "2_datos.csv";
    char *modo = "r";

    // Abrir el archivo en modo de lectura
    FILE *archivo = fopen(nombreArchivo, modo);

    // Verificar si ocurrió un error al abrir el archivo
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s", nombreArchivo);
        return 1;
    }

    // Variables para almacenar los datos leídos
    double tiempo;
    double posicion;

    // Leer el encabezado del archivo CSV (opcional)
    char encabezado[100];
    fgets(encabezado, sizeof(encabezado), archivo);

    // Leer datos del archivo CSV hasta el final
    while (fscanf(archivo, "%lf,%lf", &tiempo, &posicion) == 2) {
        // Procesar los datos leídos, por ejemplo, imprimirlos en la consola
        wprintf(L"Tiempo: %.2lf, Posición: %.2lf\n", tiempo, posicion);
    }

    // Cerrar el archivo
    fclose(archivo);

    return 0;
}
