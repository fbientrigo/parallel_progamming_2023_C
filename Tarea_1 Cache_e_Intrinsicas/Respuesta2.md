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
\text{hits/m} = 7
$$

$$
\text{miss/m} = 1
$$

Asi el total:

$$
\text{hits} = 2^7 \text{hits/m} = 7 * 2^7 = 896
$$

$$
\text{miss} = 2^7 \text{miss/m} = 1 * 2^7 = 128
$$

De esa manera el cache ratio, o el hits ratio:

$$
\text{hits}_\text{ratio} = \frac{\text{hits}}{\text{hits} + \text{miss}} = \frac{896}{1024} = 0.875
$$

#### Ahora la pregunta es sobre el total de cache miss
Se acaba de calcular los cache miss por cada N datos; por tanto al tener matrices $N \times N$, bastara con multiplicar por $N$; otra forma es usar la cantidad de miss ratio, que seria 1 menos el hits ratio y multiplicarlo por el total de calculos.

$$
\text{total miss} = \text{datos} \times (1 - \text{hits}_\text{ratio}) = N \times N (1 - 0.875)  = 1048576 (0.125) = 131072
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

Siendo así el total de cache miss:

$$
\text{total miss} = 1048576 
$$

___

# Experimentacion
Luego al correr el programa `t2_cache_lines.c`:

```
$ Clase10_Tarea/ g++ .\t2_cache_lines.cpp -o 2_cachemiss.exe       
$ Clase10_Tarea/ .\2_cachemiss.exe
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

#a 
El comando apropiado para cargar variables de tipo "__m256" depende de si trabajamos con floats o doubles; ya que al final llevara el nombre
- `_mm256_load_ps` quiere decir `packed single`, para floats
- `_mm256_load_pd` quiere decir `packed double`, para doubles

#b
La implementación de la variante vectoriazada:

```c
// row-major-order addition VECTORIZADA 
void avx_row_wise_add(float* A, float* B, float*C, uint64_t N) {

    __m256 temp; // variable temporal para guardar antes de pasar a C
	for(uint64_t i = 0; i<N; i++) {
        for(uint64_t j = 0; j<N; j += 8){

            //guardamos datos en los registros vectorizados
            __m256 AV = _mm256_load_ps(A+i*N+j);
            __m256 BV = _mm256_load_ps(B+i*N+j);
            temp = _mm256_add_ps(AV,BV); // y los sumamos en el tmp
            //para entonces guardarlos en C
            _mm256_store_ps(C+i*N+j, temp);
        }
    }
}
```


### Experimentación
Comparando varias veces con N=8:
```
$ Clase10_Tarea/ gcc .\t2_c_cache_avx.c -o 2_avx_rowadd.exe -march=native
$ Clase10_Tarea/ .\2_avx_rowadd.exe
    row_wise_add:        
    runtime: 0.003000 [s]
    avx_row_wise_add:    
    runtime: 0.002000 [s]
$ Clase10_Tarea/ .\2_avx_rowadd.exe
    row_wise_add:        
    runtime: 0.002000 [s]
    avx_row_wise_add:    
    runtime: 0.001000 [s]
$ Clase10_Tarea/ .\2_avx_rowadd.exe
    row_wise_add: 
    runtime: 0.004000 [s]
    avx_row_wise_add:    
    runtime: 0.001000 [s]
$ Clase10_Tarea/ .\2_avx_rowadd.exe
    row_wise_add:        
    runtime: 0.002000 [s]
    avx_row_wise_add:    
    runtime: 0.001000 [s]

```


# iv) Comparación de tiempos de ejecución (runtime)
	a) crea una hipotesis basada en la teoria sobre los tiempos comparando la versión clasica y la intrinsica
	b) comparalos experimentalmente

#a 
La intrinsica usada es `_mm256_add_ps`

```
Intel® Intrinsics Guide. (2023). Intel. https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#text=_mm256_add_ps&ig_expand=158
```

Esta posee una latencia entre 2 y 4, lo cual se refiere a la cantidad de clocks de cpu que demorará en estar listo para otro proceso. Throughput es exclusivamente el clock de cpu que demora en calcular o ejecutar su calculo.
Si el procesador tiene multiples unidades de ejecución podemos aprovecharnos del throughtput;
por ejemplo si tenemos la arquitectura `skylake` con una latencia de 4 y un throughput de 0.5: entonces podemos realizar 8 calculos antes del tiempo de latencia dividiendolo en las unidades de ejecución; osea que seriamos capaces de hacer threading.

Como conclusión:
- Latencia: Tiempo de espera si un calculo depende de el anterior
- Reciprocal throughput: digamos que no hay dependencia entre un calculo y otro, entonces podemos calzar un mayor numero de operaciones

en el caso de querer comparar la latencia de cada operación con sus equivalentes en assembly seria solo una aproximación pues en la cpu son operaciones que pueden ocurrir de manera desordenada y aveces puede tomar más o menos tiempo; he aqui una ista con latencias y reciprocal throughputs:

```
Fog, A. (2022). Introduction 4. Instruction tables. https://www.agner.org/optimize/instruction_tables.pdf
```

Pero si solo nos quedamos a analizar el codigo escrito en `t2_c_cache_avx.c` y comparar las funciones; 
entonces tengamos el siguiente analisis:

- asumiremos que una suma de 2 floats ocurre en un tiempo de computo $\beta$
- un avx lo que hace es tener estos 8 elementos y sumarlos de manera paralela
    - asumiremos que demora el mismo tiempo que sumar dos floats $\beta$ para comparación
    - comunicar un elemento al registro AVX toma un tiempo de comunicación $\alpha$
- tenemos N=1024, asi que tendremos $N \times N =1024^2$


Por tanto 

```c
// row-major-order addition
void row_wise_add(float* A, float* B, float*C, size_t N) {
	for(int i = 0; i<N; i++)
		for(int j = 0; j<N; j++)
			C[i*N+j] = A[i*N+j] + B[i*N+j];
}
```
Sumamos dos floats, dentro de un loop de largo N en otro loop de largo N:

$$
T = \text{Tiempo total} = \beta \times N^2
$$


```c
// row-major-order addition VECTORIZADA 
void avx_row_wise_add(float* A, float* B, float*C, uint64_t N) {

    __m256 temp; // variable temporal para guardar antes de pasar a C
	for(uint64_t i = 0; i<N; i++) {
        for(uint64_t j = 0; j<N; j += 8){

            //guardamos datos en los registros vectorizados
            __m256 AV = _mm256_load_ps(A+i*N+j);
            __m256 BV = _mm256_load_ps(B+i*N+j);
            temp = _mm256_add_ps(AV,BV); // y los sumamos en el tmp
            //para entonces guardarlos en C
            _mm256_store_ps(C+i*N+j, temp);
        }}
}
```

Tenemos dos tipos de operaciones, solo analizaremos la latencia:
- 2 Comunicaciones *independientes* a registros AVX: $t=\alpha$
- 1 Suma de registros: $t=\beta$
- 1 Suma de registros AVX a arrays $t=\alpha$

Osea que por cada loop interno tenemos
$ \sum t = 2 \alpha + \beta $

El largo del Loop interior es de $N/8$ y el exterior es de $N$

```c
for(uint64_t i = 0; i<N; i++)
    for(uint64_t j = 0; j<N; j += 8)
```

por tanto:

$$
T_{avx} = \text{Tiempo total} = (2 \alpha + \beta) \times \frac{N}{8} \times N
$$

Comparando entonces las velocidades, puede interpretarse como un speed up, tomemos el ratio de los tiempos:

‌$$
\eta = \frac{ T_{avx} }{T} = \frac{(2 \alpha + \beta) \times N^2 }{ 8 \beta \times N^2}  $$


$$
\eta = \frac{1}{4} \frac{\alpha}{\beta} + \frac{1}{8}
$$

Ahora de acuerdo a la guia de instrinsics de Intel:
- `_mm256_load_ps` tiene una latencia de $\alpha = 7$
- `_m256_add_ps` tiene una latencia de $\beta = 4$
- `_mm256_store_ps` tiene una latencia de $\alpha = 5$

Entonces tomando el promedio de $\alpha = 6$, el ratio seria de $\frac{\alpha}{\beta} = \frac{6}{4}= \frac{3}{2}$

$$
\eta = \frac{1}{4} \frac{3}{2} + \frac{1}{8} = \frac{4}{8} = \frac{1}{2}
$$

Por tanto el speed up seria:

$$
S = \frac{T}{T_{avx}} =\frac{1}{\eta} = 2
$$

```bash
$ Clase10_Tarea/ .\2_speed_avx_rowadd.exe
row_wise_add:
        runtime: 0.011000 [s]
avx_row_wise_add:
        runtime: 0.005000 [s]
Speedup: 2.200000
$ Clase10_Tarea/ .\2_speed_avx_rowadd.exe
row_wise_add:
        runtime: 0.009000 [s]
avx_row_wise_add:
        runtime: 0.004000 [s]
Speedup: 2.250000
$ Clase10_Tarea/ .\2_speed_avx_rowadd.exe
row_wise_add:
        runtime: 0.008000 [s]
avx_row_wise_add:
        runtime: 0.006000 [s]
Speedup: 1.333333
```

Lo que se compara excelente con lo experimental; podemos entonces considerar que se han utilizado buenas aproximaciones y consideraciones en el calculo teorico.