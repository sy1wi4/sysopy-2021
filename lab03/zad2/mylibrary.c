//
// Created by sylwia on 3/10/21.
//

#include <stdlib.h>
#include <string.h>
#include "mylibrary.h"
#include <stdio.h>

struct MainArray* create_main_arr(int size){
    struct MainArray* arr = calloc(1, sizeof(struct MainArray));

    arr->size = size;
    arr->last_added_idx = -1;
    arr->blocks = calloc(size, sizeof(struct Block));
    printf("Created array of size %d\n\n", size);

    return arr;
}

struct Block* create_merged_block(char* file_name1, char* file_name2){
    struct Block* block = calloc(1, sizeof(struct Block));
    FILE* file1 = fopen(file_name1, "r");
    FILE* file2 = fopen(file_name2, "r");


    if(file1 == NULL || file2 == NULL) {
        perror("Cannot open file ");
        exit(1);
    }

    char* line1 = (char*)calloc(256,sizeof(char));
    char* line2 = (char*)calloc(256,sizeof(char));

    FILE* temp = tmpfile();
    temp = fopen("temp.txt","w");
    int lines = 0;
    while(fgets(line1, 256*sizeof(char), file1) && fgets(line2, 256*sizeof(char), file2)) {
        fputs(line1,temp);
        if (feof(file1)){
            fputs("\n", temp);
        }
        fputs(line2,temp);
        lines += 2;
    }
    fclose(temp);
    fopen("temp.txt", "r");

    block->rows = (char**)calloc(lines, sizeof(char*));

    int idx = 0;
    char* line = (char*)calloc(256,sizeof(char));
    while(fgets(line, 256*sizeof(char), temp)){
        char* row = (char*)calloc(256,sizeof(char));
        strcpy(row,line);
        block->rows[idx++] = row;

    }
    block->rows_number = idx;

    fclose(file1);
    fclose(file2);
    fclose(temp);

    return block;
}


void merge_files(char* file_name1, char* file_name2){

    FILE* file1 = fopen(file_name1, "r");
    FILE* file2 = fopen(file_name2, "r");

    if(file1 == NULL || file2 == NULL) {
        perror("Cannot open file ");
        exit(1);
    }

    char* line1 = (char*)calloc(256,sizeof(char));
    char* line2 = (char*)calloc(256,sizeof(char));

    FILE* temp = tmpfile();
    int lines = 0;
    while(fgets(line1, 256*sizeof(char), file1) && fgets(line2, 256*sizeof(char), file2)) {
        fputs(line1,temp);
        if (feof(file1)){
            fputs("\n", temp);
        }
        fputs(line2,temp);
        lines += 2;
    }


    rewind(temp);

//    char* buffer = calloc(256,sizeof(char));
//    while (fgets(buffer, 256, temp))
//        printf("PID: %d   %s", getpid(), buffer);
//    printf("\n");

    fclose(file1);
    fclose(file2);
    fclose(temp);
}

int get_rows_number(struct Block* block){
    return block->rows_number;
}

void remove_block(struct MainArray* arr, int idx){
    if (arr->blocks[idx] == NULL) return;
    free(arr->blocks[idx]);
    arr->blocks[idx] = NULL;
    printf("Removed block at idx %d\n\n", idx);
}

void remove_row(struct MainArray* arr, int block_idx, int row_idx){
    if (arr->blocks[block_idx] == NULL) return;
    if (arr->blocks[block_idx]->rows[row_idx] == NULL) return;
    free(arr->blocks[block_idx]->rows[row_idx]);
    arr->blocks[block_idx]->rows[row_idx] = NULL;
    printf("Removed row at idx %d (block %d)\n\n", row_idx, block_idx);
    print_block(arr->blocks[block_idx]);
}

void print_block(struct Block* block){
    for (int j = 0; j < block->rows_number; j++){
        if (block->rows[j] != NULL) {
            printf("%d: %s\n", j, block->rows[j]);
        }
    }
    printf("\n");
}

void print_main_arr(struct MainArray* arr){
    struct Block *block;
    for (int i = 0; i < arr->size; i++){
        block = arr->blocks[i];
        if (block == NULL) continue;
        printf("BLOCK NUMBER %d\n", i);
        for (int j = 0; j < block->rows_number; j++){
            if (block->rows[j] != NULL) {
                printf("%d: %s\n", j, block->rows[j]);
            }
        }
        printf("\n");
    }
}


