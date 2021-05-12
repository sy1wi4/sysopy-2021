CC = gcc
CFLAGS = -Wall -g

all: run

compile:
	@$(CC) $(FLAGS) main.c -o main -lpthread

run: compile
	./main $(ARGS)

test: compile
	./main 1 numbers images/brain.ascii.pgm negatives/test
	./main 1 block images/brain.ascii.pgm negatives/test
	./main 2 numbers images/brain.ascii.pgm negatives/test
	./main 2 block images/brain.ascii.pgm negatives/test
	./main 4 numbers images/brain.ascii.pgm negatives/test
	./main 4 block images/brain.ascii.pgm negatives/test
	./main 8 numbers images/brain.ascii.pgm negatives/test
	./main 8 block images/brain.ascii.pgm negatives/test
	./main 16 numbers images/brain.ascii.pgm negatives/test
	./main 16 block images/brain.ascii.pgm negatives/test
	./main 99 numbers images/brain.ascii.pgm negatives/test
	./main 99 block images/brain.ascii.pgm negatives/test

clean:
	@rm -f main times.txt