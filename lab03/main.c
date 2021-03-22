//
// Created by sylwia on 3/22/21.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc, char* argv[]){
    if (argc != 2){
        printf("Wrong number of arguments");
        return -1;
    }

    else {
        printf("current: %d  parent: %d\n\n", getpid(), getppid());

        int n = atoi(argv[1]);
        int pid;

        for (int i = 0; i < n; i++) {
            printf("--- %d ---\n", i);

            pid = fork();
            if (pid == 0) {
                printf("child PID %d , parent PID %d\n", getpid(), getppid());
                exit(0);  // exit before child call fork
            }
        }

        for (int i = 0; i < 5; i++) wait(NULL);
    }
}