#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylibrary.h"

int main(int argc, char *argv[]) {

    struct MainArray *main_arr = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "create_table") == 0) {
            int size = atoi(argv[++i]);
            main_arr = create_main_arr(size);
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

                printf("file1: %s\n", file1);
                printf("file2: %s\n", file2);
                merge_files(main_arr,file1,file2);
                printf("\n");
            }
            if (i != argc - 1) i--;
        }
        else if (strcmp(argv[i], "remove_block") == 0){
            int idx = atoi(argv[++i]);
            remove_block(main_arr, idx);
        }

        else if (strcmp(argv[i], "remove_row") == 0){
            int block_idx = atoi(argv[++i]);
            int row_idx = atoi(argv[++i]);
            remove_row(main_arr, block_idx, row_idx);
        }
    }

    return 0;
}
