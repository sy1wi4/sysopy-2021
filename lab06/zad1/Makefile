CC = gcc
CFLAGS = -Wall -g

all: compile
compile:
	$(CC) $(FLAGS) server.c -o server
	$(CC) $(FLAGS) client.c -o client

server:
	$(CC) $(CFLAGS) server.c -o server
	./server

client:
	@# if client exists: make -B client
	$(CC) $(CFLAGS) client.c -o client
	./client

clean:
	rm -f server client