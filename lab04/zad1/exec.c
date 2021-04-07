//
// Created by sylwia on 4/7/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


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


        printf("Going to call execl()...\n");
        execl("./child", "./child", argv[1], NULL);
        printf("!!! Return not expected, execl() error !!!\n");
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

        sigismember(&new_mask, SIGUSR1) ? printf("Signal pending\n") : printf("Signal  EXEC TEST pending\n");

        printf("Going to call execl()...\n");
        execl("./child", "./child", argv[1], NULL);
        printf("!!! Return not expected, execl() error !!!\n");
    }

    return 0;
}

