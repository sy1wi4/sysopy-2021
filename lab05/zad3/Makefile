CC = gcc
CFLAGS = -Wall -g

all: test

compile:
	$(CC) $(FLAGS) main.c -o main
	$(CC) $(FLAGS) consumer.c -o consumer
	$(CC) $(FLAGS) producer.c -o producer

test: compile
	./main

clean:
	rm -f main consumer producer pipe