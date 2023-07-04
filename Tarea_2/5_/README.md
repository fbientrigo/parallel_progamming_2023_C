# 5- Cuales de las siguientes operaciones pueden ser paralelizadas con OpenMP?
### (i) divisor comun maximo entre 2 o más enteros

$$
\text{gcd}(a_0, a_1, \dots)
$$

Es facilmente paralelizable pues el algoritmo implica buscar entre todos los numeros desde el 1 hasta el $a_j$ menor de todos los enteros:

```c
#include <stdbool.h> // booleanos sobre C99 compiladores

a_0 // es el numero más pequeño

A // el arreglo que contiene a todos los enteros, de size N

#pragma omp parallel for schedule(guided)
for (int k = 1; k <= a_0; k++){
    bool divides_all = true;
    for (int i = 0; i < N; i++){ 
        if(A[k] % k != 0) {
            divides_all = false;
            break;
        } 
    }
    if(divides_all)

}
```