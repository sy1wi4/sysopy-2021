//
// Created by sylwia on 4/7/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main(int argc, char* argv[]) {

    printf("Child here!\n");

    if (strcmp(argv[1], "ignore") == 0){
        printf("Going to raise a signal in child process...\n");
        raise(SIGUSR1);
    }

    else if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0){
        // if "pending" mode then don't raise a signal in child process
        if (strcmp(argv[1], "mask") == 0) {
            printf("Going to raise a signal in child process...\n");
            raise(SIGUSR1);
        }

        sigset_t new_mask;
        // check whether any signals are pending
        sigpending(&new_mask);
        sigismember(&new_mask, SIGUSR1) ? printf("Signal pending [in child process]\n") : printf("Signal NOT pending [in child process]\n");
    }

    return 0;
}
