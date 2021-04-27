CC = gcc
CFLAGS = -Wall -g

all: compile
compile:
	$(CC) $(FLAGS) server.c -o server
	$(CC) $(FLAGS) client.c -o client

server:
	$(CC) $(CFLAGS) server.c -o server -lrt
	./server

client:
	@# if client exists: make -B client
	$(CC) $(CFLAGS) client.c -o client -lrt
	./client

clean:
	rm -f server client