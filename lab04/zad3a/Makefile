CC = gcc
CFLAGS = -Wall -g

all: run

compile:
	@$(CC) $(FLAGS) catcher.c -o catcher
	@$(CC) $(FLAGS) sender.c -o sender


run: compile
	@# kill -9 `pidof catcher`
	@# With & the process starts in the background, so you do not have to wait until the script is finished
	@./catcher $(ARGS) &
	@./sender `pidof catcher` 10000 $(ARGS)


clean:
	rm -f sender catcher catcher.out