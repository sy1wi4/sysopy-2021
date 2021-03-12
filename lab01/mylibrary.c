//
// Created by sylwia on 3/10/21.
//

#include <stdlib.h>
#include <string.h>
#include "mylibrary.h"

struct MainArray create_main_arr(int size){
    struct MainArray arr;
    arr.size = size;
    arr.blocks = (struct Block*)calloc(size, sizeof(struct Block));

    return arr;
}

struct Block create_merged_block(char* file_name1, char* file_name2){
    struct Block block;

    printf("\nmerguj to\n\n");
    FILE* file1 = fopen(file_name1, "r");
    FILE* file2 = fopen(file_name2, "r");


    if(file1 == NULL || file2 == NULL) {
        perror("Cannot open file ");
        exit(1);
    }

    char line1[256];
    char line2[256];

    FILE* temp = tmpfile();
    temp = fopen("temp.txt","w");
    int lines = 0;
    while(fgets(line1, sizeof(line1), file1) && fgets(line2, sizeof(line2), file2)) {
        fputs(line1,temp);
        if (feof(file1)){
            fputs("\n", temp);
        }
        fputs(line2,temp);
        lines += 2;
    }
    fclose(temp);
    fopen("temp.txt", "r");

    block.rows = (char**)calloc(lines, sizeof(char*));

    int idx = 0;
    char* line = (char*)calloc(256,sizeof(char));
    while(fgets(line, 256*sizeof(char), temp)){
        char* row = (char*)calloc(256,sizeof(char));
        strcpy(row,line);
        block.rows[idx++] = row;

        printf("%s\n", line);
    }

    block.rows_number = idx;

    fclose(file1);
    fclose(file2);
    fclose(temp);

    return block;
}


int merge_files(struct MainArray main_arr, char* file1, char* file2){
    struct Block new_block = create_merged_block(file1, file2);
    main_arr.blocks[main_arr.last_added_idx++] = new_block;
    printf("%d\n", new_block.rows_number);
    return main_arr.last_added_idx;
}

//test
void print_arr(struct MainArray arr){
    printf("number of blocks: %d\n", arr.size);
}

void print_block(struct Block block){
    printf("\n%s", block.rows[0]);
}
