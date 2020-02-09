
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char **argv)
{
	void *handler;
	handler = dlopen("/home/usre/Загрузки/libmath2.so",RTLD_LAZY);
	if (!handler){
		fprintf(stderr,"dlopen() error: %s\n", dlerror());
		exit(1);
	}
	int (*mult)(int,int) = dlsym(handler,"multiply");
	int (*div)(int,int) = dlsym(handler,"divide");
	printf("%d\n",mult(atoi(argv[2]), atoi(argv[4])));
	printf("%d",div(atoi(argv[2]), atoi(argv[4])));
	dlclose(handler);
	return 0;
}

