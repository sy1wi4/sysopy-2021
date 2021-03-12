#include <stdio.h>
#include <stdlib.h>
#include "mylibrary.h"

// liczba par plikow i lista zadan do wykonania
int main(int argc, char *argv[]) {

    int arg;
    for (arg = 0; arg < argc; ++arg)
    {
        printf("Arg %d is %s\n", arg, argv[arg]);
    }
    struct  MainArray main_arr = create_main_arr(10);
    main_arr.last_added_idx = -1;


    struct Block block;
    block.rows = (char **)calloc(5, sizeof(char*));
    main_arr.blocks[0] = block;
    print_block(main_arr.blocks[0]);

    if (argc == 3){
        merge_files(main_arr, argv[1], argv[2]);
    }

    return 0;
}
