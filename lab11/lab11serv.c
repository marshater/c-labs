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

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
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
    N = atoi(argv[1]);
    if (N <= 0) return -1;
    int result = 0;
    struct data info;
//
    struct sockaddr_in serv_addr, cl_addr;
    int port, sock_serv, cl_len;
    port = atoi(argv[2]);
//    pthread_t tid[5];

//



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
    printf("TEST");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(25000);
    bzero(&serv_addr, sizeof(serv_addr));

    if((sock_serv = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        DieWithError("Cant socket");
    };

    if(bind(sock_serv, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
        DieWithError("Cant bind");
        close(sock_serv);
    }


    for(int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            info.SubMatrix[j] = B[j];
            info.Number = N;
            info.MainMatrix[j] = A[j][i];
        }
        sendto(sock_serv, &info, sizeof(struct data),0, (struct sockaddr*) &cl_addr, cl_len);
        sleep(2);
        recvfrom(sock_serv, &result, sizeof(int),0, (struct sockaddr*) &cl_addr, &cl_len );
/*        pthread_create(&tid[i], NULL, *serv, (void*) &sock_serv);
        pthread_join(tid[i], (void**) res);*/
        i++;
    }
    
    close(sock_serv);

    arrFree(A, N);
    vecFree(B, N);
    vecFree(info.MainMatrix,N);
    vecFree(info.SubMatrix, N);

    return 0;
}

/*
void *serv(void *arg){
    int sock = *(int*)arg;
    {
        recvfrom(sock, &result, sizeof(int),0, (struct sockaddr*) &cl_addr, &cl_len );
        printf("%d\n", result);
        pthread_exit((void*)result);
    }
}*/
