//
// Created by sylwia on 5/8/21.
//


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
        // if empty_table_sem is 0, it blocks supplier process
        // so decrement value before taking pizza out from table
        lock_sem(sem_id, EMPTY_TABLE_SEM);

        // taking out
        lock_sem(sem_id, TABLE_SEM);
        type = take_pizza(t);
        printf("[S]  (pid: %d timestamp: %s)  ->   Pobieram pizze: %d. Liczba pizz na stole: %d.\n", getpid(), get_current_time(), type, t->pizzas);
        unlock_sem(sem_id, TABLE_SEM);

        // if full_table_sem is 0, it blocks cook process
        // because pizza was taken out of the table, let's increment its value - pizza can be placed by cook
        unlock_sem(sem_id, FULL_TABLE_SEM);

        sleep(DELIVERY_TIME);

        // delivery
        printf("[S]  (pid: %d timestamp: %s)  ->   Dostarczam pizze: %d.\n", getpid(), get_current_time(), type);
        sleep(RETURN_TIME);

    }


}