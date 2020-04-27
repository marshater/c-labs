#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <data.h>

int mult(int A, int B)
{
    return A*B;
}

void * calculation(void * arg)
{
    int Z = 0;
    struct data * info = (struct data*) arg;

    for (int i = 0; i < info->Number; i++)
    {
        int tmp = mult(info->MainMatrix[i], info->SubMatrix[i]);
        Z = Z + tmp;
    }

    pthread_exit((void *) Z);
}

void dieWithError(char * errorMessage)
{
    perror(errorMessage);
    exit(1);
}

int main(int argc, char** argv)
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
    int server_port = atoi(argv[4]);

    char * client_ip = argv[1];
    int client_port = atoi(argv[2]);

    struct sockaddr_in server_addr = {0};
    struct sockaddr_in client_addr = {0};

    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(client_ip);
    client_addr.sin_port = htons(client_port); 

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        dieWithError("Cant socket");
    }

    int rc = bind(sockfd, (struct sockaddr *) &client_addr, sizeof(client_addr));

    if(rc == -1)
    {
        dieWithError("Cant bind");
        close(sockfd);
    }

    int result = 0;
    int server_addr_len = 0;
    struct data buf = {0};
    pthread_t tid = 0;
    for (int idx = 0; idx < N; ++idx)
    {
        rc = recvfrom(sockfd, &buf, sizeof(struct data), 0, (struct sockaddr *) &server_addr,  &server_addr_len);
        if (rc == -1)
        {
            close(sockfd);
            dieWithError("sendto() failed");
        }

        pthread_create(&tid, NULL, calculation, (void*) &buf);
        pthread_join(tid, (void*) &result);
        rc = sendto(sockfd, &result, sizeof(int), 0, (struct sockaddr*) &server_addr, sizeof(struct sockaddr_in));
        if (rc == -1)
        {
            close(sockfd);
            dieWithError("sendto() failed");
        }
        fprintf(stderr, "result=%d\n", result);
    }
    close(sockfd);

    return 0;
}
