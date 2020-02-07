#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
void *MultMatrix(void*);

#define MAX_SEND_SIZE 80


struct DATA{
    int **MainMatrix;
    int *SubMatrix;
    int Number;
};

void arrInit(int *** arr, int dim);
void arrFree(int ** arr, int dim);


int mult(int A, int B, int N) {
    int res = 0;
    res += A * B;
    return res;
}

void arrFill(int **A, int N) {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            A[i][j] = 1 + rand() % 5;
        }
    }
}
void vecFill(int *B, int N) {
    for(int i = 0; i < N; i++) {
            B[i] = 1 + rand() % 5;
    }
}




void arrInit(int *** arr, int dim) {
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!arr) return;
    int ** arrTmp = calloc(dim, sizeof(int*));
    if (!arrTmp) return;
    for (int i = 0; i < dim; ++i) {
        arrTmp[i] = calloc(dim, sizeof(int));
        if (!arrTmp[i]) return; 
    }

  *arr = arrTmp;  
}

void vecInit(int **vec, int dim) {
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!vec) return;
    int *vecTmp = calloc(dim, sizeof(int*));
    if (!vec) return;
    *vec = vecTmp;
}

void arrFree(int ** arr, int dim) {
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!arr) return;
    for (int i = 0; i < dim; ++i) free(arr[i]);
    free(arr); 
}

void vecFree(int * arr, int dim) {
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!arr) return;
    free(arr); 
}

void arrPrint(int ** arr, int dim) {
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!arr) return;
        for (int i = 0; i < dim; ++i) {
            for (int j = 0; j < dim; ++j) {
            printf("%2d ", arr[i][j]);
        }
        printf("\n"); 
    }
  printf("\n");
}


void vecPrint(int *arr, int dim) {
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!arr) return;
    for (int i = 0; i < dim; i++){
        printf("%i", arr[i]);
        printf("\n");
    }
}

int main(int argc, char** argv) {
    int **A, *B, N;
    srand(time(NULL));
    scanf("%d", &N);
    if (N <= 0) return -1;
    int R;
    int res = 0;
    struct DATA info;
    pthread_t lines[N];


    arrInit(&A, N);
    arrInit(&info.MainMatrix, N);
    
    vecInit(&B, N);
    vecInit(&info.SubMatrix, N);


    arrFill(A,N);
    vecFill(B,N);


    printf("Arr A:\n");
    arrPrint(A, N);

    printf("Arr B:\n");
    vecPrint(B, N);

// пишем в структуру
for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
        info.MainMatrix[i][j] = A[i][j];
    }
        info.SubMatrix[i] = B[i];
        info.Number = N;
}
// create threads
    for(int i = 0; i < N; i++){
        if(pthread_create(&lines[i], NULL, MultMatrix, &info) != 0){
        perror("Cant create");
        return EXIT_FAILURE;
        }
    }
//join threads
    for(int i = 0; i < N; i++){
        if((pthread_join(lines[i],(void**) &R)) != 0){
            perror("Cant join");
            return EXIT_FAILURE;
        }else{
    printf("%i\n", (int) R);
        }
    }

    arrFree(A, N);
    vecFree(B, N);
    arrFree(info.MainMatrix,N);
    vecFree(info.SubMatrix, N);

    return 0;
}
    void *MultMatrix(void *arg){
    int Z = 0;
    int var;
    Data* info = (Data*) arg;
        for(int i = 0; i < info->Number; i++){
            sleep(1);
            for(int j = 0; j < info->Number; j++){
                int tmp = mult(info->MainMatrix[j][i], info->SubMatrix[j],info->Number);
                Z = Z + tmp;
            }
        printf("%i\n", Z);//передаем данные на вывод и обнуляем счетчик матрицы
        var = Z;
        Z = 0;
        }
    pthread_exit((void*)var);
    }
