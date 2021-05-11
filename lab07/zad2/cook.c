//
// Created by sylwia on 5/8/21.
//

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

    sem_t* oven_sem = get_sem(OVEN_SEM);
    sem_t* table_sem = get_sem(TABLE_SEM);
    sem_t* full_oven_sem = get_sem(FULL_OVEN_SEM);
    sem_t* full_table_sem = get_sem(FULL_TABLE_SEM);
    sem_t* empty_table_sem = get_sem(EMPTY_TABLE_SEM);

    int shm_oven_fd = get_shm_oven_fd();
    int shm_table_fd = get_shm_table_fd();

    oven* o = mmap(NULL, sizeof(oven), PROT_READ | PROT_WRITE, MAP_SHARED, shm_oven_fd, 0);
    table* t = mmap(NULL, sizeof(table), PROT_READ | PROT_WRITE, MAP_SHARED, shm_table_fd, 0);

    int type;

    srand(getpid());
    while(true){

        // preparing
        type = rand() % 10;
        printf("[C]  (pid: %d timestamp: %s)  ->   Przygotowuje pizze: %d\n", getpid(), get_current_time(), type);
        sleep(PREPARATION_TIME);

        // if full_oven_sem is 0, it blocks cook process
        // so decrement value before placing in oven
        lock_sem(full_oven_sem);

        // placing in oven
        lock_sem(oven_sem);
        place_in_oven(o, type);
        printf("[C]  (pid: %d timestamp: %s)  ->   Dodałem pizze: %d. Liczba pizz w piecu: %d.\n", getpid(), get_current_time(), type, o->pizzas);
        unlock_sem(oven_sem);


        // baking
        sleep(BAKING_TIME);


        // taking out
        lock_sem(oven_sem);
        type = take_out_pizza(o);
        printf("[C]  (pid: %d timestamp: %s)  ->   Wyjalem pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n", getpid(), get_current_time(), type, o->pizzas, t->pizzas);
        unlock_sem(oven_sem);

        // pizza was taken out, so increment full_oven_sem
        unlock_sem(full_oven_sem);

        // if full_table_sem is 0, it blocks cook process
        // so decrement value before placing on the table
        lock_sem(full_table_sem);

        // placing on the table
        lock_sem(table_sem);
        place_on_table(t, type);
        printf("[C]  (pid: %d timestamp: %s)  ->   Umieszczam pizze na stole: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n", getpid(), get_current_time(), type, o->pizzas, t->pizzas);
        unlock_sem(table_sem);

        // if empty_table_sem is 0, it blocks supplier process
        // because pizza was placed on the table, let's increment its value - pizza can be taken by supplier
        unlock_sem(empty_table_sem);

    }

}