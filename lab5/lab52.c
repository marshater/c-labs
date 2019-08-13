/*
 * lab52.c
 * 
 * Copyright 2019 PEKA <peka@peka-VirtualBox>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char **argv)
{
	void *handler;

	handler = dlopen("/home/usre/Загрузки/libmath2.so",RTLD_LAZY);
	if (!handler)
	{
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

