output: hello-world.o international-hello-world.o fancy-hello-world.o
	cc -o hello-world hello-world.o
	cc -o international-hello-world international-hello-world.o
	cc -o fancy-hello-world fancy-hello-world.o

creation: hello-world.c international-hello-world.c fancy-hello-world.c
	cc -c hello-world.c
	cc -c international-hello-world.c
	cc -c fancy-hello-world.c

target: dependencies
	action

clean:
	rm *.o hello-world international-hello-world fancy-hello-world
