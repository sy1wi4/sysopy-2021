//
// Created by sylwia on 5/8/21.
//


#include <stdio.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <time.h>
#include "shared.h"

int get_sem_id(){
    // connect to an already existing semaphore set
    key_t key = ftok(get_home_path(), ID);
    int sem_id;
    if((sem_id = semget(key, 2,0)) == -1){
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

void place_in_oven(oven* o, int type){
    // co jak pelny???
    o->arr[o->to_place_idx] = type;
    o->to_take_out_idx = o->to_place_idx;
    o->to_place_idx++;
    o->to_place_idx = o->to_place_idx % OVEN_SIZE;
    o->pizzas++;
}

int take_out_pizza(oven* o){
    int type = o->arr[o->to_take_out_idx];
    o->arr[o->to_take_out_idx] = -1;
    o->to_take_out_idx = -1;
    o->pizzas--;
    return type;
}


void place_on_table(table* t, int type){
    t->arr[t->to_place_idx] = type;
    t->to_take_out_idx = t->to_place_idx;
    t->to_place_idx++;
    t->to_place_idx = t->to_place_idx % OVEN_SIZE;
    t->pizzas++;
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

    srand(getpid());
    while(true){

        // preparing
        printf("\n-----------------  PREPARING  --------------\n");
        type = rand() % 10;
        printf("(pid: %d timestamp: %s)  ->   Przygotowuje pizze: %d\n\n", getpid(), get_current_time(), type);
        sleep(PREPARATION_TIME);

        // placing in oven
        lock_sem(sem_id, OVEN_SEM);
        printf("\n------------  PLACING IN OVEN  ------------\n");
        place_in_oven(o, type);
        printf("(pid: %d timestamp: %s)  ->   Dodałem pizze: %d. Liczba pizz w piecu: %d.\n", getpid(), get_current_time(), type, o->pizzas);
        printf("OVEN: ");
        print_arr(o->arr, OVEN_SIZE);
        unlock_sem(sem_id, OVEN_SEM);

        // baking
        printf("\n-----------------  BAKING  --------------\n\n");
        sleep(BAKING_TIME);


        // taking out
        lock_sem(sem_id, OVEN_SEM);
        printf("\n----------  TAKING OUT OF OVEN  ----------\n");
        type = take_out_pizza(o);
        printf("(pid: %d timestamp: %s)  ->   Wyjalem pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n", getpid(), get_current_time(), type, o->pizzas, t->pizzas);
        printf("OVEN: ");
        print_arr(o->arr, OVEN_SIZE);
        printf("OVEN_SEM value: %d\n", semctl(sem_id, OVEN_SEM, GETVAL));
        unlock_sem(sem_id, OVEN_SEM);
        printf("OVEN_SEM value: %d\n", semctl(sem_id, OVEN_SEM, GETVAL));


        // placing on the table
        // TODO: cos tu sie jebieeeeeeeeeeeeeeeeeeeeeeeee
        lock_sem(sem_id, TABLE_SEM);
        printf("\n------------  PLACING ON TABLE  ------------\n");
        place_on_table(t, type);
        printf("(pid: %d timestamp: %s)  ->   Umieszczam pizze na stole: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n", getpid(), get_current_time(), type, o->pizzas, t->pizzas);
        printf("TABLE: ");
        print_arr(t->arr, TABLE_SIZE);

        unlock_sem(sem_id, TABLE_SEM);

    }

    return 0;
}