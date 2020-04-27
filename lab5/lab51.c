#include <stdlib.h>
#include <stdio.h>
#include "math.h"
int main(int argc, char **argv)
{
	if(argc<4)
	{
		printf("too few arg\n"); exit(1);
	}
	
	printf("%i\n",multiply(atoi(argv[2]),atoi(argv[4])));
	printf("%f",divide(atoi(argv[2]),atoi(argv[4])));
	
	return 0;
}
