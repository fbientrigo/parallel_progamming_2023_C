# Memory Mangement

Se utiliza una libreria llamada

```c
#include<immintrin.h>
```

en especifico usamos Advanced Vector Extension -> AVX2 de Intel:

```c
void avx2_vecadd(float *A, float *B, float *C, uint64_t N){
    /*instrucciones avx2 dados por intel; permite agregar elementos en paralelo
    AVX2 -> advanced vector extension 2

    permite 
    
    */
    __m256 X;
    for (uint64_t i=0; i<N; i += 8){
        __m256 AV = _mm256_load_ps(A+i);
        __m256 BV = _mm256_load_ps(B+i);
        X = _mm256_add_ps(AV,BV);
        _mm256_store_ps(C+i, X);

    }
}
```
esto permite `SIMD` single instructions multiple data; proceso para 256-bit-floating-point data;
- un float son `4 byes`
- osea que puede contener 8 floats

```
256 bits =  32 bytes
```

Estas son operaciones que vienen dentro de los procesadores, podemos definir registros AVX, recordemos si estos registros son de 256 bits, q:

```c
__m256 a,b,c; // registros AVX

c = _mm256_add_ps(a,b); // c[0:&] = a[0:8] + b[0:&]
```


## Trabajo:

- aplicar tecnicas de manejo de memoria avanzadas; para la ecuación de Poisson con el metodo de Jacobi