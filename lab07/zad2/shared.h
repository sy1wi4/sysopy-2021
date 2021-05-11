//
// Created by sylwia on 5/8/21.
//

#ifndef LAB07_SHARED_H
#define LAB07_SHARED_H

#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdbool.h>


#define OVEN_SIZE 5
#define TABLE_SIZE 5

#define OVEN_SEM "/OVEN_S"
#define TABLE_SEM "/TABLE_S"
#define FULL_OVEN_SEM "/FULL_OVEN_S"
#define FULL_TABLE_SEM "/FULL_TABLE_S"
#define EMPTY_TABLE_SEM "/EMPTY_TABLE_S"

#define PREPARATION_TIME 1
#define BAKING_TIME 4
#define DELIVERY_TIME 4
#define RETURN_TIME 4


typedef struct{
    int arr[OVEN_SIZE];
    int pizzas;
    int to_place_idx;
    int to_take_out_idx;
} oven;

typedef struct{
    int arr[TABLE_SIZE];
    int pizzas;
    int to_place_idx;
    int to_take_out_idx;
} table;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
} arg;


void lock_sem(sem_t* sem){
//    int val;
//    sem_getvalue(sem, &val);
//    printf("lock,  sem value: %d\n", val);

    // decrement sem
    if (sem_wait(sem) == -1){
        printf("Error while locking semaphore\n");
        exit(1);
    }
//    sem_getvalue(sem, &val);
//    printf("locked,  sem value: %d\n", val);
}

void unlock_sem(sem_t* sem) {
//    int val;
//    sem_getvalue(sem, &val);
//    printf("unlock,  sem value: %d\n", val);

    // increment sem
    if (sem_post(sem) == -1){
        printf("Error while unlocking semaphore\n");
        exit(1);
    }
//    sem_getvalue(sem, &val);
//    printf("unlocked,  sem value: %d\n", val);
}

sem_t* get_sem(char* name){
    // connect to an already existing semaphore
    sem_t* sem;
    if((sem = sem_open(name, O_RDWR)) == SEM_FAILED){
        printf("Error while connecting to semaphore %s!\n", name);
        printf("%s\n", strerror(errno));
        exit(1);
    }
    int val;
    sem_getvalue(sem, &val);
    printf("sem: %s,  val: %d\n", name, val);
    return sem;
}

int get_shm_oven_fd(){
    int shm_oven_fd;
    if ((shm_oven_fd = shm_open("/shm_oven", O_RDWR , 0666))== -1){
        printf("Error while getting oven id!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    return shm_oven_fd;
}


int get_shm_table_fd(){
    int shm_table_fd;
    if ((shm_table_fd = shm_open("/shm_table", O_RDWR , 0666)) == -1){
        printf("Error while getting table id!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    return shm_table_fd;
}

void print_arr(int* arr, int size){
    for (int i = 0; i < size; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}

char* get_current_time(){
    struct timeval cur_time;
    gettimeofday(&cur_time, NULL);
    int millisec = cur_time.tv_usec / 1000;

    char* buffer = calloc(80, sizeof(char));
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&cur_time.tv_sec));

    char* current_time = calloc(84, sizeof(char));
    sprintf(current_time, "%s:%03d", buffer, millisec);
    return current_time;
}

#endif //LAB07_SHARED_H
