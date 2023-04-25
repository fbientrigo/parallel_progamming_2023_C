fecha: 23 / 04 / 21 (2023, abril, 2021)
tags: sumas vectoriales

# Tema principal
Se utiliza el programa `avx_mm.c`; el cual realiza sumas horizontales y extraer floats de los registros para asi sumar matrices


## funciones especiales

- `_mm256_...`: podemos cargar memoria de manera alineada para la vectorizacion, osea cargar los datos del RAM al registro de manera alineada para la vectorizacion
- `_mm256_loadu_ps`: carga a la memoria sin necesidad de tener alineacion
- `_mm256_add_ps`y `_mm256_sub_ps`: funciones para sumar y restar

## notas de notacion
el usar el corredor de bits:
```c
int M = 1 << 6; 
```
lo movemos 6 bits a la izquierda, por tanto el 1 es $2^0$ y lo que hacemos es sumarle 6 al exponente de 2: $2^6$

$$
M = 2^6 = 64
$$

## cargar valores a un registro para Poisson
Para cargar datos hemos de cargar un cuadrado de a 4:
carga 4 valores consecutivos
```c
__m256 u_ij = _mm256_loadu_pd(&u[i*N+j])
```

NOTA: la paralelizacion se hace en la direccion x; por ello separamos en vectores las otras direccion

Luego cargamos la funcion de densidad
```

```

podemos obtener la diferencia con la resta vectorial y se puede asi obtener la diferencia cuadrada:
```c
diff_ij = _mm256_mul_pd(diff_ij, diff_ij);
```

Luego podemos hacer una suma horizontal de la suma:
```c
diff_ij = _mm256_had_pd(diff_ij, diff_ij);
```

___

