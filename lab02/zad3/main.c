//
// Created by sylwia on 3/18/21.
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


bool check_digit(int n){
    if (n / 10 == 0) return false;
    else if ((n / 10) % 10 == 0 || (n / 10) % 10 == 7) return true;
    return false;
}

bool is_square(int n){
    if (n == 0) return true;
    for (int i = 1; i * i <= n; i++) {
        if ((n % i == 0) && (n / i == i)) {
            return true;
        }
    }
    return false;
}

char* int_to_string(int n){
    int digits = 0;
    int cur = n;
    while(cur != 0){
        cur /= 10;
        ++digits;
    }

    char* result = calloc(digits, sizeof(char));
    sprintf(result, "%d", n);
    return result;
}

void create_files_lib(char* filename){

    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        perror("Cannot open file");
        exit(1);
    }

    FILE* a_lib = fopen("a.txt", "w");
    FILE* b_lib = fopen("b.txt", "w");
    FILE* c_lib = fopen("c.txt", "w");

    int even = 0, num, size;
    char buffer;
    char* number;
    char* str;
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

        num = atoi(number);

        if (num % 2 == 0) even++;

        if (check_digit(num)) {
            str = int_to_string(num);
            fwrite(str, sizeof(char), strlen(str), b_lib);
            fwrite("\n", sizeof(char), 1, b_lib);
        }

        if (is_square(num)) {
            str = int_to_string(num);
            fwrite(str, sizeof(char), strlen(str), c_lib);
            fwrite("\n", sizeof(char), 1, c_lib);
        }
    }

    fwrite("Liczb parzystych jest ", sizeof(char), 22, a_lib);
    str = int_to_string(even);
    fwrite(str, sizeof(char), strlen(str), a_lib);
    fwrite("\n", sizeof(char), 1, a_lib);

    fclose(file);
    fclose(a_lib);
    fclose(b_lib);
    fclose(c_lib);
}


void create_files_sys(char* file){

    int file_d = open(file, O_RDONLY);

    if (file_d < 0) {
        perror("Cannot open file");
        exit(1);
    }

    int a_sys = open("a.txt",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    int b_sys = open("b.txt",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    int c_sys = open("c.txt",O_WRONLY | O_CREAT |O_TRUNC, S_IRUSR | S_IWUSR);

    int even = 0, num, size;
    char buffer;
    char* number;
    char* str;
    bool end = false;

    while(!end) {
        size = -1;
        do {
            if (read(file_d, &buffer, sizeof(char)) == 0) {
                end = true;
                break;
            }

            size++;
        } while (buffer != '\n');

        lseek(file_d, -1 * (size + 1), SEEK_CUR);   // go backward
        number = calloc(sizeof(char), size);
        read(file_d, number, (size + 1) * sizeof(char));

        num = atoi(number);

        if (num % 2 == 0) even++;

        if (check_digit(num)) {
            str = int_to_string(num);
            write(b_sys, str, strlen(str));
            write(b_sys, "\n", 1);
        }

        if (is_square(num)) {
            str = int_to_string(num);
            write(c_sys, str, strlen(str));
            write(c_sys, "\n", 1);

        }
    }

    write(a_sys, "Liczb parzystych jest ", 22 * sizeof(char));
    str = int_to_string(even);
    write(a_sys, str, strlen(str));
    write(a_sys, "\n", 1);

    close(file_d);
    close(a_sys);
    close(b_sys);
    close(b_sys);
}


int main() {

    // execution time
    FILE* result_file = fopen("pomiar_zad_3.txt", "w");
    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;
    struct tms start_tms2;
    struct tms end_tms2;
    clock_t clock_start2;
    clock_t clock_end2;


    clock_start = times(&start_tms);
    create_files_lib("dane.txt");
    clock_end = times(&end_tms);

    clock_start2 = times(&start_tms2);
    create_files_sys("dane.txt");
    clock_end2 = times(&end_tms2);


    fprintf(result_file, "\n------LIB-----");
    printf("\n------LIB-----");
    print_res(clock_start, clock_end, start_tms, end_tms,result_file);

    fprintf(result_file, "\n------SYS-----");
    printf("\n------SYS-----");
    print_res(clock_start2, clock_end2, start_tms2, end_tms2, result_file);


    return 0;
}