#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

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

void guardar_datos(double* phi, int N){

    FILE *fp = fopen("result.dat", "w");
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            fprintf(fp, "%g\n", phi[i*N+j]);
        }
    }
    fclose(fp);
}

int main(){

    int N = 100;
    double L = 10.0;
    double dx = L/((double)N);

    int Nmax = 10000;
    int it = 0;

    double err = 1.0;
    double tol = 1.0e-5;

    double a = 1.0;

    double* tmp;
    double* phi_n = malloc(sizeof(double)*N*N);
    double* phi_nm1 = malloc(sizeof(double)*N*N);
    double* rho = malloc(sizeof(double)*N*N);

    memset(phi_n, 0, sizeof(double)*N*N);
    llenar_rho(rho, N, L, dx, a);

    while (it < Nmax && err > tol){
        err = 0.0;

        for (int i = 1; i < N-1; i++){
            for (int j = 1; j < N-1; j++){

                phi_nm1[i*N+j] = 0.25*(phi_n[(i+1)*N+j] + phi_n[(i-1)*N+j] + phi_n[i*N+j+1] + phi_n[i*N+j-1] - rho[i*N+j]*dx*dx);

                double epsilon = fabs(phi_nm1[i*N+j] - phi_n[i*N+j]);
                err = fmax(err, epsilon);
                //printf("%f\n",phi_nm1[i*N+j]);

            }
        }
        
        it++;
        tmp = phi_nm1;
        phi_nm1 = phi_n;        
        phi_n = tmp;

    }

    guardar_datos(phi_nm1, N);

    printf("Iteraciones: %d\n",it);
}

