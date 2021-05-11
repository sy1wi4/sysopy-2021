CC = gcc
CFLAGS = -Wall -g

all: run

compile:
	@$(CC) $(FLAGS) cook.c -o cook -lpthread -lrt
	@$(CC) $(FLAGS) supplier.c -o supplier -lpthread -lrt
	@$(CC) $(FLAGS) main.c -o main -lpthread -lrt

run: compile
	./main $(ARGS)
	@make --no-print-directory clean


clean:
	@rm -f main cook supplier oven table