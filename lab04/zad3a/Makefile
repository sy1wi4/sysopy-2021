CC = gcc
CFLAGS = -Wall -g

all: test

compile:
	@$(CC) $(FLAGS) catcher.c -o catcher
	@$(CC) $(FLAGS) sender.c -o sender


run: compile
	@# kill -9 `pidof catcher`
	@# With & the process starts in the background, so you do not have to wait until the script is finished
	@./catcher $(ARGS) &
	@./sender `pidof catcher` 10000 $(ARGS)

test: compile
	@echo "\nTESTS FOR 5000 SIGNALS (with signal loss)"
	@echo "\n#########################################################################"
	@echo "#############################   mode: kill   ############################"
	@echo "#########################################################################"
	@./catcher kill &
	@./sender `pidof catcher` 5000 kill

	@echo "\n#########################################################################"
	@echo "#############################   mode: queue   ###########################"
	@echo "#########################################################################"
	@./catcher queue &
	@./sender `pidof catcher` 5000 queue

	@echo "\n#########################################################################"
	@echo "#############################   mode: sigrt   ###########################"
	@echo "#########################################################################"
	@./catcher sigrt &
	@./sender `pidof catcher` 5000 sigrt

clean:
	rm -f sender catcher catcher.out