//
// Created by sylwia on 5/8/21.
//

#ifndef LAB07_SHARED_H
#define LAB07_SHARED_H

#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>

#define ID 'P'

#define OVEN_ID 'O'
#define TABLE_ID 'T'

#define OVEN_PATH "oven"
#define TABLE_PATH "table"

#define OVEN_SIZE 5
#define TABLE_SIZE 5

#define OVEN_SEM 0
#define TABLE_SEM 1

#define PREPARATION_TIME 1
#define BAKING_TIME 4
#define DELIVERY_TIME 4

char* get_home_path(){
    char* path = getenv("HOME");
    if (path == NULL) {
        path = getpwuid(getuid())->pw_dir;
    }
    return path;
}

typedef struct{
    int arr[OVEN_SIZE];
    int pizzas;
} oven;

typedef struct{
    int arr[TABLE_SIZE];
    int pizzas;
} table;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
} arg;

void print_arr(int* arr, int size){
    for (int i = 0; i < size; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}

#endif //LAB07_SHARED_H
