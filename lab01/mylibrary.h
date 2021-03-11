//
// Created by sylwia on 3/10/21.
//

#ifndef LAB01_MYLIBRARY_H
#define LAB01_MYLIBRARY_H

#include <stdio.h>

struct Block{
    char** rows;
};

struct MainArray{
    int size;
    struct Block* blocks;
};

struct MainArray create_main_arr(int size);


// merge i zapisz do pliku
// na razie merge i stworz nowy blok
struct Block marge_files(){

}




//test
void print_arr(struct MainArray arr);

void print_block(struct Block block);

#endif //LAB01_MYLIBRARY_H
