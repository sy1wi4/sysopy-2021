#include <stdio.h>
#include <stdlib.h>
#include "mylibrary.h"

// liczba par plikow i lista zadan do wykonania
int main(int argc, char *argv[]) {

    struct MainArray* main_arr = create_main_arr(3);
    main_arr->last_added_idx = -1;
    printf("xdfsdgs\n");


    if (argc == 3){
        printf("merged at idx: %d\n", merge_files(main_arr, argv[1], argv[2]));
    }

    if (argc == 5){
        int i = merge_files(main_arr, argv[1], argv[2]);
        printf("merged at idx: %d\n\n", i);
        main_arr->last_added_idx++;
        printf("merged at idx: %d\n\n", merge_files(main_arr, argv[3], argv[4]));
        main_arr->last_added_idx++;
        print_main_arr(main_arr);

        remove_block(main_arr, 1);
        remove_row(main_arr, 0,4);
        print_main_arr(main_arr);
    }

    return 0;
}
