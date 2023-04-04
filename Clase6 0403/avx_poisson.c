#include<stdio.h>
#include<stdint.h>
#include<immintrin.h> // memory management intel
#include<time.h>
#include<math.h>
#include<string.h>

void llenar_rho(double* rho, int N, double L, double dx, double a);
void llenar_rho_avx2(double* rho, int N, double L, double dx, double a);
void avx2_vecadd(float *A, float *B, float *C, uint64_t N);
void guardar_datos(double* phi, int N, char* filename);



int main(){
    int N = 1000;
    double L = 10.0;
    double dx = L/((double)N);

    int Nmax = 1000;
    int it = 0;

    double err = 1.0;
    double tol = 1.0e-5;

    double a = 1.0;


    double* phi = (double*) _mm_malloc(sizeof(double) * N * N, 64);
    // populate phi with data...
    double* rho = (double*) _mm_malloc(sizeof(double)*N*N, 64); 

    llenar_rho_avx2(rho, N, L, dx, a);

    char* filename = "result2.dat";
    guardar_datos(rho, N, filename);
    return 0;
}




void llenar_rho(double* rho, int N, double L, double dx, double a){

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            double x = j*dx - L/2.0;
            double y = i*dx - L/2.0;
            double r = sqrt(x*x + y*y);
            rho[i*N+j] = 1.0/(pow(r*r + a*a, 3.0/2.0));
        }
    }
}


void llenar_rho_avx2(double* rho, int N, double L, double dx, double a) {
    /*
    llena rho usando una mayor cantidad de memoria;
    [256] -> [32 bytes] -> [double, double, double, double]

    __m256d A = [double, double, double, double] 

    */

     

   // miden 8 bytes
    __m256d L_2 = _mm256_set1_pd(L/2.0); // fill a vector with the same value
    __m256d A = _mm256_set1_pd(a);
    __m256d DX = _mm256_set1_pd(dx);
    __m256d DX_8 = _mm256_set1_pd(dx*4);
    __m256d ONE = _mm256_set1_pd(1.0);
    
    for (int i = 0; i < N; i++) {
        double y = i*dx - L/2.0;
        __m256d Y = _mm256_set1_pd(y);
        
        //for (int j = 0; j < N; j += 8) {
        for (int j = 0; j < N; j += 4) {
            // inicia X con j+0.5, j+1.5, ...
            //double arr[8] = {j+7, j+6, j+5, j+4, j+3, j+2, j+1, j};
            double arr[4] = {j+3, j+2, j+1, j};
            __m256d X = _mm256_add_pd(_mm256_loadu_pd(arr), _mm256_set1_pd(0.5));
            
            X = _mm256_mul_pd(X, DX); // multiplica X por dx y lo guarda en la memoria (recordemos, vector de 8)
            X = _mm256_sub_pd(X, L_2); // lo mismo por L/2
            
            __m256d R2 = _mm256_add_pd(_mm256_mul_pd(X, X), _mm256_mul_pd(Y, Y));
            __m256d R = _mm256_sqrt_pd(R2);
            __m256d DEN = _mm256_add_pd(_mm256_mul_pd(R2, R), _mm256_mul_pd(A, A));
            DEN = _mm256_mul_pd(DEN, _mm256_mul_pd(DEN, DEN));
            __m256d VAL = _mm256_div_pd(ONE, DEN);
            
            _mm256_storeu_pd(&rho[i*N+j], VAL);
        }
    }
}



void guardar_datos(double* phi, int N, char* filename){
    FILE *fp = fopen(filename, "w");
    if(fp == NULL){
        printf("Error opening file\n");
        return;
    }
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            fprintf(fp, "%g\n", phi[i*N+j]);
        }
    }
    fclose(fp);
}





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