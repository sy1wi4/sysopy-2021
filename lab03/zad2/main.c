#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylibrary.h"
#include <sys/times.h>
#include <unistd.h>
#include <sys/wait.h>

long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms){
    printf("real %Lf\n", time_sec(clock_end - clock_start));
    printf("user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}

int main(int argc, char *argv[]) {
    // make ARGS="f1.txt:f2.txt f3.txt:f4.txt"

    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;

    clock_start = times(&start_tms);

    pid_t pid;
    for (int i = 1; i < argc ; i++) {
        char* file1 = (char*)calloc(strlen(argv[i]), sizeof(char));
        char* file2 = (char*)calloc(strlen(argv[i]), sizeof(char));

        pid = fork();
        if (pid == 0) {
            int file1_len = strchr(argv[i], ':') - argv[i];
            for (int a = 0; a < file1_len; a++) {
                file1[a] = argv[i][a];
            }
            for (int b = file1_len + 1; b < strlen(argv[i]); b++) {
                file2[b - file1_len - 1] = argv[i][b];
            }
            merge_files(file1, file2);

            exit(0);
        }
    }

    for (int i = 1; i < argc ; i++) wait(NULL);

    clock_end = times(&end_tms);
    printf("TOTAL EXECUTION TIME\n");
    print_res(clock_start, clock_end, start_tms, end_tms);

    return 0;
}
