#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h> // for AVX intrinsics

#define N 6 // number of grid points
#define EPSILON 1e-6 // convergence threshold

int main() {
    double dx = 1.0/(N-1);
    double dx2 = dx*dx;
    double *u = (double*) malloc(N*N*sizeof(double)); // solution
    double *f = (double*) malloc(N*N*sizeof(double)); // forcing function
    double *unew = (double*) malloc(N*N*sizeof(double)); // temporary storage for new solution

    double *buffer = (double*) malloc(4*sizeof(double));

    int it = 0;

    // Initialize u and f
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            u[i*N+j] = ((double)i)*N+(double)j;
            f[i*N+j] = 1.0;
        }
    }

    // Jacobi iteration
    double diff = EPSILON + 1.0;
    while (diff > EPSILON) {
        diff = 0.0;
        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j+=4) {
                // Compute new value of u using Jacobi iteration
                __m256d u_ij = _mm256_loadu_pd(&u[i*N+j]); // load 4 consecutive elements from u
                /* _mm256_storeu_pd(&buffer[0], u_ij);
                printf("u_ij values = %f %f %f %f\n",buffer[0],buffer[1],buffer[2],buffer[3]); */

                __m256d u_im1j = _mm256_loadu_pd(&u[(i-1)*N+j]); // load 4 consecutive elements from u
                /* _mm256_storeu_pd(&buffer[0], u_im1j);
                printf("u_im1j values = %f %f %f %f\n",buffer[0],buffer[1],buffer[2],buffer[3]); */

                __m256d u_ip1j = _mm256_loadu_pd(&u[(i+1)*N+j]); // load 4 consecutive elements from u
                /* _mm256_storeu_pd(&buffer[0], u_ip1j);
                printf("u_ip1j values = %f %f %f %f\n",buffer[0],buffer[1],buffer[2],buffer[3]); */

                __m256d u_ijm1 = _mm256_loadu_pd(&u[i*N+j-1]); // load 4 consecutive elements from u
                /* _mm256_storeu_pd(&buffer[0], u_ijm1);
                printf("u_ijm1 values = %f %f %f %f\n",buffer[0],buffer[1],buffer[2],buffer[3]); */

                __m256d u_ijp1 = _mm256_loadu_pd(&u[i*N+j+1]); // load 4 consecutive elements from u
                /* _mm256_storeu_pd(&buffer[0], u_ijp1);
                printf("u_ijp1 values = %f %f %f %f\n",buffer[0],buffer[1],buffer[2],buffer[3]); */

                __m256d f_ij = _mm256_loadu_pd(&f[i*N+j]); // load 4 consecutive elements from f

                __m256d unew_ij = _mm256_add_pd(u_im1j, u_ip1j); // sum of values in neighboring cells
                unew_ij = _mm256_add_pd(unew_ij, u_ijm1);
                unew_ij = _mm256_add_pd(unew_ij, u_ijp1);
                unew_ij = _mm256_add_pd(unew_ij, f_ij);

                unew_ij = _mm256_mul_pd(unew_ij, _mm256_set1_pd(dx2/4.0)); // multiply by dx^2/4

                _mm256_storeu_pd(&unew[i*N+j], unew_ij); // store 4 consecutive elements to unew

                __m256d diff_ij = _mm256_sub_pd(unew_ij, u_ij); // compute difference between new and old value

                _mm256_storeu_pd(&buffer[0], diff_ij);
                //printf("diff_ij values = %f %f %f %f\n",buffer[0],buffer[1],buffer[2],buffer[3]);

                diff_ij = _mm256_mul_pd(diff_ij, diff_ij); // square difference

                _mm256_storeu_pd(&buffer[0], diff_ij);
                //printf("diff_ij values = %f %f %f %f\n",buffer[0],buffer[1],buffer[2],buffer[3]);

                diff_ij = _mm256_hadd_pd(diff_ij, diff_ij); // horizontal add to get sum of squares

                _mm256_storeu_pd(&buffer[0], diff_ij);
                //printf("diff_ij values = %f %f %f %f\n",buffer[0],buffer[1],buffer[2],buffer[3]);

                diff_ij = _mm256_hadd_pd(diff_ij, diff_ij);

                _mm256_storeu_pd(&buffer[0], diff_ij);
                //printf("diff_ij values = %f %f %f %f\n",buffer[0],buffer[1],buffer[2],buffer[3]);

                diff += _mm256_cvtsd_f64(diff_ij); // convert to float and add to total diff */
            }
        }
        diff = 0.0;
        // Update solution
        double *temp = u;
        u = unew;
        unew = temp;
        it++;
    }

    printf("Iterations: %d\n",it);
}