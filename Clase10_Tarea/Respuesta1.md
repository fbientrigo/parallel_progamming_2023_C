Consideresé una CPU con una linea de 
- cache de largo `L = sizeof(float) * 16` 
- dos matrices cuadradas $A, B \in float^{N \times N}$
	- $N = 1024$
Se calcula $C = A+B$

```
! Para esta respuesta asumiremos que cada vez que utilizamos valores de la memoria estos:
    debe cargarse la memoria al cache para pasar a la CPU
    1.- primero pasan de la memoria al cache
    2.- luego son utilizados
    3.- entonces al pedir otro valor que no esta en cache; se vacia al cargar otros
    4.- y asi el programa sigue
```

Primero se realizara una introduccion explicativa para poder aplicar la formulacion matematica

# i) calcule el numero de `cache misses` en la función `row_wise_add`:

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
- las columnas son lo que iteran mas rapiadmente; podemos imaginar la matrix como una lista plana; donde cada N floats tenemos la siguiente fila; 

$$
A = [[fila1], [fila2],...] = A_{0,0}, \dots, A_{1,0}, \dots, A_{N-1, N-1}
$$

al iterar `row_wise` pasamos de un valor al siguiente; de manera que podemos cargar en cache un grupo de valores en este caso 16 numeros `floats`, iterar sobre ellos y luego cargar los siguientes 16 y continuar el proceso.


#### realizando la cuenta por acceso a memoria por cada N datos
Por tanto si podemos cargar $l=8=2^3$ valores en cache (contando A y B) entonces podemos calcular cantidad de numeros de datos que pasaran correctamente de los totales de una linea $N=1024=2^{10}$, siendo los accesos a memoria: $\text{memory acces} = \frac{N}{l} = \frac{2^{10}}{2^3} = 2^7$.

Por tanto podemos calcular un acceso a memoria y entonces multiplicarlo por la cantidad de accesos totales

```
C[i*N+j] = A[i*N+j] + B[i*N+j];
```

- en esta suma tenemos que cargar 8 valores de `A` y 8 valores de `B`; llenando asi el cache y comenzando a escribir en la memoria de a donde apunta `C`. Osea paso a paso:
    - pedimos entonces: `A[0]`, `B[0]`
    - no estan en memoria: 1 cache miss
    - se carga toda la linea de cache, entonces `A[0:8]` y `B[0:8]` en cache
    - se realiza `C[0] = A[0] + B[0]`
    - los siguientes 7 valores al ser utilizados ya se encontraran en cache: `A[1],A[2],...,A[7]`: 7 cache hits

Tenemos asi cada acceso de memoria

$$
\text{hits/m} = 1
$$

$$
\text{miss/m} = 7
$$

Asi el total:

$$
\text{hits} = 2^7 \text{hits/m} = 2^7 = 128
$$

$$
\text{miss} = 2^7 \text{miss/m} = 7 * 2^7 = 896
$$

De esa manera el cache ratio, o el hits ratio:

$$
\text{hits}_\text{ratio} = \frac{\text{hits}}{\text{hits} + \text{miss}} = \frac{128}{1024} = 0.125
$$

#### Ahora la pregunta es sobre el total de cache miss
Se acaba de calcular los cache miss por cada N datos; por tanto al tener matrices $N \times N$, bastara con multiplicar por $N$; otra forma es usar la cantidad de miss ratio, que seria 1 menos el hits ratio y multiplicarlo por el total de calculos.

$$
\text{total miss} = \text{datos} \times (1 - \text{hits}_\text{ratio}) = N \times N (1 - 0.125)  = 1048576 (0.875) = 917504
$$

# ii) lo mismo para `col_wise_add`:

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


En el caso de col_wise_add, como iteramos sobre columnas, cada acceso a memoria accede a valores que están separados por una distancia mayor en la memoria. Esto hace que los valores no estén cerca unos de otros en el cache y, por lo tanto, es muy difícil hacer uso efectivo de la memoria cache. Por lo tanto, en este caso, no hay cache hits y todos los accesos a memoria serán cache misses.

$$
\text{hits}_\text{ratio} = 0
$$

Esto significa que para cada acceso a memoria, tendremos un cache miss, y para una matriz de tamaño $N \times N$, habrá $N^2$ accesos a memoria. Por lo tanto, el número total de cache misses será:

$$
\text{total miss} = N^2 (1-\text{hits}_\text{ratio}) = N^2 (1 - 0) = N^2
$$

___

# Experimentacion
Luego al correr el programa `t1_cache_lines.c`:

```
$ Clase10_Tarea/ g++ .\t1_cache_lines.cpp -o 1_cachemiss.exe       
$ Clase10_Tarea/ .\1_cachemiss.exe
    row_wise_add: 
    runtime: 0.016000 [s]
    col_wise_add: 
    runtime: 0.075000 [s]
$ Clase10_Tarea/ 
```

podemos observar una clara diferencia de tiempo de ejecucion, atribuido al uso de cache.
- es interesante observar que row_wise_add demoro un $21.3 \%$ del tiempo
- osea un aumento de la velocidad de 4.68
- esto solo con un hits ratio de 0.125 comparado a uno de 0.

___

# iii) Intrinsicas
usando la intrinsica `_mm256_load_ps` se pueden cargar 8 floats de un array alineado y guardarlos en una variable local `__m256 tmp`. La funcion equivalente `_mm256_store_ps` es usada para escribir `tmp` de vuelta en el arreglo original. 
	a) Encuentra el comando apropiada para agregar dos variables de tipo `__m256`.
	b) Implimenta una variante AVX-vectorizada para `row_wise_add`

iv) Comparación de tiempos de ejecución (runtime)
	a) crea una hipotesis basada en la teoria sobre los tiempos
	b) comparalos experimentalmente

