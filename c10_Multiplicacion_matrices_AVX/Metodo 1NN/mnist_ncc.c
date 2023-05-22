#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>

#include <omp.h>

#define num_pixels 784
#define num_train 60000
#define num_test 10000


/*
Interactua con los binarios
t10k*
train-images*
train-labels*
*/




int read_binary_data(char* filename, bool images, int n_values, uint8_t data[]) {

   FILE *fptr;
   int dummy;

   if ((fptr = fopen(filename,"rb")) == NULL){
       printf("Error! opening file");

       // Program exits if the file pointer returns NULL.
       exit(1);
   }

   if (images) {
       for (int i = 0; i < 4; i++) {
           fread(&dummy, sizeof(dummy), 1, fptr);
       }
   } else {
       for (int i = 0; i < 2; i++) {
           fread(&dummy, sizeof(dummy), 1, fptr);
       }
   }

   for (int i = 0; i < n_values; i++){
       fread(&data[i], sizeof(uint8_t), 1, fptr);
   }

   fclose(fptr);

}


void all_vs_all(uint8_t test[], uint8_t train[], uint8_t delta[], bool parallel) {

    #pragma omp parallel for collapse(2) if(parallel)
    for (int i = 0; i < num_test; i++)
        for (int j = 0; j < num_train; j++) {
            uint8_t accum = 0;
            for (int k = 0; k < num_pixels; k++) {
                const uint8_t residue = test[i*num_pixels+k] 
                                        - train[j*num_pixels+k];
                accum += residue*residue;
            }
            delta[i*num_train+j] = accum;
        }
}

double accuracy(uint8_t label_test[], uint8_t label_train[], uint8_t delta[], bool parallel) {

    int counter = 0;

    #pragma omp parallel for reduction(+:counter) if(parallel)
    for (int i = 0; i < num_test; i++) {

        double bsf = DBL_MAX; // from <float.h>
        int jst = -1; // dummy value

        for (int j = 0; j < num_train; j++) {
            const double value = delta[i*num_train+j];
            if (value < bsf) {
                bsf = value;
                jst = j;
            }
        }

        bool match = true;
        match = match && (label_test[i] == label_train[jst]);
        counter += match;
    }

    return (double) counter/(double) num_test;
}

int main(int argc, char* argv[]) {

    // run parallelised when any command line argument is given
    const bool parallel = argc > 1;

    if (parallel) {
        printf("Running in parallel.\n");
    }
    else {
        printf("Running sequentially.\n");
    }

    static uint8_t test[num_test*num_pixels];
    static uint8_t train[num_train*num_pixels];

    static uint8_t test_label[num_test];
    static uint8_t train_label[num_train];

    static uint8_t delta[num_test*num_train];

    read_binary_data("train-labels-idx1-ubyte",false,num_train,train_label);
    read_binary_data("t10k-labels-idx1-ubyte",false,num_test,test_label);

    read_binary_data("train-images-idx3-ubyte",true,num_train,train);
    read_binary_data("t10k-images-idx3-ubyte",true,num_test,test);

    all_vs_all(test, train, delta, parallel);
    double acc = accuracy(test_label, train_label, delta, parallel);

    printf("Test accuracy: %f",acc);
  
    return 0;
}