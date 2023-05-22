# Escribir

```c
    char *nombreArchivo = "nombre_archivo.txt";
    char *modo = "w"; // w para escribir, a para agregar
    FILE *archivo = fopen(nombreArchivo, modo);
    if (archivo == NULL) {
        printf("Error abriendo archivo %s", nombreArchivo);
        return 1;
    }

    // como escribir:
    for (int i = 0; i < numTiempo; i++) {
        fprintf(archivo, "%lf,%lf\n", tiempo[i], posiciones[i]);
    }

    // ...
    fclose(archivo);
```


Archivo `c3_1`:
- Se define el nombre del archivo y el modo de apertura.
- El archivo se abre en modo de escritura utilizando la función fopen.
- Se verifica si ocurrió algún error al abrir el archivo.
- Se utilizan las funciones fprintf para escribir contenido en el archivo. En este caso, se escriben algunas líneas de texto ejemplificando el tipo de información que se puede almacenar.
- El archivo se cierra utilizando la función fclose.
- Se muestra un mensaje de éxito indicando que el archivo ha sido escrito correctamente.

Archivo `c3_2`:
- En este ejemplo, se crea un archivo CSV llamado "datos.csv" en modo de escritura. Luego, se declaran dos arreglos: uno para el tiempo y otro para las posiciones. Cada arreglo contiene varios elementos.
- El programa escribe un encabezado en el archivo CSV para indicar qué columnas representan los datos. Luego, se itera sobre los arreglos de tiempo y posiciones, escribiendo cada par de valores en una nueva línea del archivo CSV.
- Recuerda que en un archivo CSV, los valores suelen separarse por comas (,) y cada registro se coloca en una nueva línea.
- Al ejecutar este programa, se generará el archivo "datos.csv" con el contenido de los arreglos de tiempo y posiciones.

