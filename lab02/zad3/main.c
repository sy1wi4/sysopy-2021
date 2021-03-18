//
// Created by sylwia on 3/18/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

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

    FILE* a_lib = fopen("a_lib.txt", "w");
    FILE* b_lib = fopen("b_lib.txt", "w");
    FILE* c_lib = fopen("c_lib.txt", "w");

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

    printf("%d", even);

    fwrite("Liczb parzystych jest ", sizeof(char), 22, a_lib);
    str = int_to_string(even);
    fwrite(str, sizeof(char), strlen(str), a_lib);

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

    int a_sys = open("a_sys.txt",O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    int b_sys = open("b_sys.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    int c_sys = open("c_sys.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

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


    close(file_d);
    close(a_sys);
    close(b_sys);
    close(b_sys);
}


int main() {

    create_files_lib("dane.txt");
    create_files_sys("dane.txt");
    return 0;
}