//
// Created by sylwia on 3/18/21.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
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

char*  replace(char* line, char* str1, char* str2){
    // count occurrences of str1 in line
    int ctr = 0;
    int i;
    char* occ;
    for (i = 0; i < strlen(line); i++){
        occ = strstr(&line[i], str1);
        if (occ == &line[i]) {
            ctr++;
            i += strlen(str1) - 1;
        }
    }

    char* new_line;
    new_line = calloc(sizeof(char), i + ctr * (strlen(str2) - strlen(str1)) + 1);

    int idx = 0;  // of new line
    while(*line){
        occ = strstr(line, str1);
        // swap words
        if (occ == line){
            strcpy(&new_line[idx], str2);
            idx += strlen(str2);
            line += strlen(str1);
        }
        else{
            strcpy(&new_line[idx], line);
            *line++;
            idx++;
        }
    }
    return new_line;
}


void read_lines_lib(char* input_file, char* output_file, char* str1, char* str2){
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
            new_line = replace(line, str1, str2);

            fwrite(new_line, sizeof(char), strlen(new_line), o_file);
            fwrite("\n", sizeof(char), 1, o_file);
            size = 0;
        }

    }

    fclose(i_file);
    fclose(o_file);
}

void read_lines_sys(char* input_file, char* output_file, char* str1, char* str2) {

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
            new_line = replace(line, str1, str2);

            write(o_file, new_line, sizeof(char) * strlen(new_line));
            write(o_file, "\n", sizeof(char));
            size = 0;
        }

    }

    close(i_file);
    close(o_file);

}


int main(int argc, char* argv[]) {

    // lorem.txt output.txt Lorem REPLACED
    // make ARGS="file.txt output.txt to REPLACED"

    if (argc != 5){
        printf("Wrong number of arguments!\n");
        return 1;
    }

    // execution time
    FILE* result_file = fopen("pomiar_zad_5.txt", "w");
    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;
    struct tms start_tms2;
    struct tms end_tms2;
    clock_t clock_start2;
    clock_t clock_end2;


    char* input_file = argv[1];
    char* output_file = argv[2];
    char* str1 = argv[3];
    char* str2 = argv[4];


    clock_start = times(&start_tms);
    read_lines_lib(input_file, output_file, str1, str2);
    clock_end = times(&end_tms);

    clock_start2 = times(&start_tms2);
    read_lines_sys(input_file, output_file, str1, str2);
    clock_end2 = times(&end_tms2);


    fprintf(result_file, "\n------LIB-----");
    printf("\n------LIB-----");
    print_res(clock_start, clock_end, start_tms, end_tms,result_file);

    fprintf(result_file, "\n------SYS-----");
    printf("\n------SYS-----");
    print_res(clock_start2, clock_end2, start_tms2, end_tms2, result_file);

    return 0;
}