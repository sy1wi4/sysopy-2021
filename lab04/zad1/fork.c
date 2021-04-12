//
// Created by sylwia on 4/5/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


void handler(int signum){
    printf("Received signal %d. PID: %d, PPID: %d\n", signum, getpid(), getppid());
}


int main(int argc, char* argv[]){
    if (argc != 2) {
        printf("Wrong number of arguments!");
        return -1;
    }

    else if (strcmp(argv[1], "ignore") == 0){
        printf("\n------------ TESTING \"ignore\" ------------\n");

        // sets the disposition of the signal (SIGUSR1) to SIG_IGN - the signal is ignored
        signal(SIGUSR1, SIG_IGN);

        printf("Going to raise a signal...\n");
        raise(SIGUSR1);   // alternatively:  kill(getpid(),SIGUSR1);

        pid_t pid = fork();
        if (pid == 0){
            printf("Going to raise a signal in child process...\n");
            raise(SIGUSR1);
        }
        else {
            wait(NULL);
        }
    }

    else if (strcmp(argv[1], "handler") == 0){
        printf("\n------------ TESTING \"handler\" ------------\n");

        // sets the disposition of the signal (SIGUSR1) to handler
        signal(SIGUSR1, handler);

        printf("Going to raise a signal...\n");
        raise(SIGUSR1);   // alternatively:  kill(getpid(),SIGUSR1);

        pid_t pid = fork();
        if (pid == 0){
            printf("Going to raise a signal in child process...\n");
            raise(SIGUSR1);
        }
        else {
            wait(NULL);
        }
    }
    else if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0){
        if (strcmp(argv[1], "mask") == 0)
            printf("\n------------ TESTING \"mask\" ------------\n");
        else
            printf("\n------------ TESTING \"pending\" ------------\n");

        sigset_t new_mask;   // signals to block
        sigset_t old_mask;   // current mask
        sigemptyset(&new_mask);
        sigaddset(&new_mask, SIGUSR1);

        if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0)
            perror("Signal blocking failed");

        printf("Going to raise a signal...\n");
        raise(SIGUSR1);

        sigismember(&new_mask, SIGUSR1) ? printf("Signal pending\n") : printf("Signal NOT pending\n");

        pid_t pid = fork();
        if (pid == 0){

            // if "pending" mode then don't raise a signal in child process
            if (strcmp(argv[1], "mask") == 0) {
                printf("Going to raise a signal in child process...\n");
                raise(SIGUSR1);
            }
            else
                printf("Child process here\n");

            // check whether any signals are pending
            sigpending(&new_mask);
            sigismember(&new_mask, SIGUSR1) ? printf("Signal pending [in child process]\n") : printf("Signal NOT pending [in child process]\n");
        }
        else {
            wait(NULL);
        }
    }

    return 0;
}