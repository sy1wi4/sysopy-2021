//
// Created by sylwia on 3/17/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void find_char_lib(char ch, char* filename){
    FILE* file = fopen(filename, "r");

    if (file == NULL ) {
        perror("Cannot open file");
        exit(1);
    }

    int size;
    bool found;
    char buffer;
    char *row;

    do {
        found = false;
        size = 0;
        do {
            fread(&buffer, sizeof(char), 1, file);
            if (feof(file)) break;
            if (ch == buffer) {
                found = true;
            }
            size++;
        } while (buffer != '\n');

        if (found) {
            fseek(file, -1 * (size), 1);   // go backward
            row = calloc(sizeof(char), size - 1);
            fread(row, sizeof(char), size - 1, file);
            printf("%s\n", row);
        }
    } while(!feof(file));

    fclose(file);
}

void find_char_sys(char ch, char* filename) {
    int file_d = open(filename, O_RDONLY);

    if (file_d < 0) {
        perror("Cannot open file");
        exit(1);
    }

    int size;
    bool found;
    char buffer;
    bool end = false;

    while(!end) {
        found = false;
        size = 0;
        do {
            if (read(file_d, &buffer, sizeof(char)) == 0) {
                end = true;
                break;
            }
            if (ch == buffer) {
                found = true;
            }
            size++;
        } while (buffer != '\n');


        if (found) {
            lseek(file_d, -1 * (size), SEEK_CUR);   // go backward
            char *row = calloc(sizeof(char), size - 1);
            read(file_d, row, (size - 1) * sizeof(char));
            read(file_d, &buffer, sizeof(char));
            printf("%s\n", row);
        }
    }

    close(file_d);
}



int main(int argc, char* argv[]) {

    char ch;
    char* filename;

    if (argc == 3){
        ch = *(argv[1]);   // ch = argv[1][0]
        filename = argv[2];
        printf("Char: %c\n", ch);
        printf("File: %s\n\n", filename);

        printf("------LIB------\n\n");
        find_char_lib(ch, filename);
        printf("\n\n------SYS------\n\n");
        find_char_sys(ch, filename);

    }

    else {
        perror("Wrong number of arguments!");
        exit(1);
    }

    return 0;
}