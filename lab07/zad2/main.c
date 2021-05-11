//
// Created by sylwia on 5/8/21.
//


#include "shared.h"

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
    int shm_oven_fd,shm_table_fd;

    if ((shm_oven_fd = shm_open(OVEN_SHM, O_RDWR | O_CREAT, 0666))== -1){
        printf("Error while creating shared memory segment [oven]!\n");
        exit(1);
    }
    if ((shm_table_fd = shm_open(TABLE_SHM, O_RDWR | O_CREAT, 0666)) == -1){
        printf("Error while creating shared memory segment [table]!\n");
        exit(1);
    };


    // set size of shm segment
    if (ftruncate(shm_oven_fd, sizeof(oven)) == -1){
        printf("ftruncate() error  [oven]\n");
        exit(1);
    }

    if (ftruncate(shm_table_fd, sizeof(table)) == -1){
        printf("ftruncate() error  [table]\n");
        exit(1);
    }

    // get address of attached segment
    oven* o = mmap(NULL, sizeof(oven), PROT_READ | PROT_WRITE, MAP_SHARED, shm_oven_fd, 0);
    table* t = mmap(NULL, sizeof(table), PROT_READ | PROT_WRITE, MAP_SHARED, shm_table_fd, 0);

    if (o == (void*) -1){
        printf("Error while attaching shm segment [oven]\n");
        exit(1);
    }

    if (t == (void*) -1){
        printf("Error while attaching shm segment [table]\n");
        exit(1);
    }

    set_oven(o);
    set_table(t);

    printf("OK, shared memory segment created.\noven_fd: %d, table_fd: %d\n", shm_oven_fd, shm_table_fd);

}

void create_semaphores(){
    sem_t* oven_sem = sem_open(OVEN_SEM, O_CREAT, 0666, 1);
    if (oven_sem == SEM_FAILED){
        printf("Error while creating [oven_sem] semaphore!\n");
        exit(1);
    }

    sem_t* table_sem = sem_open(TABLE_SEM, O_CREAT, 0666, 1);
    if (table_sem == SEM_FAILED){
        printf("Error while creating [table_sem] semaphore!\n");
        exit(1);
    }

    // if oven is full, block cook process - cannot place pizza
    sem_t* full_oven_sem = sem_open(FULL_OVEN_SEM, O_CREAT, 0666, OVEN_SIZE);
    if (full_oven_sem == SEM_FAILED){
        printf("Error while creating [full_oven_sem] semaphore!\n");
        exit(1);
    }

    // if table is full, block cook process - cannot place pizza
    sem_t* full_table_sem = sem_open(FULL_TABLE_SEM, O_CREAT, 0666, TABLE_SIZE);
    if (full_table_sem == SEM_FAILED){
        printf("Error while creating [full_table_sem] semaphore!\n");
        exit(1);
    }

    // if table is empty, block supplier process - cannot take out pizza
    sem_t* empty_table_sem = sem_open(EMPTY_TABLE_SEM, O_CREAT, 0666, 0);
    if (empty_table_sem == SEM_FAILED){
        printf("Error while creating [empty_table_sem] semaphore!\n");
        exit(1);
    }

    printf("OK, semaphores created.\n\n");
}

void handler(int signum){
    sem_unlink(OVEN_SEM);
    sem_unlink(TABLE_SEM);
    sem_unlink(FULL_OVEN_SEM);
    sem_unlink(FULL_TABLE_SEM);
    sem_unlink(EMPTY_TABLE_SEM);
    shm_unlink(TABLE_SHM);
    shm_unlink(OVEN_SHM);
}

int main(int argc, char* argv[]){

    if (argc != 3){
        printf("Wrong number of arguments!\n");
        return -1;
    }

    signal(SIGINT, handler);

    int cooks = atoi(argv[1]);
    int suppliers = atoi(argv[2]);

    create_sh_m_segment();
    create_semaphores();

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

    return 0;
}

