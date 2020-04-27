
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


int mult(int A, int B) 
{
    return A*B;
}

void arrFill(int **A, int N) 
{
    for(int i = 0; i < N; i++) 
    {
        for(int j = 0; j < N; j++) 
        {
            A[i][j] = 1 + rand() % 5;
        }
    }
}
void vecFill(int *B, int N) 
{
    for(int i = 0; i < N; i++) 
    {
            B[i] = 1 + rand() % 5;
    }
}


int doit(int **A, int *B, int *M, int N) 
{
    int status;
    int R = 0;
    pid_t pid[N];
    int fd[N];
    int pfd[N];
    int fifo[N];
    for (int i = 0; i < N; i++) 
    {
        pid[i] = fork();
        mkfifo(fifo[i], 0666);
        if (pid < 0) 
        {
            printf("fork() failed\n");
            exit(-1);
        } else if (pid[i] == 0) 
        {
            for(int i = 0; i < N; i++)
            {
                for(int j = 0; j < N; j++)
                {
                    int tmp = mult(A[j][i], B[j]);
                    R = R + tmp;
                }
                fd[i] = open(fifo[i], O_RDWR, 0);
                write(fd[i], &R, sizeof(int));
                close(fd[i]);
                printf("child: C[%d]=%d \t",i, R);
                R = 0;
            }
        }
        exit(0);
    }
    for(int i = 0; i < N; i++) 
    {
        for (int i = 0; i < N; i++) 
        {
            pfd[i]=open(fifo[i], O_RDONLY, 0);
            int res;
            if (pid[i]==waitpid(pid[i], &status, 0)) 
            {
                read(pfd[i], &res, sizeof(int));
                close(pfd[i]);
            }
        }
    }
    return *M;
}

void arrInit(int *** arr, int dim) 
{
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!arr) return;
    int ** arrTmp = calloc(dim, sizeof(int*));
    if (!arrTmp) return;
    for (int i = 0; i < dim; ++i) 
    {
        arrTmp[i] = calloc(dim, sizeof(int));
        if (!arrTmp[i]) return; 
    }
  *arr = arrTmp;  
}

void vecInit(int **vec, int dim) 
{
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!vec) return;
    int *vecTmp = calloc(dim, sizeof(int*));
    if (!vec) return;
    *vec = vecTmp;
}

void arrFree(int ** arr, int dim) 
{
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!arr) return;
    for (int i = 0; i < dim; ++i) free(arr[i]);
    free(arr); 
}

void vecFree(int * arr, int dim) 
{
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!arr) return;
    free(arr); 
}

void arrPrint(int ** arr, int dim) 
{
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!arr) return;
        for (int i = 0; i < dim; ++i) 
        {
            for (int j = 0; j < dim; ++j)
            {
                printf("%2d ", arr[i][j]);
            }
            printf("\n"); 
        }
    printf("\n");
}


void vecPrint(int *arr, int dim)
{
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!arr) return;
    for (int i = 0; i < dim; i++)
    {
        printf("%i", arr[i]);
        printf("\n");
    }
}

int main(int argc, char** argv) 
{
    int **A, *B, *M, N;
    srand(time(NULL));
    scanf("%d", &N);
    if (N <= 0) return -1;


    arrInit(&A, N);
    vecInit(&B, N);
    vecInit(&M, N);

    arrFill(A,N);
    vecFill(B,N);

    printf("Arr A:\n");
    arrPrint(A, N);

    printf("Arr B:\n");
    vecPrint(B, N);

    doit(A, B, M, N);
    printf("Arr M = A x B =\n");
    vecPrint(M, N);

    arrFree(A, N);
    vecFree(B, N);
    vecFree(M, N);

    return 0;
}
