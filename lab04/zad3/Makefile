CC = gcc
CFLAGS = -Wall -g

all: run_catcher

compile_catcher:
	$(CC) $(FLAGS) catcher.c -o catcher

compile_sender:
	$(CC) $(FLAGS) sender.c -o sender

run_catcher: compile_catcher
	./catcher $(ARGS)

run_sender: compile_sender
	./sender $(ARGS)


clean:
	rm -f sender catcher