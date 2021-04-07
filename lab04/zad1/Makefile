CC = gcc
CFLAGS = -Wall -g

run_fork: compile_fork
	./fork $(ARGS)

test_fork: compile_fork
	@./fork ignore
	@./fork handler
	@./fork mask
	@./fork pending

compile_fork:
	$(CC) $(FLAGS) fork.c -o fork

clean:
	rm -f fork exec