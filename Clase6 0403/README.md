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

## Importancia del tipo de dato usado

Los for y las sumas se componen asi:

```c
    __m256 X;
    for (uint64_t i=0; i<N; i += 8){
        __m256 AV = _mm256_load_ps(A+i);
        __m256 BV = _mm256_load_ps(B+i);
        X = _mm256_add_ps(AV,BV);
        _mm256_store_ps(C+i, X);

    }
```

En el caso ed que usemos floats, osea de que A, B y C sean floats, entonces caberan 8 datos dentro de ellos; pues cada float es de 4 bytes y tenemos 32 bytes dentro de `__m256`

En cambio se usamos doubles; hemos de cambiar a 4 la linea, o si no habran espacios donde no habran datos

```c

void avx2_vecadd(double *A, double *B, double *C, uint64_t N){
    /* notar como se escribe __m256d por double precission
    */
    __m256d X;
    for (uint64_t i=0; i<N; i += 4){
        // _pd packed double precission
        __m256d AV = _mm256_load_pd(A+i);
        __m256d BV = _mm256_load_pd(B+i);
        X = _mm256_add_pd(AV,BV);
        _mm256_store_pd(C+i, X);

    }
}
```




## Mucha mas velocida
en este seccion de codigo tenemos un output de tiempo de:

```c
    begin = clock(); // ================== clock ======

    double* rho = (double*) _mm_malloc(sizeof(double)*N*N, 64); 
    llenar_rho_avx2(rho, N, L, dx, a);

    end = clock();
    time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
    printf("Avx definition and fill of rho; %f\n", time_spent); 
    // ================== end clock =====================



    begin = clock(); // ================== clock ======

    double* rho_classic = malloc(sizeof(double)*N*N);
    llenar_rho(rho_classic, N, L, dx, a);

    end = clock();
    time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
    printf("Serial definitoon of rho; %f\n", time_spent); 
    // ================== end clock =====================
```

comparacion de tiempos:

```bash
$ Clase6 0403/ .\avxpoisson.exe
    Avx definition and fill of rho; 0.000000
    Serial definitoon of rho; 0.002000
```

como se observa es varios ordenes de magnitud de diferencia, como 4 ordenes de magnitud


## Trabajo:

- aplicar tecnicas de manejo de memoria avanzadas; para la ecuación de Poisson con el metodo de Jacobi

