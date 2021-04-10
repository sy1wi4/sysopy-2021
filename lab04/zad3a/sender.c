//
// Created by sylwia on 4/9/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int caught = 0;
int to_send;


void handler_SIGUSR1(int signum) {
    printf("Sender handler_SIGUSR1 here. New SIGUSR1 caught.\n");
    caught++;
}


void handler_SIGUSR2(int signum) {
    printf("\n--------------------------- ALL SIGNALS SENT BACK --------------------------------\n\n");
    printf("Sender handler_SIGUSR2 here. SIGUSR2 caught.\n");
    printf("Sender caught [%d] SIGUSR signals, should be [%d].\n", caught, to_send);
    printf("\n///////// EXIT /////////\n\n");
    exit(0);
}


int main(int argc, char* argv[]){

    if (argc != 4){
        printf("Wrong number of arguments!\n");
        return -1;
    }


    int catcher_PID = atoi(argv[1]);
    to_send = atoi(argv[2]);
    char* mode = argv[3];

    printf("\n\nSender here! My PID: %d\n", getpid());
    printf("Catcher PID: %d\n", catcher_PID);
    printf("Number of signals to send: %d\n", to_send);
    printf("Mode: %s\n", mode);


    // SIGUSR1
    struct sigaction act_SIGUSR1;
    sigemptyset(&act_SIGUSR1.sa_mask);
    act_SIGUSR1.sa_handler = handler_SIGUSR1;
    sigaction(SIGUSR1, &act_SIGUSR1, NULL);

    //SIGUSR2
    struct sigaction act_SIGUSR2;
    sigemptyset(&act_SIGUSR2.sa_mask);
    act_SIGUSR2.sa_handler = handler_SIGUSR2;
    sigaction(SIGUSR2, &act_SIGUSR2, NULL);


    printf("\nSender going to send %d SIGUSR1 signals ......\n", to_send);
    for (int i = 0; i < to_send; i++) {

        if (strcmp(mode, "kill") == 0) {
            kill(catcher_PID, SIGUSR1);

        }

        else if (strcmp(mode, "queue") == 0) {
            union sigval value;
            sigqueue(catcher_PID, SIGUSR1, value);
        }

        else if (strcmp(mode, "sigrt") == 0) {
            ;
        }

        else {
            printf("Wrong mode\n");
            return -1;
        }
    }


    printf("Sender sent all [%d] SIGUSR1 signals.\n\n", to_send);

    // send SIGUSR2

    printf("Now sender is going to send SIGUSR2\n");

    if (strcmp(mode, "kill") == 0) {
        printf("GOING TO KILL YOU CATCHER (pid: %d)\n\n", catcher_PID);
        kill(catcher_PID, SIGUSR2);
    }

    else if (strcmp(mode, "queue") == 0) {
        union sigval value;
        sigqueue(catcher_PID, SIGUSR2, value);
    }

    else if (strcmp(mode, "sigrt") == 0) {
        ;
    }

    else {
        printf("Wrong mode\n");
        return -1;
    }

    while(1){}

}