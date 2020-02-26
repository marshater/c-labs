
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <ctype.h>

#define MAX_SEND_SIZE 80

union semun 
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

struct msgbuf 
{
	long type;
	int msg;
};

void arrInit(int *** arr, int dim);
void arrFree(int ** arr, int dim);


int mult(int A, int B) 
{
	return A * B;
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

void vecPrint(int *arr, int dim)
{
	fprintf(stderr, "dbg: %s\n", __func__);
	if (!arr) return;
	for (int i = 0; i < dim; i++)
	{
		printf("%i", arr[i]);
		printf("\n");
	}
	printf("\n");
}

void doit(int **A, int *B, int N, int *shmem, int key )
{
	pid_t pid[N];
	for (int i = 0; i < N; i++) 
	{
		pid[i] = fork();
		if (pid < 0) 
		{
			printf("fork() failed\n");
			exit(-1);
		} else if (pid[i] == 0) 
		{
			int semid;
			union semun arg;
			struct sembuf lock_res = {0, -1, 0};
			struct sembuf rel_res = {0, 1, 0};
			semid = semget(key, 1, 0666 | IPC_CREAT);
			arg.val = 1;
			semctl(semid, 0, SETVAL, arg);

			int R = 0;

			for(int j = 0; j < N; j++)
			{
				R += mult(A[j][i], B[j]);
			}

			if((semop(semid, &lock_res, 1)) == -1)
			{
					fprintf(stderr, "Lock failed\n");
					exit(1);
			}

				shmem[i] = R;

			if((semop(semid, &rel_res, 1)) == -1)
			{
			fprintf(stderr, "Unlock failed\n");
			exit(6);
			} else {
				printf("Semaphore resources decremented by one (locked) i=%d\n", i);
			}

		semctl(semid, 0, IPC_RMID);
		exit(0);
		}
	}
	int status;
	for(int i = 0; i < N; i++) {
		if (pid[i] == waitpid(pid[i], &status, 0))
		{
		}
	}
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

int main(int argc, char** argv) 
{
	int **A, *B, N;
	int shid, *shmem;
	key_t key;


	srand(time(NULL));

	scanf("%d", &N);
	if (N <= 0) return -1;

	if((shid = shmget(key, N*sizeof(long), 0666 | IPC_CREAT))< 0)
	{
		perror("shmget failure");
		exit(1);
	}

	if ((shmem = shmat(shid, NULL, 0)) == (int *) -1) 
	{
		perror("shmat failure");
		exit(2);
	}

	arrInit(&A, N);
	vecInit(&B, N);

	arrFill(A,N);
	vecFill(B,N);

	printf("Arr A:\n");
	arrPrint(A, N);

	printf("Vec B:\n");
	vecPrint(B, N);

	doit(A, B, N, shmem, key);
	vecPrint(shmem, N);


	arrFree(A, N);
	vecFree(B, N);


	if (shmctl(shid, IPC_RMID, 0) < 0) 
	{
		perror("shmem deleting failure\n");
		exit(1);
	}

	return 0;
}
