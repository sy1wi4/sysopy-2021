//
// Created by sylwia on 4/13/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MAX_ARGS 20  // in line
#define MAX_LINES 10
#define READ 0
#define WRITE 1


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


char** get_program_args(char* command, char* path){
    char** args = (char**)calloc(256, sizeof(char*));

    char* arg = strtok(command, " ");
    strcpy(path, arg);
    int ctr = 0;
    args[ctr++] = arg;  // program name
    while ((arg = strtok(NULL, " ")) != NULL){
        args[ctr++] = arg;
    }
    args[ctr] = NULL;
    return args;
}


int get_line_num(char* line, int i){
    // format: commandsX,  0 <= X <= 9
    if (i == 0) return line[8] - '0';
    else return line[9] - '0';
}


int* get_lines_to_execute(char* line){
    char** lines = (char**)calloc(MAX_ARGS, sizeof(char*));
    char* arg = strtok(line, "|");

    int ctr = 0;
    lines[ctr++] = arg;
    while ((arg = strtok(NULL, "|")) != NULL){
        lines[ctr++] = arg;
    }

    static int lines_num[MAX_ARGS];
    int i = 0;
    while(lines[i] != NULL) {
        lines_num[i] = get_line_num(lines[i], i);
        i++;
    }
    lines_num[i] = -1;

    return lines_num;
}


void get_lines(FILE* file){
    char* line = (char*)calloc(256, sizeof(char));
    char* current_line = (char*)calloc(256, sizeof(char));
    char** commands;
    char** args;
    int i;
    char** lines = (char**)calloc(MAX_LINES, sizeof(char*));   // commands: lines with "="
    int line_ctr = 0;
    int* lines_num;

    while(fgets(line, 256 * sizeof(char), file)) {
        printf("\n--------------------------------------------");
        printf("\nLINE: %s", line);

        if (strstr(line, "=")) {
            // save line

            char *line_copy = (char *) calloc(256, sizeof(char));
            strcpy(line_copy, line);
            lines[line_ctr++] = line_copy;
        }
        else {
            // get number of lines to execute now
            lines_num = get_lines_to_execute(line);
            printf("lines to execute: \n");
            i = 0;
            while (lines_num[i] != -1) {
                printf("lin %d\n", lines_num[i]);
                i++;
            }

            // execute all commands
            for (i = 0; lines_num[i] != -1; i++) {
                current_line = lines[lines_num[i]];
                printf("\nexecuting line %d:  %s \n", i, current_line);

                commands = parse(lines[lines_num[i]]);

                // print parsed commands
                int m = 0;
                while (commands[m] != NULL) {
                    printf("com%d:  %s\n", m + 1, commands[m]);
                    m++;
                }

                ////
                // idx 0 - fd for the read end
                // idx 1 - fd for the write end

                int pipe_in[2];
                int pipe_out[2];

                if (pipe(pipe_out) != 0) {
                    printf("Error while creating a pipe!\n");
                    exit(1);
                }
                printf("num of commands %d\n\n", m);

                // now m is a number of commands in current line

                ////////////
                for (int j = 0; j < m; j++) {

                    pid_t pid = fork();
                    //////
                    if (pid == 0) {
//                        printf("HELLO FROM CHILD PROCESS\n");
                        // first
                        if (j == 0) {
                            close(pipe_out[READ]);
                            // redirect
                            dup2(pipe_out[WRITE], STDOUT_FILENO);
                        }

                            // last
                        else if (j == m - 1) {
                            close(pipe_out[READ]);
                            close(pipe_out[WRITE]);
                            close(pipe_in[WRITE]);
                            dup2(pipe_in[READ], STDIN_FILENO);
                        }

                            // internal
                        else {
                            close(pipe_in[WRITE]);
                            close(pipe_out[READ]);
                            dup2(pipe_in[READ], STDIN_FILENO);
                            dup2(pipe_out[WRITE], STDOUT_FILENO);
                        }

                        char path[256];
                        args = get_program_args(commands[j], path);


                        printf("----- EXEC ----- \npath: %s\n", path);
                        // print args
                        m = 0;
                        while (args[m] != NULL) {
                            printf("arg%d:  %s\n", m + 1, args[m]);
                            m++;
                        }


                        // execute program
                        if (execvp(path, args) == -1) {
                            printf("ERROR in exec\n");
                            exit(1);
                        }
                    } else {
                        // move pipes
//                        printf("HELLO FROM PARENT PROCESS\n");

                        close(pipe_in[WRITE]);
                        pipe_in[READ] = pipe_out[READ];
                        pipe_in[WRITE] = pipe_out[WRITE];

                        if (pipe(pipe_out) != 0) {
                            printf("Error while moving pipe!\n");
                            exit(1);
                        }
                    }
                }
                ////////////
            }
            printf("KONIEC\n");
            ////
    }

        // wait for all the child processes to terminate
        int status = 0;
        pid_t wpid;
        while ((wpid = wait(&status)) != -1);
//        printf("\nALL CHILDREN TERMINATED\n");
    }

    // print lines
    i = 0;
    while(lines[i] != NULL) {
        printf("LINE%d:  %s\n", i + 1, lines[i]);
        i++;
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