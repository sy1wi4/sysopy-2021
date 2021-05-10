//
// Created by sylwia on 5/8/21.
//


#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/shm.h>
#include "shared.h"

void place_on_table(table* t, int type){
    t->arr[t->to_place_idx] = type;
    t->to_place_idx++;
    t->to_place_idx = t->to_place_idx % TABLE_SIZE;
    t->pizzas++;
}

int take_pizza(table* t){
    int type = t->arr[t->to_take_out_idx];
    t->arr[t->to_take_out_idx] = -1;
    t->to_take_out_idx++;
    t->to_take_out_idx = t->to_take_out_idx % TABLE_SIZE;
    t->pizzas--;
    return type;
}

int main(){

    int sem_id = get_sem_id();
    int shm_table_id = get_shm_table_id();

    table* t = shmat(shm_table_id, NULL, 0);

    int type;

    while(true) {
        // taking out
        lock_sem(sem_id, EMPTY_TABLE_SEM);

        lock_sem(sem_id, TABLE_SEM);
        type = take_pizza(t);
        printf("[S]  (pid: %d timestamp: %s)  ->   Pobieram pizze: %d. Liczba pizz na stole: %d.\n", getpid(), get_current_time(), type, t->pizzas);
        unlock_sem(sem_id, TABLE_SEM);

        unlock_sem(sem_id, FULL_TABLE_SEM);

        sleep(DELIVERY_TIME);

        // delivery
        printf("[S]  (pid: %d timestamp: %s)  ->   Dostarczam pizze: %d.\n", getpid(), get_current_time(), type);
        sleep(RETURN_TIME);

    }


}