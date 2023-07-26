#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

// Funciones útiles
void printOutput(int rows, int cols, float data[]);

// para leer:
void readCSVInput(int rows, int cols, float data[], const char *filename);
// para guardar los datos y graficar
void saveData(float *data, int rows, int cols, const char *filename);


void updateJacobi(float *myData, int myRows, int myCols, float *buff, int myID, int numP, MPI_Comm cartComm) {
    // Buffers para recibir filas límite
    float *prevRow = (float*) malloc(myCols * sizeof(float));
    float *nextRow = (float*) malloc(myCols * sizeof(float));

    // Create column type and commit it
    MPI_Datatype columnType;
    MPI_Type_vector(myRows, 1, myCols, MPI_FLOAT, &columnType);
    MPI_Type_commit(&columnType);

    // Update first row and receive previous row
    if (myID > 0) {
        MPI_Send(myData, 1, columnType, myID - 1, 0, cartComm);
        MPI_Recv(prevRow, myCols, MPI_FLOAT, myID - 1, 0, cartComm, MPI_STATUS_IGNORE);
    }

    // Update last row and receive next row
    if (myID < numP - 1) {
        MPI_Send(&myData[(myRows - 1) * myCols], 1, columnType, myID + 1, 0, cartComm);
        MPI_Recv(nextRow, myCols, MPI_FLOAT, myID + 1, 0, cartComm, MPI_STATUS_IGNORE);
    }

    // Update the first row (same as before)
    if ((myID > 0) && (myRows > 1)) {
        // (same code as before)
    }

    // Update the main block (same as before)
    for (int i = 1; i < myRows - 1; i++) {
        // (same code as before)
    }

    // Update the last row (same as before)
    if ((myID < numP - 1) && (myRows > 1)) {
        // (same code as before)
    }

    // Copy the local buffer back to myData (same as before)
    memcpy(myData, buff, myRows * myCols * sizeof(float));

    // Free memory and clean up
    free(prevRow);
    free(nextRow);
    MPI_Type_free(&columnType);
}

int main(int argc, char *argv[]) {
    // Inicialización MPI
    MPI_Init(&argc, &argv);

    int numP, myID;
    MPI_Comm_size(MPI_COMM_WORLD, &numP);
    MPI_Comm_rank(MPI_COMM_WORLD, &myID);

    if (argc < 4) {
        if (myID == 0) {
            printf("Program should be called as ./jacobi rows cols errThreshold\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    float errThres = atof(argv[3]);

    if ((rows < 1) || (cols < 1)) {
        if (myID == 0) {
            printf("Number of rows and columns must be greater than 0.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    if (rows % numP) {
        if (myID == 0) {
            printf("Number of rows must be a multiple of the number of processes.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // Distribución de la matriz en un grid de tiles bidimensional
    int myRows = rows / numP;
    int myCols = cols;
    float *data = NULL;
    const char* csvFileName = "input_data.csv";

    if (myID == 0) {
        data = (float*) malloc(rows * cols * sizeof(float));
        readCSVInput(rows, cols, data, csvFileName);
    }

    // Crear tipo de dato derivado MPI para representar una columna de la matriz
    MPI_Datatype columnType;
    MPI_Type_vector(myRows, 1, cols, MPI_FLOAT, &columnType);
    MPI_Type_commit(&columnType);

    // Crear tipo de dato derivado MPI para representar una submatriz (tile)
    MPI_Datatype tileType;
    MPI_Type_vector(myRows, cols, cols, MPI_FLOAT, &tileType);
    MPI_Type_commit(&tileType);

    // Crear un nuevo comunicador cartesiano 2D para la distribución bidimensional de datos
    MPI_Comm cartComm;
    int dims[2] = {numP, 1}; // El grid tiene solo 1 columna
    int periods[2] = {0, 0}; // Sin períodos (no se trata como un toroide)
    int reorder = 1; // Permitir reordenamiento de procesos para optimización
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cartComm);

    int myCoords[2]; // Coordenadas (rank de fila y columna) del proceso actual
    MPI_Cart_coords(cartComm, myID, 2, myCoords);

    // Determinar la submatriz (tile) correspondiente al proceso actual
    float *myData = (float*) malloc(myRows * myCols * sizeof(float));
    MPI_Scatter(data, 1, tileType, myData, myRows * myCols, MPI_FLOAT, 0, cartComm);

    // Buffers para recibir filas límite
    float *prevRow = (float*) malloc(cols * sizeof(float));
    float *nextRow = (float*) malloc(cols * sizeof(float));

    MPI_Barrier(MPI_COMM_WORLD);

    // Medir el tiempo inicial
    double start = MPI_Wtime();

    // Bucle para iteraciones de Jacobi
    float *buff1 = (float*) malloc(myRows * myCols * sizeof(float)); // Buffer para almacenar resultados temporales 1
    float *buff2 = (float*) malloc(myRows * myCols * sizeof(float)); // Buffer para almacenar resultados temporales 2 (copia de myData)
    float *tempBuff; // Puntero temporal para intercambiar buffers

    // Bucle para iteraciones de Jacobi
    float error = errThres + 1.0;
    float myError;

    // Guardar la data inicial
    char initialDataFile[100];
    sprintf(initialDataFile, "initial_data_process_%d.txt", myID);
    saveData(myData, myRows, myCols, initialDataFile);

    // Bucle de cálculo de Jacobi y comunicación entre bloques vecinos
    while (error > errThres) {
        tempBuff = buff1;
        buff1 = buff2;
        buff2 = tempBuff;

        updateJacobi(myData, myRows, myCols, buff1, myID, numP, cartComm);

        // Calculate the local error
        myError = 0.0;
        for (int i=1; i < myRows; i++) {
            for (int j=1; j < cols-1; j++) {
                // Determine difference between data and buff1
                myError += (myData[i*cols+j]-buff1[i*cols+j])*(myData[i*cols+j]-buff1[i*cols+j]);
            }
        }

        // Sum error of all processes and store in 'error' on all processes
        MPI_Allreduce(&myError, &error, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Gather final matrix on process 0 for output
    MPI_Gather(myData, myRows*cols, MPI_FLOAT, data, myRows*cols, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Measure current time
    double end = MPI_Wtime();

    char finalDataFile[100];
    sprintf(finalDataFile, "final_data_process_%d.txt", myID);
    saveData(myData, myRows, myCols, finalDataFile);

    // Impresión de los resultados por el proceso 0
    if (myID == 0) {
        printf("Time with %d processes: %f seconds.\n", numP, end - start);
        //printOutput(rows, cols, data);
        free(data);
    }

    // Liberar memoria y finalizar MPI
    free(myData);
    free(buff1);
    free(buff2);
    free(prevRow);
    free(nextRow);

    MPI_Type_free(&columnType);
    MPI_Type_free(&tileType);
    MPI_Comm_free(&cartComm);
    MPI_Finalize();
}

// Las funciones utiles:
void printOutput(int rows, int cols, float data[]){
    for (int i = 0; i < rows*cols; i++){
        printf("%f ",data[i]);
        if ((i+1)%cols == 0) printf("\n");
    }
}

void readCSVInput(int rows, int cols, float data[], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (fscanf(file, "%f,", &data[i * cols + j]) != 1) {
                    printf("Error reading data from CSV file.\n");
                    fclose(file);
                    exit(1);
                }
            }
        }
        fclose(file);
    } else {
        printf("Error opening CSV file.\n");
        exit(1);
    }
}

void saveData(float *data, int rows, int cols, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                fprintf(file, "%f ", data[i * cols + j]);
            }
            fprintf(file, "\n");
        }
        fclose(file);
    }
}
