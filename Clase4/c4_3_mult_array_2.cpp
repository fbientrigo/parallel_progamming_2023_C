#include <cstdint> // uint32_t
#include <iostream> // std::cout
#include <immintrin.h> // AVX intrinsics
#include <time.h>

/*
este codigo agrega el desafio de sumar de manera paralela 
los componentes en un escalar

es exactamente el mismo codigo que c4_2
pero agrega el

hsum_avx

lo que se refiere a suma horizontal

de acuerdo a la literatura, la forma mas rapida de hacerlo
es iterativamente realizar conversiones
*/

float hsum_avx(__m256 v) {
    /*
    packfloats 256 son [f, f, f, f, f, f, f, f], 8 elementos
    al extraer 128 bits tenemos [f, f, f, f]
    luego se puede realizar la suma
    */

    // al hacer el casting de packfloat256 bits a packfloat128
    // se queda con los primeros elementos
    __m128 vlow = _mm256_castps256_ps128(v);
    __m128 vhigh = _mm256_extractf128_ps(v, 1); // 1 quiere decir que la mitad de arriba
            // vlow = vlow + vhigh
            vlow = _mm_add_ps(vlow, vhigh); 

    // [vlow2, vlow3, vlow0, vlow1]
    __m128 shuf = _mm_shuffle_ps(vlow, vlow, _MM_SHUFFLE(2, 3, 0, 1));

    // [vlow0+vlow1, -, -, -]
    __m128 sums = _mm_add_ps(vlow, shuf);

    // [-, -, vlow0+vlow1, -]
    shuf = _mm_movehl_ps(shuf, sums);

    // [vlow0+vlow1 + vlow2+vlow3, -, -, -]
    sums = _mm_add_ss(sums, shuf);

    // convert to simple float and return
    return _mm_cvtss_f32(sums);
}

void avx2_tmm(float* A, float* B, float* C, uint64_t M, uint64_t L, uint64_t N) {
    for (uint64_t i = 0; i < M; i++)
        for (uint64_t j = 0; j < N; j++) {

            __m256 X = _mm256_setzero_ps();
            for (uint64_t k = 0; k < L; k += 8) {
                const __m256 AV = _mm256_load_ps(A+i*L+k);
                const __m256 BV = _mm256_load_ps(B+j*L+k);
                X = _mm256_fmadd_ps(AV,BV,X);
            }
            C[i*N+j] = hsum_avx(X);
        }
}



// aqui esta tambien implementada en doubles
double hsum_double_avx(__m256d v);
// esta es la misma funcion del script anterior
void plain_tmm(float* A, float* B, float* C, uint64_t M, uint64_t L, uint64_t N);


int main() {

    // correr 10 bits a la izquierda el uno, osea
    // 2^10 = 1024
    const uint64_t m = 1 << 10;
    const uint64_t n = 1 << 10;
    const uint64_t l = 1 << 10;

    printf("m: %d, n: %d, l: %d \n", m,n,l);
    printf("m * l = %d \n", m*l);
    printf("n * l = %d \n", n*l);

    clock_t t;


    // matrices de float
    float* A = (float*) malloc(sizeof(float)*m*l);
    float* B = (float*)  malloc(sizeof(float)*l*n);
    float* Bt = (float*)  malloc(sizeof(float)*n*l);
    float* C = (float*)  malloc(sizeof(float)*m*n);

    // inicializar componente por componente
    for (uint64_t i = 0; i < m*l; i ++){
      A[i] = 0.0;
      B[i] = 0.0;
      Bt[i] = 0.0;
      C[i] = 0.0;
    }


    printf("Comenzando la suma de manera clasica, C = A . B : \n");
    // comenzamos a sumar
    t = clock();

    plain_tmm(A, B, C, m, l, n);
    t = clock() - t;

    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("C = A . B; metodo ingenuo: %f [s]\n\n",time_taken);

		/*
		si primero definimos una matriz traspuesta;
		entonces la multiplicacion de matrices sera contigua
		*/

    

    printf("Comenzando la suma que aproveche el cache, C = A * BT : \n");
    printf("Hemos de calcular la traspuesta primero: \n");
    t = clock();
    for (uint64_t k = 0; k < l; k++) {
        for (uint64_t j = 0; j < n; j++) {   
            Bt[j*l+k] = B[k*n+j];
        }
    }
    t = clock() - t;

    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Calculando B^T , la transpuesta: %f [s]\n",time_taken);

    printf("Entonces, C = A * BT : \n");
    t = clock();
    plain_tmm(A, Bt, C, m, l, n);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("C = A * BT: %f [s] \n\n",time_taken);


    // version utilizando avx
    printf("Sumando con AVX: \n");
    t = clock();
    avx2_tmm(A, B, C, m, l, n);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("C = A * B, con avx: %f [s] \n\n",time_taken);

    return 0;
}






double hsum_double_avx(__m256d v) {
    /*
    packdoubles 256 son [d, d, d, d], 4 elementos
    al extraer 128 bits tenemos [d, d]
    luego se puede realizar la suma
    */

    // al hacer el casting de packdouble256 bits a packdouble128
    // se queda con los primeros elementos
    __m128d vlow = _mm256_castpd256_pd128(v);
    __m128d vhigh = _mm256_extractf128_pd(v, 1); // 1 quiere decir que la mitad de arriba
            // vlow = vlow + vhigh
            vlow = _mm_add_pd(vlow, vhigh); 

    // [vlow1, vlow1]
    __m128d high64 = _mm_unpackhi_pd(vlow, vlow);

                    // la parte _mm_add_sd(vlow, high64) es:
                        // [vlow0, vlow1] + [vlow1, vlow1]
                        // [vlow0 + vlow1, vlow1 + vlow1]  
    //entonces convert to simple double o cvtsd, toma la parte baja
    // y la convierte en escalar, osea toma vlow0 + vlow1
    // tenemos asi terminada la suma
    return  _mm_cvtsd_f64(_mm_add_sd(vlow, high64));  // reduce to scalar

}




void plain_tmm(float* A, float* B, float* C, uint64_t M, uint64_t L, uint64_t N) {
    for (uint64_t i = 0; i < M; i++)
        for (uint64_t j = 0; j < N; j++) {
            float accum = float(0);
            for (uint64_t k = 0; k < L; k++)
                accum += A[i*L+k]*B[j*L+k];
            C[i*N+j] = accum;
        }
}


