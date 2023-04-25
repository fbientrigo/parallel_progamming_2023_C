fecha: 23 04 21
tags: openmp, threads, hilos

# Paralelizar mediante hilos
Tipicamente queremos darle un hilo a cada procesador; pero si gustamos podemos ejecutar la cantidad de threads que queramos, 
- sin embargo tendriamos ejecucion secuencial de estos

```c
#include <stdio.h>

int main(){
    #pragma omp parallel
    // esta linea de codigo se paralelizara en hilos
}

```
- el uso de `#` provoca que el pre procesador ejecute otras intrucciones de acuerdo a OpenMP

## Directivas:
Es posible decirle al pre procesador que en ciertas partes no es necesario openmp

```c
#if defined(_OPENMP) 
#include <open.mp>
#endif

#if defined(_OPENMP)
    // codigo con openmp
#else
    // codigo que no necesita openmp


```

## Parallel for
Restricciones:
- no se puede modificar el valor del indice reasignandole nuevos valores dentro del cuerpo

## OpenMP
(codigo vector_addition)

- `omp_get_wtime()` una funcion para obtener el tiempo actual en lugar de usar clock y ore calcular el tiempo con los ciclos por segundo
- sobre el speedup ideal, no lo alcanzaremos pues tambien hay un costo en lanzar los threads y dividir los datos

- en la suma de un par de vectores tenemos un `embarassingly parallel` pues no hay inter dependencia; es tan facil que se demora mas en acceder a la memoria que en los calculos: `memory-bound`

- concepto de `race-conditions` en donde dos hilos intentan acceder a la misma locacion o un calculo depende de que las tareas se dividan y ejecutan secuencialmente. un ejemplo de esto puede ser en:

```c
A[i+1] = A[i] + B[i]
```

- concepto de variables privadas y compartidas, en este codigo tendriamos modificaicones raras
```
int i,j

#pragma omp parallel for
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)

```
