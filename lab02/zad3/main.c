//
// Created by sylwia on 3/18/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void create_files_lib(char* filename){

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Cannot open file");
        exit(1);
    }

    FILE* a = fopen("a.txt", "w");

    char buffer;
    int even = 0;
    int size;
    char* number;
    bool end = false;

    while(!end) {
        size = -1;
        do {
            fread(&buffer, sizeof(char), 1, file);
            if (feof(file)) {
                end = true;
                break;
            }
            size++;
        } while (buffer != '\n');


        fseek(file, -1 * (size + 1), 1);   // go backward
        number = calloc(sizeof(char), size);
        fread(number, sizeof(char), size + 1, file);

        if (atoi(number) % 2 == 0) even++;
    }

    printf("%d", even);

    int digits = 0;
    int e = even;
    while(e != 0){
        e /= 10;
        ++digits;
    }
    char result[digits];
    sprintf(result, "%d", even);
    fwrite(result, sizeof(char), digits, a);
}


void create_files_sys(char* file){

}



int main() {

    printf("------LIB------\n\n");
    create_files_lib("dane.txt");
    printf("\n\n------SYS------\n\n");
    create_files_sys("dane.txt");

    return 0;
}