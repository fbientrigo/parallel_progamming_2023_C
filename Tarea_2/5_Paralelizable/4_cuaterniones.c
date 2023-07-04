#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <omp.h>

// Definición de la estructura Quaternion
typedef struct {
    double w;
    double x;
    double y;
    double z;
} Quaternion;

// Función para multiplicar dos cuaterniones (paralelizable)
Quaternion multiplyQuaternions(Quaternion a, Quaternion b) {
    Quaternion result;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
        }
        #pragma omp section
        {
            result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
        }
        #pragma omp section
        {
            result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
        }
        #pragma omp section
        {
            result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
        }
    }

    return result;
}

int main() {
    int N = 4; // Tamaño de los vectores
    Quaternion* vector1 = (Quaternion*)malloc(N * sizeof(Quaternion));
    Quaternion* vector2 = (Quaternion*)malloc(N * sizeof(Quaternion));
    Quaternion* producto = (Quaternion*)malloc(N * sizeof(Quaternion));

    // Inicializar y mostrar los valores de los vectores
    for (int i = 0; i < N; i++) {
        vector1[i].w = i + 1;
        vector1[i].x = i + 2;
        vector1[i].y = i + 3;
        vector1[i].z = i + 4;

        vector2[i].w = i + 5;
        vector2[i].x = i + 6;
        vector2[i].y = i + 7;
        vector2[i].z = i + 8;

        printf("Vector 1[%d]: (%f, %f, %f, %f)\n", i, vector1[i].w, vector1[i].x, vector1[i].y, vector1[i].z);
        printf("Vector 2[%d]: (%f, %f, %f, %f)\n", i, vector2[i].w, vector2[i].x, vector2[i].y, vector2[i].z);
    }

    // Multiplicar los cuaterniones (paralelizado)
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        producto[i] = multiplyQuaternions(vector1[i], vector2[i]);
    }

    // Mostrar los resultados
    for (int i = 0; i < N; i++) {
        printf("Producto[%d]: (%f, %f, %f, %f)\n", i, producto[i].w, producto[i].x, producto[i].y, producto[i].z);
    }

    free(vector1);
    free(vector2);
    free(producto);

    return 0;
}
