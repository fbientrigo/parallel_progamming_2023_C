# 5- Cuales de las siguientes operaciones pueden ser paralelizadas con OpenMP?
Se asumirá que se refiere a paralelizar *directamente* una implementación con OpenMP

### (i) divisor comun maximo entre 2 o más enteros
$$
\text{gcd}(a_0, a_1, \dots)
$$

Es facilmente paralelizable pues el algoritmo implica buscar entre todos los numeros desde el 1 hasta el $a_j$ menor de todos los enteros:

```c
#include <stdbool.h> // booleanos sobre C99 compiladores

int a_0 = ...; // es el numero más pequeño
int* A = ...; // el arreglo que contiene a todos los enteros, de size N

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
        break
}

// k sera el gcd
```

### (ii) Producto entre dos matrices 2x2
Ya que el producto de cada elemento depende de los elementos de la matriz de entrada,  no será un calculo independiente, no hay entonces una implementación directa con OpenMP.

Si se desea realizar es posible dividir las matries de entrada en bloque, pero habria que ser cuidadosos en la creación de este algoritmo.

### (iii) El producto complejo entre dos o más numeros complejos

```c
#include <complex.h>

int N = ...; 
double complex* vector1 = ...; 
double complex* vector2 = ...; 
double complex* producto = ...; 

#pragma omp parallel for
for (int i = 0; i < N; i++) {
    producto[i] = vector1[i] * vector2[i];
}
```

### (iv) El producto entre dos o más cuaterniones
Cito el codigo adjunto que escribí luego de investigar `4_cuaterniones.c`

```bash
$ 5_/ gcc .\4_cuaterniones.c -fopenmp -o cuaternions.exe
$ 5_/ .\cuaternions.exe
Vector 1[0]: (1.000000, 2.000000, 3.000000, 4.000000)
Vector 2[0]: (5.000000, 6.000000, 7.000000, 8.000000)
Vector 1[1]: (2.000000, 3.000000, 4.000000, 5.000000)
Vector 2[1]: (6.000000, 7.000000, 8.000000, 9.000000)
Vector 1[2]: (3.000000, 4.000000, 5.000000, 6.000000)
Vector 2[2]: (7.000000, 8.000000, 9.000000, 10.000000)
Vector 1[3]: (4.000000, 5.000000, 6.000000, 7.000000)
Vector 2[3]: (8.000000, 9.000000, 10.000000, 11.000000)
Producto[0]: (-60.000000, 12.000000, 30.000000, 24.000000)
Producto[1]: (-86.000000, 28.000000, 48.000000, 44.000000)
Producto[2]: (-116.000000, 48.000000, 70.000000, 68.000000)
Producto[3]: (-150.000000, 72.000000, 96.000000, 96.000000)
```


En donde la primera operación la comprobe con una calculadora:
```
q₁ = 1 + 2i + 3j + 4k and q₂ = 5 + 6i + 7j + 8k 
q₁ * q₂ = -60 + 12i + 30j + 24k 
```
Ref: Quaternion Calculator. (2023). Omnicalculator.com. https://www.omnicalculator.com/math/quaternion


### (v) la ecuación incremental
$$
(a,b) \rightarrow \frac{a+b}{2}
$$
Ya que es incremental, el calculo $n$ dependeria del resultado de $n-1$, por tanto no puede ser implementada directamente.