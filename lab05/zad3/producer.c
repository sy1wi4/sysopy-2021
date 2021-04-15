//
// Created by sylwia on 4/15/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


int main(int argc, char* argv[]){

    if (argc != 5){
        printf("Wrong number of arguments!");
        exit(1);
    }

    char* pipe_path = argv[1];
    int row = atoi(argv[2]);
    char* file_path = argv[3];
    int N = atoi(argv[4]);

    int pipe = open(pipe_path, O_WRONLY);
    int file = open(file_path, O_RDONLY);

    return 0;
}