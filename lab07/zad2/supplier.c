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

    sem_t* table_sem = get_sem(TABLE_SEM);
    sem_t* full_table_sem = get_sem(FULL_TABLE_SEM);
    sem_t* empty_table_sem = get_sem(EMPTY_TABLE_SEM);

    int shm_table_fd = get_shm_table_fd();

    table* t = mmap(NULL, sizeof(table), PROT_READ | PROT_WRITE, MAP_SHARED, shm_table_fd, 0);


    int type;

    while(true) {
        // taking out
        lock_sem(empty_table_sem);

        lock_sem(table_sem);
        type = take_pizza(t);
        printf("[S]  (pid: %d timestamp: %s)  ->   Pobieram pizze: %d. Liczba pizz na stole: %d.\n", getpid(), get_current_time(), type, t->pizzas);
        unlock_sem(table_sem);

        unlock_sem(full_table_sem);

        sleep(DELIVERY_TIME);

        // delivery
        printf("[S]  (pid: %d timestamp: %s)  ->   Dostarczam pizze: %d.\n", getpid(), get_current_time(), type);
        sleep(RETURN_TIME);

    }


}