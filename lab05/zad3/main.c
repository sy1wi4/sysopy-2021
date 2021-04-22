//
// Created by sylwia on 4/15/21.
//

#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <sys/stat.h>


int main(){

    mkfifo("pipe", 0666);

    char result[] = "res.txt";
    char cons_N[] = "10";
    char prod_N[] = "5";
    char* input_files[] = {"1.txt", "2.txt", "A.txt", "B.txt"};

    pid_t pid = fork();
    if (pid == 0){
        execl("./consumer", "./consumer", "pipe", result, cons_N, NULL);
        printf("!!! Return not expected, execl() error !!!\n");

    }

    printf("Going to exec producers...\n");

    for (int i = 0; i < 4; i++){
        pid = fork();
        char row[2];
        sprintf(row,"%d", i);
        if (pid == 0){
            printf("%d\n", execl("./producer", "./producer", "pipe", row, input_files[i], prod_N,NULL));
        }
    }

    for (int i = 0; i < 5; i++){
        wait(NULL);
    }


    return 0;
}

