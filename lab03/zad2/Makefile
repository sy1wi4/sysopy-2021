CC = gcc
CFLAGS = -Wall -g
DIR := ${CURDIR}

all: run_static

static:
	$(CC) $(CFLAGS) -c mylibrary.c -o mylibrary.o
	ar rcs libmylibrary.a mylibrary.o

shared:
	$(CC) $(CFLAGS) -fPIC -c mylibrary.c -o mylibrary.o
	$(CC) $(CFLAGS) -shared -o libmylibrary.so mylibrary.o

run_static: static
	$(CC) -c main.c
	$(CC) main.o libmylibrary.a -o main
	./main $(ARGS)

clean:
	@rm -rf *.o *.a *.so temp.txt f main