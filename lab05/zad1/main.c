//
// Created by sylwia on 4/13/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARGS 20  // in line
#define MAX_LINES 10


char** parse(char* line){
    // array of commands in given line
    char** commands = (char**)calloc(MAX_ARGS, sizeof(char*));

    // format of line is: commandsX = command1 | command2 | ... | commandN, so let's split it
    // into sequences separated by "=" and into commands separated by "|"
    char* seq = strtok(line, "=");

    int ctr = 0;
    while ((seq = strtok(NULL, "|")) != NULL){
        commands[ctr++] = seq;
    }
    return commands;
}

void get_lines(FILE* file){
    char* line = (char*)calloc(256, sizeof(char));
    char** commands = (char**)calloc(MAX_ARGS, sizeof(char*));
    int i;
    while(fgets(line, 256*sizeof(char), file)) {
        printf("\n%s", line);
        commands = parse(line);

        // print parsed commands
        i = 0;
        while(commands[i] != NULL) {
            printf("com%d:  %s\n", i + 1, commands[i]);
            i++;
        }

        // run every command in separate process

        int** pipes = (int**)calloc(MAX_LINES, sizeof(int*));
        for (int j = 0; j < MAX_LINES; j++){
            pipes[j] = (int*)calloc(2, sizeof(int));
            // pipes[j][0] - fd for the read end
            // pipes[j][1] - fd for the write end
            if (pipe(pipes[j]) != 0){
                printf("Error while creating a pipe\n");
                exit(1);
            }
        }
    }


}

int main(int argc, char* argv[]){
    if (argc != 2){
        printf("Wrong number of arguments!\n");
        exit(1);
    }

    char* path = argv[1];
    FILE* commands = fopen(path, "r");

    if (commands == NULL){
        printf("Cannot open file\n");
        exit(1);
    }

    get_lines(commands);

    fclose(commands);

}