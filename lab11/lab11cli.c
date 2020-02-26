#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <data.h>

void dieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

void arrFill(int **A, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            A[i][j] = 1 + rand() % 5;
    }
}

void vecFill(int *B, int N)
{
    for (int i = 0; i < N; i++)
        B[i] = 1 + rand() % 5;
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

void vecInit(int ** vec, int dim)
{
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!vec) return;
    int *vecTmp = calloc(dim, sizeof(int*));
    if (!vec) return;
    *vec = vecTmp;
}

void arrFree(int ** arr, int dim) {
    fprintf(stderr, "dbg: %s\n", __func__);
    if (!arr) return;
    for (int i = 0; i < dim; ++i)
        free(arr[i]);
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

// usage: server <client ip addr> <client port> <server ip> <server port> <matrix size>

int main(int argc, char ** argv)
{
    if (6 != argc)
    {
        printf("usage: ./server <client ip addr> <client port> <server ip> <server port> <matrix size>\n");
        return -1;
    }

    int N = atoi(argv[5]);
    if (N < 1 || N > MAX_SIZE)
    {
        printf("invalid matrix size\n");
        return -1;
    }

    char * server_ip = argv[3];
    char * client_ip = argv[1];
    int server_port = atoi(argv[4]);
    int client_port = atoi(argv[2]);

    struct sockaddr_in server_addr = {0};
    struct sockaddr_in client_addr = {0};

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(client_ip);
    client_addr.sin_port = htons(client_port); 

    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
    {
        dieWithError("Cant socket");
    };

    int rc = bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (rc == -1)
    {
        dieWithError("Cant bind");
        close(sockfd);
    }

    srand(time(NULL));

    int **A;
    arrInit(&A, N);
    arrFill(A, N);

    int *B;
    vecInit(&B, N);
    vecFill(B, N);


    printf("Arr A:\n");
    arrPrint(A, N);

    printf("Arr B:\n");
    vecPrint(B, N);

    int result = 0;
    struct data info;
    socklen_t cl_len;

    for(int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            info.SubMatrix[j] = B[j];
            info.Number = N;
            info.MainMatrix[j] = A[j][i];
        }

        rc = sendto(sockfd, &info, sizeof(struct data), 0, (struct sockaddr*) &client_addr, sizeof(struct sockaddr_in));
        if (rc == -1)
        {
            arrFree(A, N);
            vecFree(B, N);
            close(sockfd);
            dieWithError("sendto() failed");
        }

        rc = recvfrom(sockfd, &result, sizeof(int),0, (struct sockaddr*) &client_addr, &cl_len);
        if (rc == -1)
        {
            arrFree(A, N);
            vecFree(B, N);
            close(sockfd);
            dieWithError("sendto() failed");
        }

        fprintf(stderr, "result=%d\n", result);
    }

    close(sockfd);
    arrFree(A, N);
    vecFree(B, N);

    return 0;
}
