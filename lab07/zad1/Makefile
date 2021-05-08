CC = gcc
CFLAGS = -Wall -g

all: run

compile:
	@touch oven
	@touch table
	@$(CC) $(FLAGS) cook.c -o cook
	@$(CC) $(FLAGS) supplier.c -o supplier
	@$(CC) $(FLAGS) main.c -o main

run: compile
	./main $(ARGS)
	@make --no-print-directory clean


clean:
	@rm -f main cook supplier oven table