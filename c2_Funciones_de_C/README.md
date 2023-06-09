#Clase 2

## Arreglos multidimensionales
Poblar o iterar sobre un arreglo bidimensional, solemos pensarlos de manera uni dimensional
```
for (int i=0; i<xN; i++){
    for (int j=0; j<yN; j++){
        // array[i][j] = i + j; // es equivalente a realizar lo de abajo
        array[i * Ny + j] = i+j;  
    }
}
```
Pues asi nos aproximamos mas al modelo que tiene la computadora fisica de los datos.

# Paralelizacion y la idea del curso
La idea del curso es paralelizar programas, metodos de machine learning, paralelizaciones, programas que requieran de muchos calculos.

Tendremos ciclos extremadamente largos, la paralelizacion es comunmente dividir el trabajo de los ciclos en distinos hilos llamados `threads`

es posible utilizar `OpenMP` facilmente, ya viene pre instalado en `gcc`:

```c++
#include "stdio.h"
#include "omp.h"

int main(){

    // se usa para compilar en paralelo el siguiente bloque
    #pragma omp parallel
    {
       int n;
       n = omp_get_num_threads();

       printf("Numero threads: %d\n",n);
    }

}
```

la parte de compilar es importante, ya que requiere especificar que utilizamos `OpenMP`, es el flag `-fopenmp`

```bash
g++ -fopenmp .\c2_8_librerias.cpp -o 8_librerias.exe
```

### Incluir flags personalizados en CMake
Basta con modificar la `CMakeLists.txt`, agregando las flags en las variables `CMAKE_C_FLAGS` y `CMAKE_CXX_FLAGS`

```cmake
cmake_minimum_required(VERSION 3.1)

project(myproject)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(myexecutable main.c)

```



## Optimizacion
Es buena idea partir por la optimizacion antes de paralelizar;
aprender los algoritmos a utilizar y su eficiencia es algo netamente matematico, muchas veces facil de aplicar es algo poderoso.

Es posible incluso optimizar en el momento de compilar
```
gcc .\c2_9_tiempo.c -o tiempo.exe -O0
```
sin el uso de optimizaciones : `El tiempo: 0.312000`

el compilador analiza el estado del programa en cada linea de ejecucion, la optimizacion cambia el programa pero el resultado es identico:
```
gcc .\c2_9_tiempo.c -o tiempo.exe -O3
```
Con el uso de optimizaciones : `El tiempo: 0.091000`

### object code
Permite pasar nuestro codio a lenguaje de maquina, reduciendo la abstracciones innecesarias y dejando lo importante

Generamos un object file:
```
gcc .\c2_9_tiempo.c -c -O3
```
estamos generando un objet file `c2_0_tiempo.o` utilizando la flag `-c`

```
objdump -d .\c2_9_tiempo.o
```
es increible todo lo que se obtiene;

en resumen, cuando escribimos un programa queremos buscar posibilidades de optimizacion, optimizacion via algoritmos o via compilador.


## Instalar Librerias
El profe se encargara de que tengamos cuentas para KOSMOS, y ahi se procedera a continuar esa parte.


# Metricas
### Speedup:

$$
S = \frac{T(1)}{T(p)}
$$

donde $T(p)$ es el tiempo de `p` procesadores

### Eficiencia:
El speedup per procesador

$$
E = \frac{S}{p}
$$

### Costo

$$
C = T(p) * p
$$

## Conceptos como
- Escalabilidad, fuerte: estamos hablando de cuanto cambia la eficiencia de la paralelizacion con una cantidad fija de datos variando p.
Al pedir tiempo de computacion de un super cluster, se pide un analizis de escalabilidad para ver si vale la pena que nos entreguen mas procesadores, eventualmente se llegara a un peak donde no ayude poner mas procesadores.

- Escalabilidad, debil: como cambia la eficiencia, variando la cantidad de datos y p.
Esta version es mas complicada porque variamos 2 cantidades;

- Razon de computa-a-comunicacion: este es el concepto por el que la paralelizacion no siempre funciona, a veces necesitamos comunicacion entre los distintos procesos; este tiempo de comunicacion posee un coste.
Queremos tener el porcentaje maximo de computo y minimizar la comunicacion (que es tiempo perdido).

