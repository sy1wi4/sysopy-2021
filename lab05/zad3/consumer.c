//
// Created by sylwia on 4/15/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    if (argc != 4){
        printf("Wrong number of arguments!");
        exit(1);
    }
    printf("Consumer here\n");


    char* pipe_path = argv[1];
    char* file_path = argv[2];
    int N = atoi(argv[3]);

    FILE* pipe = fopen(pipe_path, "r");
    FILE* file = fopen(file_path, "w");

    char buffer[N];

    while(fgets(buffer, N, file)){
        fprintf(pipe, buffer, strlen(buffer));
    }

    fclose(pipe);
    fclose(file);

    return 0;
}
