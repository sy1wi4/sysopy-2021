//
// Created by sylwia on 4/15/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>


void save_to_file(FILE* file, char* message, int row){
    rewind(file);
    int fd = fileno(file);
    char buffer[257];
    int counter = 0;
    int i;

    flock(fd, LOCK_EX);

    printf("Going to save line to result file (row %d)...\n", row);
    while (fgets(buffer,  257, file) != NULL){
        if (counter == row) {

            for (i = 0; i < 256; i++){
                if (buffer[i] == ' ') break;
            }
            fseek(file, row * 256 + i, SEEK_SET);
            printf("Write mess: %s\n\n", message);
            fprintf(file, message, strlen(message) + 1);
            fflush(file);
            break;
        }
        counter++;
    }

    flock(fd, LOCK_UN);
}

int main(int argc, char* argv[]){

    if (argc != 4){
        printf("Wrong number of arguments!");
        exit(1);
    }


    char* pipe_path = argv[1];
    char* file_path = argv[2];
    int N = atoi(argv[3]);


    FILE* pipe = fopen(pipe_path, "r");
    if(pipe < 0) {
        printf("Pipe error!\n");
    }

    FILE* file = fopen(file_path, "r+");
    if(file < 0) {
        printf("File opening error!\n");
    }

    char buffer[N];

    while(fread(buffer, sizeof(char), N, pipe) == N){
        printf("CONSUMER READ FROM PIPE: %s", buffer);

        char* seq = strtok(buffer, ".");   // separate number of producer (before the dot)
        int num = atoi(seq);

        seq = strtok(NULL, "\n");

        char* line;
        sprintf(line, "%s", seq);


        save_to_file(file, line, num);
    }

    fclose(pipe);
    fclose(file);

    return 0;
}
