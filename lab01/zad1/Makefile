CC = gcc
CFLAGS = -Wall -g

all: static shared

static:
	$(CC) $(CFLAGS) -c mylibrary.c -o mylibrary.o
	ar rcs libmylibrary.a mylibrary.o

shared:
	$(CC) $(CFLAGS) -fPIC -c mylibrary.c -o mylibrary.o
	$(CC) $(CFLAGS) -shared -o libmylibrary.so mylibrary.o

clean:
	rm -rf *.o *.a *.so main