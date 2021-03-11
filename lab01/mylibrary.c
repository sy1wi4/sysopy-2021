//
// Created by sylwia on 3/10/21.
//

#include <stdlib.h>
#include "mylibrary.h"

struct MainArray create_main_arr(int size){
    struct MainArray arr;
    arr.size = size;
    arr.blocks = (struct Block*)calloc(size, sizeof(struct Block));

    return arr;
}

//test
void print_arr(struct MainArray arr){
    printf("size: %d\n", arr.size);
}

void print_block(struct Block block){
    printf("\n%s", block.rows[0]);
}
