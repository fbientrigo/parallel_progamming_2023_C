#include <stdio.h>

int main() {
    // Definir el nombre del archivo y el modo de apertura
    char *nombreArchivo = "1_archivo.txt";
    char *modo = "w";

    // Abrir el archivo en modo de escritura
    FILE *archivo = fopen(nombreArchivo, modo);

    // Verificar si ocurrió un error al abrir el archivo
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s", nombreArchivo);
        return 1;
    }

    // Escribir contenido en el archivo
    fprintf(archivo, "Hola, este es un archivo de texto.\n");
    fprintf(archivo, "Aquí podemos escribir cualquier información que queramos guardar.\n");
    fprintf(archivo, "Por ejemplo, números, cadenas de texto, etc.\n");

    // Ejemplo: Escribir números
    int numero = 123;
    fprintf(archivo, "Número: %d\n", numero);

    // Ejemplo: Escribir una cadena de texto
    char *cadena = "Este es un ejemplo de cadena.";
    fprintf(archivo, "Cadena: %s\n", cadena);

    // Ejemplo: Escribir valores en formato de coma flotante
    float flotante = 3.14159;
    fprintf(archivo, "Valor flotante: %.2f\n", flotante);

    // Cerrar el archivo
    fclose(archivo);

    // Mostrar mensaje de éxito
    printf("El archivo %s ha sido escrito correctamente.\n", nombreArchivo);

    return 0;
}
