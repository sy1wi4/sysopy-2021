//
// Created by sylwia on 3/10/21.
//

#ifndef LAB01_MYLIBRARY_H
#define LAB01_MYLIBRARY_H

#include <stdio.h>

struct Block{
    int rows_number;
    char** rows;
};

struct MainArray{
    int size;
    int last_added_idx;
    struct Block* blocks;
};

struct MainArray create_main_arr(int size);


struct Block create_merged_block(char* file1, char* file2);

int merge_files(struct MainArray main_arr, char* file1, char* file2);

//test
void print_arr(struct MainArray arr);

void print_block(struct Block block);

#endif //LAB01_MYLIBRARY_H
