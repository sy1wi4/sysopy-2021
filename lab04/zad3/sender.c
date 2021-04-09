//
// Created by sylwia on 4/9/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main(int argc, char* argv[]){

    if (argc != 4){
        printf("Wrong number of arguments!\n");
        return -1;
    }


    int catcher_PID = atoi(argv[1]);
    int to_send = atoi(argv[2]);
    char* mode = argv[3];
    printf("Catcher PID: %d\n", catcher_PID);
    printf("Number of signals to send: %d\n", to_send);
    printf("Mode: %s\n", mode);

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


    // send SIGUSR2

    if (strcmp(mode, "kill") == 0) {
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