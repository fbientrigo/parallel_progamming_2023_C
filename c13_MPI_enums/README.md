# Matrix Multiplication with submatrix scattering
Se trata de paralelizar la multiplicacion de matrices separandola en partes
el algoritmo implica duplicar 2 veces la informacion de la matriz

GLosario:
- enums
    los enums son similars a los diccionarios de python

Existen funciones y enums (estructuras de datos) con MPI:
- MPI_Datatype
- MPI_Send
    utilizado para enviar datos
- MPI_Recv
    utilizado para recibir datos
- MPI_Type_free
    util para liberar memoria

## Tarea
Se relaciona con la Jacobi Iteration
de manera de tener 6 procesos, la forma mas facil de enivar columnas es con el tipo vector de MPI

