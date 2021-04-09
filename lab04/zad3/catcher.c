//
// Created by sylwia on 4/9/21.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>


int caught = 0;
char* mode;

void handler_SIGUSR1(int signum){
    printf("Received SIGUSR1\n");
    caught++;
}

void handler_SIGUSR2(int sig, siginfo_t *info, void *ucontext){
    printf("Total received SIGUSR1 signals: %d", caught);

    // get sender PID from siginfo_t
    pid_t sender_PID = info->si_pid;
    printf("Sender PID: %d\n", sender_PID);

    // SIGSR2 handled - send back SIGUSR1 (caught times)
    for (int i = 0; i < caught; i++){
        if (strcmp(mode,"kill") == 0){
            kill(sender_PID, SIGUSR1);
        }
        else if (strcmp(mode,"queue") == 0){
            union sigval val;
            val.sival_int = caught;
            sigqueue(sender_PID, SIGUSR1, val);
        }
        else if (strcmp(mode,"sigrt") == 0){

        }
        else{
            printf("Wrong mode\n");
        }
    }

    // SIGUSR2 send back (caught times) - send SIGUSR2
    if (strcmp(mode,"kill") == 0){
        kill(sender_PID, SIGUSR2);
    }

    else if (strcmp(mode,"queue") == 0){
        union sigval value;
        sigqueue(sender_PID, SIGUSR2, value);
    }

    else if (strcmp(mode,"sigrt") == 0){
        ;
    }

    else{
        printf("Wrong mode\n");
    }
}


int main(int argc, char* argv[]) {

    if (argc != 2){
        printf("Wrong number of arguments!\n");
        return -1;
    }

    printf("Catcher here! My PID: %d\n", getpid());

    mode = argv[1];
    printf("Mode: %s\n", mode);

    printf("\nCatcher waiting for SIGUSR1...\n");

    // block other signals, wait by calling sigsuspend()


    return 0;
}