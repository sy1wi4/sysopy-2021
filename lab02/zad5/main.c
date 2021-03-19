//
// Created by sylwia on 3/19/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>


char*  break_lines(char* line){
    // count occurrences of str1 in line
    int ctr = strlen(line) / 50;
    char* occ;

    char* new_line;
    new_line = calloc(sizeof(char), strlen(line) + ctr + 1);

    int idx = 0;  // of new line
    int current = 0;
    while(*line){
        current++;
        // swap words
        if (current % 51 == 0){
            strcpy(&new_line[idx], "\n");
            idx += 1;
        }
        else{
            strcpy(&new_line[idx], line);
            *line++;
            idx++;
        }
    }
    return new_line;
}


void read_lines_lib(char* input_file, char* output_file){
    FILE* i_file = fopen(input_file, "r");
    FILE* o_file = fopen(output_file, "w");

    if (i_file == NULL ) {
        perror("Cannot open file");
        exit(1);
    }

    char* line;
    char* new_line;
    char buffer;
    int size = 0;

    while(true){
        fread(&buffer, sizeof(char), 1, i_file);
        size++;

        if (buffer == '\n' ) {
            fseek(i_file, -1 * (size), 1);   // go backward
            line = calloc(sizeof(char), size - 1);
            fread(line, sizeof(char), size - 1, i_file);
            fread(&buffer, sizeof(char), 1, i_file);  // '\n'
            if (size == 1) break;
            new_line = break_lines(line);

            fwrite(new_line, sizeof(char), strlen(new_line), o_file);
            fwrite("\n", sizeof(char), 1, o_file);
            size = 0;
        }
    }

    fclose(i_file);
    fclose(o_file);
}

void read_lines_sys(char* input_file, char* output_file) {

    int i_file = open(input_file, O_RDONLY);
    int o_file = open(output_file,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (i_file < 0) {
        perror("Cannot open file");
        exit(1);
    }

    char* line;
    char* new_line;
    char buffer;
    int size = 0;

    while(true){
        read(i_file, &buffer, sizeof(char));
        size++;

        if (buffer == '\n' ) {
            lseek(i_file, -1 * (size), SEEK_CUR);   // go backward
            line = calloc(sizeof(char), size - 1);
            read(i_file, line, (size - 1) * sizeof(char));
            read(i_file, &buffer, sizeof(char));
            if (size == 1) break;
            new_line = break_lines(line);

            write(o_file, new_line, sizeof(char) * strlen(new_line));
            write(o_file, "\n", sizeof(char));
            size = 0;
        }

    }

    close(i_file);
    close(o_file);
}


int main(int argc, char* argv[]) {

    // poem.txt output5.txt

    if (argc != 3){
        perror("Wrong number of arguments!");
        exit(1);
    }

    char* input_file = argv[1];
    char* output_file = argv[2];

    read_lines_lib(input_file, output_file);
    read_lines_sys(input_file, output_file);

    return 0;
}