#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylibrary.h"
#include <sys/times.h>
#include <unistd.h>

long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms){
    printf("real %Lf\n", time_sec(clock_end - clock_start));
    printf("user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}

int main(int argc, char* argv[]) {

    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;

    struct MainArray *main_arr = NULL;

    for (int i = 1; i < argc; i++) {
        clock_start = times(&start_tms);

        if (strcmp(argv[i], "create_table") == 0) {
            int size = atoi(argv[++i]);
            main_arr = create_main_arr(size);

            printf("create_table\n");

        }

        else if (strcmp(argv[i], "merge_files") == 0) {
            char* file1 = (char*)calloc(strlen(argv[i]), sizeof(char));
            char* file2 = (char*)calloc(strlen(argv[i]), sizeof(char));

            while (i < argc - 1 && strchr(argv[++i], ':')) {
                int file1_len = strchr(argv[i], ':') - argv[i];
                for (int a = 0; a < file1_len; a++){
                    file1[a] = argv[i][a];
                }
                for(int b = file1_len + 1; b < strlen(argv[i]); b++){
                    file2[b-file1_len-1] = argv[i][b];
                }

                merge_files(main_arr,file1,file2);
//                printf("");
            }
            if (i != argc - 1) i--;

            printf("merge_files\n");

        }
        else if (strcmp(argv[i], "remove_block") == 0){
            int idx = atoi(argv[++i]);
            remove_block(main_arr, idx);

            printf("remove_block\n");

        }

        else if (strcmp(argv[i], "remove_row") == 0){
            int block_idx = atoi(argv[++i]);
            int row_idx = atoi(argv[++i]);
            remove_row(main_arr, block_idx, row_idx);

            printf("remove_row\n");

        }

        clock_end = times(&end_tms);
        print_res(clock_start, clock_end, start_tms, end_tms);

    }
    clock_end = times(&end_tms);
    printf("total_execution\n");
    print_res(clock_start, clock_end, start_tms, end_tms);


    return 0;
}
