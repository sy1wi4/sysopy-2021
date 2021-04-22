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
    printf("\nConsumer here!\n\n");


    char* pipe_path = argv[1];
    char* file_path = argv[2];
    int N = atoi(argv[3]);


    FILE* pipe = fopen(pipe_path, "r");
    if(pipe < 0) {
        printf("Pipe error!\n");
    }

    FILE* file = fopen(file_path, "w");
    if(file < 0) {
        printf("File opening error!\n");
    }

    char buffer[N + 5];

    while(fread(buffer, sizeof(char), N, pipe) == N){
        printf("CONSUMER READ FROM PIPE: %s\n", buffer);
        fprintf(file, buffer, strlen(buffer));
    }

    fclose(pipe);
    fclose(file);

    return 0;
}
