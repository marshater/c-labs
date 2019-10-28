#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>


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
void VecFill(int *B, int N) {
    for(int i = 0; i < N; i++) {
            B[i] = 1 + rand() % 5;
    }
}



int doit(int **A, int *B, int *M, int N) {
    int status;
    int R = 0;
    pid_t pid[N];
    int fd[N];
    int pfd[N];
    int FIFO[N];
    for (int i = 0; i < N; i++) {
        pid[i] = fork();
        mkfifo(FIFO[i], 0111);
        if (pid < 0) {
            printf("fork() failed\n");
            exit(-1);
        } else if (pid[i] == 0) {
            for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                    int tmp = mult(A[j][i], B[j], N);
                    R = R + tmp;
                }
                fd[i] = open(FIFO[i], O_RDWR, 0);
                write(fd[i], &R, sizeof(int));
                close(fd[i]);
                printf("child: C[%d]=%d \t",i, R);
                R = 0;
            }
        }
        exit(0);
    }
        for(int i = 0; i < N; i++) {
            for (int i = 0; i < N; i++) {
                pfd[i]=open(FIFO[i], O_RDONLY, 0);
                int res;
                if (pid[i]==waitpid(pid[i], &status, 0)) {
                    read(pfd[i], &res, sizeof(int));
                    close(pfd[i]);
                }
        }
    }
    return 0;
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

void VecInit(int * Vec, int dim) {
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!Vec) return;
    Vec = calloc(dim, sizeof(int*));
    if (!Vec) return;
}

void arrFree(int ** arr, int dim) {
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!arr) return;
    for (int i = 0; i < dim; ++i) free(arr[i]);
    free(arr); 
}

void VecFree(int * arr, int dim) {
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


void VecPrint(int *arr, int dim) {
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!arr) return;
    for (int i = 0; i < dim; i++){
        printf("%i", arr[i]);
        printf("\n");
    }
}

int main(int argc, char** argv) {
    int **A, *B, *M, N;
    srand(time(NULL));
    scanf("%d", &N);
    if (N <= 0) return -1;
    B = calloc(N, sizeof(int*));
    M = calloc(N, sizeof(int*));

    arrInit(&A, N);

    arrFill(A,N);
    VecFill(B,N);

    printf("Arr A:\n");
    arrPrint(A, N);

    printf("Arr B:\n");
    VecPrint(B, N);

    doit(A, B, M, N);
    printf("Arr M = A x B =\n");
    VecPrint(M, N);

    arrFree(A, N);
    free(B);
    free(M);
  return 0;
}
