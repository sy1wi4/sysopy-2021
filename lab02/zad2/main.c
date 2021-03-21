//
// Created by sylwia on 3/17/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/times.h>


long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms, FILE* file){
    printf("\nEXECUTION TIME\n");
    printf("real %Lf\n", time_sec(clock_end - clock_start));
    printf("user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
    fprintf(file, "\nEXECUTION TIME\n");
    fprintf(file, "real %Lf\n", time_sec(clock_end - clock_start));
    fprintf(file, "user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    fprintf(file, "sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}

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
    // f poem.txt

    // execution time
    FILE* result_file = fopen("pomiar_zad_2.txt", "w");
    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;
    struct tms start_tms2;
    struct tms end_tms2;
    clock_t clock_start2;
    clock_t clock_end2;


    char ch;
    char* filename;

    if (argc == 3){
        ch = *(argv[1]);   // ch = argv[1][0]
        filename = argv[2];
        printf("Char: %c\n", ch);
        printf("File: %s\n\n", filename);

        printf("------LIB------\n\n");
        clock_start = times(&start_tms);
        find_char_lib(ch, filename);
        clock_end = times(&end_tms);

        printf("\n\n------SYS------\n\n");
        clock_start2 = times(&start_tms2);
        find_char_lib(ch, filename);
        clock_end2 = times(&end_tms2);
    }

    else {
        printf("Wrong number of arguments!\n");
        return 1;
    }

    fprintf(result_file, "\n------LIB-----");
    printf("\n------LIB-----");
    print_res(clock_start, clock_end, start_tms, end_tms,result_file);

    fprintf(result_file, "\n------SYS-----");
    printf("\n------SYS-----");
    print_res(clock_start2, clock_end2, start_tms2, end_tms2, result_file);


    return 0;
}