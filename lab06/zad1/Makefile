CC = gcc
CFLAGS = -Wall -g

all: run

compile:
	$(CC) $(FLAGS) server.c -o server
	$(CC) $(FLAGS) client.c -o client

server:
	$(CC) $(CFLAGS) server.c -o server
	./server

client:
	$(CC) $(CFLAGS) client.c -o client
	./client

run: compile
	./server &
	./client

clean:
	rm -f server client