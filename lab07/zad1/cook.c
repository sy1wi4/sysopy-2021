//
// Created by sylwia on 5/8/21.
//


#include <stdio.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <stdbool.h>
#include "shared.h"

int get_sem_id(){
    // connect to an already existing semaphore set
    key_t key = ftok(get_home_path(), ID);
    int sem_id;
    if((sem_id = semget(key, 2, 0)) == -1){
        printf("Error while connecting to semaphore set!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    return sem_id;
}

int get_shm_oven_id(){
    key_t key_o = ftok(OVEN_PATH, OVEN_ID);
    int shm_oven_id;
    if ((shm_oven_id = shmget(key_o, sizeof(oven), 0)) == -1){
        printf("Error while getting oven id!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    return shm_oven_id;
}


int get_shm_table_id(){
    key_t key_t = ftok(TABLE_PATH, TABLE_ID);
    int shm_table_id;
    if ((shm_table_id = shmget(key_t, sizeof(table), 0)) == -1){
        printf("Error while getting table id!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    return shm_table_id;
}


int main(){
    printf("Hello, cook here!     [%d]\n", getpid());

    int sem_id = get_sem_id();
    int shm_oven_id = get_shm_oven_id();
    int shm_table_id = get_shm_table_id();

//    printf("Cook connected!  sem_id: %d, oven_id: %d, table_id: %d \n\n", sem_id, shm_oven_id, shm_table_id);

    oven* o = shmat(shm_oven_id, NULL, 0);
    table* t = shmat(shm_table_id, NULL, 0);

    printf("\n----- OVEN -----\n");
    print_arr(o->arr, OVEN_SIZE);
    printf("----- TABLE ----\n");
    print_arr(t->arr, TABLE_SIZE);
    printf("\n");

    int type;

//    while(true){
//        type = rand() % 10;
//
//        sleep(PREPARATION_TIME);
//    }

    return 0;
}