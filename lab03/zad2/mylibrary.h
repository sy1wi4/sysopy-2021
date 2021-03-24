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
    struct Block** blocks;
};

struct MainArray* create_main_arr(int size);

struct Block* create_merged_block(char* file1, char* file2);

void merge_files(char* file1, char* file2);

int get_rows_number(struct Block* block);

void remove_block(struct MainArray* arr, int idx);

void remove_row(struct MainArray* arr, int block_idx, int row_idx);

void print_main_arr(struct MainArray* arr);

void print_block(struct Block* block);

#endif //LAB01_MYLIBRARY_H
