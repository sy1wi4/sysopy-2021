//
// Created by sylwia on 5/8/21.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include "shared.h"

int shm_oven_id, shm_table_id, sem_id;

void set_oven(oven* o){
    for (int i = 0; i < OVEN_SIZE; i++){
        o->arr[i] = -1;
    }
    o->pizzas = 0;
    o->to_place_idx = 0;
    o->to_take_out_idx = 0;
}

void set_table(table* t){
    for (int i = 0; i < OVEN_SIZE; i++){
        t->arr[i] = -1;
    }
    t->pizzas = 0;
    t->to_place_idx = 0;
    t->to_take_out_idx = 0;
}

void create_sh_m_segment(){
    key_t key_o, key_t;

    if ((key_o = ftok(OVEN_PATH, OVEN_ID)) == -1){
        printf("Error while generating key [oven]!\n");
        printf("%s", strerror(errno));
        exit(1);
    }

    if ((key_t = ftok(TABLE_PATH, TABLE_ID)) == -1){
        printf("Error while generating key [table]!\n");
        printf("%s", strerror(errno));
        exit(1);
    }

    printf("o: %d, t: %d\n", key_o, key_t);
    if ((shm_oven_id = shmget(key_o, sizeof(oven), IPC_CREAT | 0666)) == -1){
        printf("Error while creating shared memory segment [oven]!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    if ((shm_table_id = shmget(key_t, sizeof(table), IPC_CREAT | 0666)) == -1){
        printf("Error while creating shared memory segment [table]!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    oven* o = shmat(shm_oven_id, NULL, 0);
    table* t = shmat(shm_table_id, NULL, 0);

    set_oven(o);
    set_table(t);

    printf("OK, shared memory segment created\noven_id: %d, table_id: %d \n\n", shm_oven_id, shm_table_id);

}

void create_semaphores(){
    key_t key;

    if ((key = ftok(get_home_path(), ID)) == -1){
        printf("Error while generating key!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }


    if((sem_id = semget(key, 5, 0666 | IPC_CREAT)) == -1){
        printf("Error while creating semaphore set!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    union arg;
    arg.val = 1;

    if (semctl(sem_id, OVEN_SEM, SETVAL, arg) == -1){
        printf("Error while setting oven semaphore value!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    if (semctl(sem_id, TABLE_SEM, SETVAL,arg) == -1){
        printf("Error while setting table semaphore value!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    arg.val = OVEN_SIZE;
    if (semctl(sem_id, FULL_OVEN_SEM, SETVAL,arg) == -1){
        printf("Error while setting table semaphore value!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    arg.val = TABLE_SIZE;
    if (semctl(sem_id, FULL_TABLE_SEM, SETVAL,arg) == -1){
        printf("Error while setting table semaphore value!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    arg.val = 0;
    if (semctl(sem_id, EMPTY_TABLE_SEM, SETVAL,arg) == -1){
        printf("Error while setting table semaphore value!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    printf("OK, semaphore set created\nsem_id: %d\n\n", sem_id);
}



int main(int argc, char* argv[]){

    if (argc != 3){
        printf("Wrong number of arguments!\n");
        return -1;
    }

    int cooks = atoi(argv[1]);
    int suppliers = atoi(argv[2]);

    create_sh_m_segment();
    create_sem_set();

    for (int i = 0; i < cooks; i++){
        pid_t pid = fork();
        if (pid == 0){
            execl("./cook", "./cook", NULL);
            printf("!!! Return not expected, execl() error !!!\n");

        }
    }

    for (int i = 0; i < suppliers; i++){
        pid_t pid = fork();
        if (pid == 0){
            execl("./supplier", "./supplier", NULL);
            printf("!!! Return not expected, execl() error !!!\n");

        }
    }

    for(int i = 0; i < cooks + suppliers; i++)
        wait(NULL);

    //clear
    semctl(sem_id, 0, IPC_RMID, NULL);
    shmctl(shm_oven_id, IPC_RMID, NULL);
    shmctl(shm_table_id, IPC_RMID, NULL);


    return 0;
}

