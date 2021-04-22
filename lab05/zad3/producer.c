//
// Created by sylwia on 4/15/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char* argv[]){
    if (argc != 5){
        printf("Wrong number of arguments!");
        exit(1);
    }
    printf("Producer here\n");

    char* pipe_path = argv[1];
    int row = atoi(argv[2]);
    char* file_path = argv[3];
    int N = atoi(argv[4]);

    FILE* pipe = fopen(pipe_path, "w");
    FILE* file = fopen(file_path, "r");

    char buffer[N];
    char message[N + 5];


    while(fread(buffer, sizeof(char), N, file) == N){
        snprintf(message, sizeof(message), "%d %s", row, buffer);
        fwrite(message, sizeof(char), N + 5, pipe);
        sleep(1);
    }

    fclose(pipe);
    fclose(file);

    return 0;
}