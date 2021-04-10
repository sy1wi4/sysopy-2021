//
// Created by sylwia on 4/9/21.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>


int caught = 0;
char* mode;

void handler_SIGUSR1(int signum){
    caught++;
    printf("\nCatcher handler_SIGUSR1 here. New SIGUSR1 caught ([%d] so far)\n", caught);
}

void handler_SIGUSR2(int sig, siginfo_t *info, void *ucontext){
    printf("--------------------------- ALL SIGNALS SENT -------------------------------------\n\n");
    printf("Catcher received SIGUSR2, now is going to send back [%d] received SIGUSR1 signals.\n", caught);

    // get sender PID from siginfo_t
    pid_t sender_PID = info->si_pid;
    printf("Sender PID: %d\n", sender_PID);

    // SIGUSR2 handled - send back SIGUSR1 (caught times)
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

    exit(0);
}


int main(int argc, char* argv[]) {

    if (argc != 2){
        printf("Wrong number of arguments!\n");
        return -1;
    }

    printf("\n\nCatcher here! My PID: %d\n", getpid());

    mode = argv[1];
    printf("Mode: %s\n", mode);

    printf("Catcher waiting for SIGUSR1...\n");

    // sigaction() can block other signals until the current handler returns


    // now we should set up handlers, etc.
    struct sigaction act_SIGUSR1;
    sigemptyset(&act_SIGUSR1.sa_mask);
    act_SIGUSR1.sa_handler = handler_SIGUSR1;
    sigaction(SIGUSR1, &act_SIGUSR1, NULL);

    //SIGUSR2
    struct sigaction act_SIGUSR2;
    sigemptyset(&act_SIGUSR2.sa_mask);
    act_SIGUSR2.sa_flags = SA_SIGINFO;
    act_SIGUSR2.sa_sigaction  = handler_SIGUSR2;
    sigaction(SIGUSR2, &act_SIGUSR2, NULL);


    // block other signals, wait by calling sigsuspend()

    sigset_t mask;
    sigfillset(&mask);   // initializes set including all signals
    // remove SIGUSR1 & SIGUSR2 from mask
    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGUSR2);
//    if(strcmp(mode, "sigrt") == 0){
//        sigdelset(&mask, SIGRTMIN+1);
//        sigdelset(&mask, SIGRTMIN+2);
//    }
    sigprocmask(SIG_SETMASK, &mask, NULL);


//    sigsuspend(&mask);
    while(1){}

    return 0;
}