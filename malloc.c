#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cblas.h>

int randomGenerator();

int main(){
    srand((unsigned) time(NULL));
    int size = randomGenerator();
    double timeSpentCheck = 0.0;
    double timeSpentMMM = 0.0;
    double *A = (double *)malloc(size * size * sizeof(double));
    double *B = (double *)malloc(size * size * sizeof(double));
    double *C = (double *)malloc(size * size * sizeof(double));
    double *checkMatrix = (double *)malloc(size * size * sizeof(double));
    
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            int offset = i * size + j;
            int n1 = randomGenerator();
            int n2 = randomGenerator();
            A[offset] = (double)n1; 
            B[offset] = (double)n2; 
            C[offset] = 0;
        }
    }

    clock_t beginMMM = clock();
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            for(int k = 0; k < size; k++){
                C[i * size + j] += A[i * size + k] * B[k * size + j];
            }
        }
    }
    clock_t endMMM = clock();

    clock_t beginCheck = clock();
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, size, size, size, 1.0, A, size, B, size, 0.0, checkMatrix, size);
    clock_t endCheck = clock();

    int flag = 1;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(checkMatrix[i * size + j] != C[i * size + j]){
                printf("Wrong");
                flag = 0;
                break;
            }
        }
        if(flag == 0){
            break;
        }
    }

    timeSpentCheck += (double)(endCheck - beginCheck) / CLOCKS_PER_SEC;
    timeSpentMMM += (double)(endMMM - beginMMM) / CLOCKS_PER_SEC;
    if(flag == 1){
        printf("Size of matrices: %d \n", size);
        printf("CBLAS function: %f \n", timeSpentCheck);
        printf("Our function: %f \n", timeSpentMMM);
        printf("Ratio: %f \n", (timeSpentMMM / timeSpentCheck));
    }

    free(A);
    free(B);
    free(C);
    free(checkMatrix);

    return 0;
}

int randomGenerator(){
    int num = 2 + (rand() % 2000);
    return num;
}