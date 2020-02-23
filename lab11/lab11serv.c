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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void *MultMatrix(void*);
void *serv(void*);

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

//
    struct sockaddr_in serv_addr, cl_addr;
    int port, sock_serv, sock_listen, cl_len;
    pthread_t tid[5];

//
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

//
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    sock_listen = accept(sock_serv, (struct sockaddr *) &cl_addr, &cl_len);
    sock_serv = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sock_serv, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(sock_serv, N);
    int i = 0;


//
    while(i<N){
        for (int j = 0; j < N; j++){
            info.SubMatrix[j] = B[j];
            info.Number = N;
            info.MainMatrix[j] = A[j][i];
        }
//
        send(sock_listen, &info, sizeof(struct data),0);
        pthread_create(&tid[i], NULL, *serv, (void*) &sock_listen);
        i++;
}
pthread_join(*tid, (void**) res);
close(sock_listen);

    arrFree(A, N);
    vecFree(B, N);
    vecFree(info.MainMatrix,N);
    vecFree(info.SubMatrix, N);

    return 0;
}


void *serv(void *arg){
    int sock = *(int*)arg;
    int i, result = 0;
    {
        recv(sock, &result, sizeof(int),0);
        printf("%d\n", result);
        pthread_exit((void*)result);
    }
}
