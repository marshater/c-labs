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
#include <netinet/in.h>
#include <arpa/inet.h>

void *MultMatrix(void*);
void *serv(void*);
void *calculation(void*);

int mult(int A, int B) {
    return A*B;
}

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

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

    int port,result;
    char *servIP;
    int N=3;
    int *addrlen;

//
    struct sockaddr_in serv_addr, cli_addr;
    port = atoi(argv[1]);


//



    int cli_sock;
    pthread_t tid[N];
        cli_addr.sin_family = AF_INET;
        cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        cli_addr.sin_port = htons(port);





    struct data buf;

    int len = sizeof(serv_addr);

        if ((cli_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
            DieWithError("Cant socket");
        }

        if(bind(cli_sock, (struct sockaddr *) &cli_addr, sizeof(cli_addr)) == -1){
            DieWithError("Cant bind");
            close(cli_sock);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(25000);
        if(inet_aton(argv[2], &serv_addr.sin_addr) == 0){
            DieWithError("WRITE SERV, B***T!");
        }
        recvfrom(cli_sock, &buf, sizeof(struct data),0,(struct sockaddr*) NULL, 0 );
        pthread_create(tid, NULL, calculation,(void*) &buf);
        printf("TEST");
        int code;
        pthread_join(*tid, (void*) &code);
        sendto(cli_sock, &result, sizeof(int), 0,(struct sockaddr*) &serv_addr, sizeof(&serv_addr) );
        close(cli_sock);
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
