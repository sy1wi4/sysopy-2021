CC = gcc
CFLAGS = -Wall -g

client:
	$(CC) $(FLAGS) client.c -o client -lpthread -lm
	./client $(ARGS)

server:
	$(CC) $(FLAGS) server.c -o server -lpthread -lm
	./server $(ARGS)

clean:
	rm -f server client socket