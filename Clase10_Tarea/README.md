# Tarea 1 de Programación Paralela
Es una lista de ejercicios que provienen del libro del curso;
haz el intento de completarlos

Mi respuesta se encuentra en los distintos archivos.c

# 1.- Cache Lines y Vectorización
Consideresé una CPU con una linea de 
- cache de largo `L = sizeof(float) * 16` 
- dos matrices cuadradas $A, B \in float^{N \times N}$
	- $N = 1024$
Se calcula $C = A+B$, osea 

$$
C_{ij} = A_{ij} + B_{ij}
$$

i) calcule el numero de `cache misses` en la función `row_wise_add`:

```c++
// row-major-order addition
void row_wise_add(float* A, float* B, float*C, size_t N) {
	// i: fila
	// j: columna
	for(int i = 0; i<N; i++)
		for(int j = 0; j<N; j++)
			C[i*N+j] = A[i*N+j] + B[i*N+j];
}
```

ii) lo mismo para `col_wise_add`:

```c++
// col-major-order addition
void col_wise_add(float* A, float* B, float*C, size_t N) {
	// i: columna
	// j: fila
	for(int i = 0; i<N; i++)
		for(int j = 0; j<N; j++)
			C[j*N+i] = A[j*N+i] + B[j*N+i];
}
```

iii) usando la intrinsica `_mm256_load_ps` se pueden cargar 8 floats de un array alineado y guardarlos en una variable local `__m256 tmp`. La funcion equivalente `_mm256_store_ps` es usada para escribir `tmp` de vuelta en el arreglo original. 
	a) Encuentra el comando apropiada para agregar dos variables de tipo `__m256`.
	b) Implimenta una variante AVX-vectorizada para `row_wise_add`

iv) Comparación de tiempos de ejecución (runtime)
	a) crea una hipotesis basada en la teoria sobre los tiempos
	b) comparalos experimentalmente


# 2.- "Loop Unrolling for Max Reduction" (pg 73)
La siguiente función (secuencial) computa el maxio de un arreglo de floats:

```c++
float plain_max(float* data, uint64_t lenght) {
	float max = - INFINITY;

	for(uint64_t i=0; i<lenght; i++)
		max = std::max(max, data[i]);

	return max;
}
```




# 3.- "Vectorized Jacobi 1D Stencil" (pg 75)
La iteración de Jacobi es un metodo con gran variedad de aplicaciones. Un simple stencil de 3 puntos se puede implementar con el siguiente fragmento de codigo:

```c++
for(uint64_t t=0; t<T; t++) {
	for(uint64_t i=0; i<N-1; i++)
		B[i] = 0.33 * (A[i-1] + A[i] + A[i+1]);
	for(uint64_t i=0; i<N-1; i++)
		A[i] = B[i];
}
```
el primer loop interno realiza la computacion stencil; mientras que el segundo loop interno copia el output array en el input array para la siguiente iteración

i) analyza las `data dependencies` en el codigo secuencial

ii) propon una `data layout transformation` para realizar una vectorización eficiente del codigo

iii) que intrinsics son necesarias para tu implementación de vectorización


