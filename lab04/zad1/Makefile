CC = gcc
CFLAGS = -Wall -g

all:
	@echo "\n\n#################################################"
	@echo "FORK TEST FORK TEST FORK TEST FORK TEST FORK TEST"
	@echo "#################################################"
	@make --no-print-directory test_fork
	@echo "\n\n#################################################"
	@echo "EXEC TEST EXEC TEST EXEC TEST EXEC TEST EXEC TEST"
	@echo "#################################################"
	@make --no-print-directory test_exec

run_fork: compile_fork
	./fork $(ARGS)

test_fork: compile_fork
	@./fork ignore
	@./fork handler
	@./fork mask
	@./fork pending

compile_fork:
	@$(CC) $(FLAGS) fork.c -o fork

run_exec: compile_exec
	./exec $(ARGS)

test_exec: compile_exec
	@./exec ignore
	@./exec mask
	@./exec pending

compile_exec:
	@$(CC) $(FLAGS) exec.c -o exec
	@$(CC) $(FLAGS) child.c -o child

clean:
	rm -f fork exec child