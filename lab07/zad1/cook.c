//
// Created by sylwia on 5/8/21.
//


#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <stdbool.h>
#include "shared.h"


void place_in_oven(oven* o, int type){
    o->arr[o->to_place_idx] = type;
    o->to_place_idx++;
    o->to_place_idx = o->to_place_idx % OVEN_SIZE;
    o->pizzas++;
}

int take_out_pizza(oven* o){
    int type = o->arr[o->to_take_out_idx];
    o->arr[o->to_take_out_idx] = -1;
    o->to_take_out_idx++;
    o->to_take_out_idx = o->to_take_out_idx % OVEN_SIZE;
    o->pizzas--;
    return type;
}


void place_on_table(table* t, int type){
    t->arr[t->to_place_idx] = type;
    t->to_place_idx++;
    t->to_place_idx = t->to_place_idx % OVEN_SIZE;
    t->pizzas++;
}

int main(){

    int sem_id = get_sem_id();
    int shm_oven_id = get_shm_oven_id();
    int shm_table_id = get_shm_table_id();

    oven* o = shmat(shm_oven_id, NULL, 0);
    table* t = shmat(shm_table_id, NULL, 0);


    int type;

    srand(getpid());
    while(true){

        // preparing
        type = rand() % 10;
        printf("[C]  (pid: %d timestamp: %s)  ->   Przygotowuje pizze: %d\n", getpid(), get_current_time(), type);
        sleep(PREPARATION_TIME);

        lock_sem(sem_id, FULL_OVEN_SEM);

        // placing in oven
        lock_sem(sem_id, OVEN_SEM);
        place_in_oven(o, type);
        printf("[C]  (pid: %d timestamp: %s)  ->   Dodałem pizze: %d. Liczba pizz w piecu: %d.\n", getpid(), get_current_time(), type, o->pizzas);
        unlock_sem(sem_id, OVEN_SEM);


        // baking
        sleep(BAKING_TIME);


        // taking out
        lock_sem(sem_id, OVEN_SEM);
        type = take_out_pizza(o);
        printf("[C]  (pid: %d timestamp: %s)  ->   Wyjalem pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n", getpid(), get_current_time(), type, o->pizzas, t->pizzas);
        unlock_sem(sem_id, OVEN_SEM);

        unlock_sem(sem_id, FULL_OVEN_SEM);

        lock_sem(sem_id, FULL_TABLE_SEM);

        // placing on the table
        lock_sem(sem_id, TABLE_SEM);
        place_on_table(t, type);
        printf("[C]  (pid: %d timestamp: %s)  ->   Umieszczam pizze na stole: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n", getpid(), get_current_time(), type, o->pizzas, t->pizzas);

        unlock_sem(sem_id, TABLE_SEM);

        unlock_sem(sem_id, EMPTY_TABLE_SEM);

    }

}