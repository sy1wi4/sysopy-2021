//
// Created by sylwia on 3/17/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
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

void open_lib(char* filename1, char* filename2){
    FILE* file1 = fopen(filename1, "r");
    FILE* file2 = fopen(filename2, "r");

    if (file1 == NULL || file2 == NULL) {
        perror("Cannot open file");
        exit(1);
    }

    char ch1, ch2;

    do {
        do {
            fread(&ch1, sizeof(char), 1, file1);
            if (feof(file1)) break;
            fprintf(stdout, "%c", ch1);
        } while(ch1 != '\n');

        do {
            fread(&ch2, sizeof(char), 1, file2);
            if (feof(file2)) break;
            fprintf(stdout, "%c", ch2);
        } while(ch2 != '\n');

    } while(!feof(file1) || !feof(file2));
    fclose(file1);
    fclose(file2);
}

void open_sys(char* filename1, char* filename2){
    int file_d1 = open(filename1, O_RDONLY);
    int file_d2 = open(filename2, O_RDONLY);

    if (file_d1 < 0 || file_d2 < 0) {
        perror("Cannot open file");
        exit(1);
    }

    char ch1, ch2;

    bool end1 = false;
    bool end2 = false;

    while(end1 == false || end2 == false) {
        do {
            if (read(file_d1, &ch1, sizeof(char)) == 0) {
                end1 = true;
                break;
            }
            fprintf(stdout, "%c", ch1);
        } while(ch1 != '\n');

        do {
            if (read(file_d2, &ch2, sizeof(char)) == 0) {
                end2 = true;
                break;
            }
            fprintf(stdout, "%c", ch2);
        } while(ch2 != '\n');

    }


    close(file_d1);
    close(file_d2);
}


int main(int argc, char* argv[]){
    // poem.txt lorem.txt
    // make ARGS="file1.txt file2.txt"

    char* filename1;
    char* filename2;

    if (argc == 3){
        filename1 = argv[1];
        filename2 = argv[2];
        printf("Loaded files:\n");
        printf("%s %s\n", filename1, filename2);
    }
    else {
        filename1 = calloc(256, sizeof(char));
        filename2 = calloc(256, sizeof(char));
        printf("Please enter filenames:\n");
        scanf("%s", filename1);
        scanf("%s", filename2);
        printf("Loaded files:\n");
        printf("%s %s\n", filename1, filename2);
    }

    // execution time
    FILE* result_file = fopen("pomiar_zad_1.txt", "w");
    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;
    struct tms start_tms2;
    struct tms end_tms2;
    clock_t clock_start2;
    clock_t clock_end2;



    printf("\n------------LIB------------\n");
    clock_start = times(&start_tms);
    open_lib(filename1, filename2);
    clock_end = times(&end_tms);

    printf("\n\n------------SYS------------\n");
    clock_start2 = times(&start_tms2);
    open_sys(filename1, filename2);
    clock_end2 = times(&end_tms2);


    fprintf(result_file, "\n------LIB-----");
    printf("\n------LIB-----");
    print_res(clock_start, clock_end, start_tms, end_tms,result_file);

    fprintf(result_file, "\n------SYS-----");
    printf("\n------SYS-----");
    print_res(clock_start2, clock_end2, start_tms2, end_tms2, result_file);

    return 0;
}