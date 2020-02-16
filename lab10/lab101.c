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

struct data {
    int *MainMatrix;
    int *SubMatrix;
    int Number;
};

void arrInit(int *** arr, int dim);
void arrFree(int ** arr, int dim);

int mult(int A, int B) {
    return A*B;
}

void arrFill(int **A, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = 1 + rand() % 5;
        }
    }
}

void vecFill(int *B, int N) {
    for (int i = 0; i < N; i++) {
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
    int R[N];
    int res = 0;
    struct data info;
    pthread_t lines[N];

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    arrInit(&A, N);
    vecInit(&info.MainMatrix, N);
    
    vecInit(&B, N);
    vecInit(&info.SubMatrix, N);

    arrFill(A,N);
    vecFill(B,N);

    printf("Arr A:\n");
    arrPrint(A, N);

    printf("Arr B:\n");
    vecPrint(B, N);

// пишем в структуру
for (int i = 0; i < N; i++){
	for (int j = 0; j < N; j++){
// грязный костыль для ввода построчно
		info.SubMatrix[j] = B[j];
		info.Number = N;
		info.MainMatrix[j] = A[j][i];
	}
// вы мутехи, да?
        pthread_mutex_lock(&mutex);
        if (pthread_create(&lines[i], NULL, MultMatrix, &info) != 0){
        	perror("Cant create");
        return EXIT_FAILURE;
        } else {
            pthread_mutex_unlock(&mutex);
        }
        if ((pthread_join(lines[i],(void**) &R)) != 0){
            perror("Cant join");
			pthread_mutex_destroy(&mutex);
            return EXIT_FAILURE;
        } else {
    	printf("%i\n", (int) *R);
        }
}

    pthread_mutex_destroy(&mutex);

    arrFree(A, N);
    vecFree(B, N);
    vecFree(info.MainMatrix,N);
    vecFree(info.SubMatrix, N);

    return 0;
}

void *MultMatrix(void *arg){
    int Z = 0;
    struct data* info = (struct data*) arg;
    for (int i = 0; i < info->Number; i++){
    	int tmp = mult(info->MainMatrix[i], info->SubMatrix[i],info->Number);
        Z = Z + tmp;
    }
    pthread_exit((void*) Z);
    }
