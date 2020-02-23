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
void *calculation(void*);

struct data {
    int *MainMatrix;
    int *SubMatrix;
    int Number;
};

struct result {
    int sock;
    int res;
};

int main(int argc, char** argv) {
    struct result resu;
    int port,result;
    char *servIP;


//
    struct sockaddr_in serv_addr;
    servIP = argv[1];
    port = atoi(argv[2]);

//
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);


    int cli_sock[N];
    pthread_t tid[N];
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(servIP);
    serv_addr.sin_port = htons(port);



    for(int i = 0; i < N; i++){
        struct data buf;
        cli_sock[i] = socket(AF_INET, SOCK_DGRAM, 0);
        connect(cli_sock[i], (struct sockaddr*) &serv_addr, sizeof(serv_addr));
        resu.sock = cli_sock[i];
        recv(cli_sock[i], &buf, sizeof(struct data),0);
        pthread_create(&tid[i], NULL, calculation,(void*) &buf);
        int code;
        pthread_join(tid[i], (void*) &code);
        send(cli_sock[i], &result, sizeof(int), 0);
        close(cli_sock[i]);
    }
    return 0;
}


void *calculation(void *arg){
    int Z = 0;
    struct data* info = (struct data*) arg;
    for (int i = 0; i < info->Number; i++){
        int tmp = mult(info->MainMatrix[i], info->SubMatrix[i]);
        Z = Z + tmp;
    }
    pthread_exit((void*) Z);
    }
