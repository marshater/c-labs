doit2: lab52.c libmath2.so
	gcc lab52.c -o doit2 -ldl
libma: math2.o
	gcc -shared -o libmath2.so math2.o
math2.o: math2.c
	gcc -c -fPIC math2.c
clean:
	rm -f *.o *.so doit2
