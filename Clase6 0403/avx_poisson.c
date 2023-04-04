#include<stdio.h>
#include<stdint.h>
#include<immintrin.h> // memory management intel
#include<time.h>
#include<math.h>
#include<string.h>

void llenar_rho(double* rho, int N, double L, double dx, double a);
void llenar_rho_avx2(double* rho, int N, double L, double dx, double a);
void avx2_vecadd(double *A, double *B, double *C, uint64_t N);
void guardar_datos(double* phi, int N, char* filename);


void llenar_con_zeros(double* phi, int N){
    /* asume un arreglo cuadrado, phi[N][N]
        ya que itera hasta el elemento N*N
    */

    // _ps packed single precission
    // _pd packed double precission
    __m256d zero = _mm256_setzero_pd(); // zero = [0 0 0 0 ]

    for(int i=0; i< N * N; i+=4){
        //hemos de cargar phi en una variable __m256d
        //__m256d AV = _mm256_load_pd(phi+i); // accede a phi[i]
        // __m256d X = _mm256_add_pd(X, zero);
        _mm256_store_pd(phi+i, zero);

        // _mm256_storeu_pd(&rho[i*N+j], VAL);
    }

}


int main(){
    int N = 100;
    double L = 10.0;
    double dx = L/((double)N);
    int Nmax = 1000;
    int it = 0;
    double err = 1.0;
    double tol = 1.0e-5;
    double a = 1.0;

    clock_t begin, end;
    double time_spent;

    double* phi_n = (double*) _mm_malloc(sizeof(double) * N * N, 64);
    double* phi_nm1 = (double*) _mm_malloc(sizeof(double) * N * N, 64);



    begin = clock(); // ================== clock ======

    double* rho = (double*) _mm_malloc(sizeof(double)*N*N, 64); 
    llenar_rho_avx2(rho, N, L, dx, a);

    end = clock();
    time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
    printf("Avx definition and fill of rho; %f\n", time_spent); 
    // ================== end clock =====================


    /*
     begin = clock(); 

    double* rho_classic = malloc(sizeof(double)*N*N);
    llenar_rho(rho_classic, N, L, dx, a);

    end = clock();
    time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
    printf("Serial definitoon of rho; %f\n", time_spent); 
    */

    char* filename = "t1.dat";
    guardar_datos(rho, N, filename);

    llenar_con_zeros(rho, N);

    char* filename2 = "t2.dat";
    guardar_datos(rho, N, filename2);
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

    __m256d A = [double, double, double, double]  = [float float float float float float float float]

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
        

        for (int j = 0; j < N; j += 4) {

            // inicia X con j+0.5, j+1.5, ...
            //double arr[4] = {j+3, j+2, j+1, j};
            double arr[4] = {j, j+1, j+2, j+3};
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





void avx2_vecadd(double *A, double *B, double *C, uint64_t N){
    /*instrucciones avx2 dados por intel; permite agregar elementos en paralelo
    AVX2 -> advanced vector extension 2

    permite 
    
    */
    __m256d X;
    for (uint64_t i=0; i<N; i += 4){
        __m256d AV = _mm256_load_pd(A+i);
        __m256d BV = _mm256_load_pd(B+i);
        X = _mm256_add_pd(AV,BV);
        _mm256_store_pd(C+i, X);

    }
}