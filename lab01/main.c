#include <stdio.h>
#include <stdlib.h>
#include "mylibrary.h"

// liczba par plikow i lista zadan do wykonania
int main(int argc, char *argv[]) {

    struct  MainArray main_arr = create_main_arr(10);
    main_arr.last_added_idx = -1;


    struct Block block;
    block.rows = (char **)calloc(5, sizeof(char*));

    if (argc == 3){
        printf("merged at idx: %d\n", merge_files(main_arr, argv[1], argv[2]));
    }

    if (argc == 5){
        // TODO: czemu tu sie nie zmienia last_added_idx ???
        printf("merged at idx: %d\n", merge_files(main_arr, argv[1], argv[2]));
//        printf("no i co %s\n\n", main_arr.blocks[0].rows[0]);
        printf("ostatni lur %d\n", main_arr.last_added_idx);
        printf("merged at idx: %d\n", merge_files(main_arr, argv[3], argv[4]));
        printf("ostatni lur %d\n", main_arr.last_added_idx);

    }

    return 0;
}
