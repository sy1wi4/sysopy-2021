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
        pid_t pid;

        for (int i = 0; i < n; i++) {
            pid = fork();
            if (pid == 0) {
                printf("%d.  Child PID: %d, parent PID: %d\n", i + 1, getpid(), getppid());
                exit(0);  // exit before child call fork()
            }
        }

        // block parent process until any of children has finished
        // if child terminates before parent process reaches wait(NULL) then the child
        // process turns to a zombie process until its parent waits on it and its released from memory.
        for (int i = 0; i < n; i++) wait(NULL);
    }
}