//
// Created by sylwia on 5/8/21.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shared.h"

int main(int argc, char* argv[]){

    if (argc != 3){
        printf("Wrong number of arguments!\n");
        return -1;
    }

    // generate unique key
    key_t key = ftok(get_home_path(), ID);

    if (key == -1){
        printf("Error while generating key!\n");
        return -1;
    }

    // allocate a System V shared memory segment
    int shm_id = shmget(key, SH_M_SIZE, IPC_CREAT | IPC_EXCL | 0666);

    // ...

    return 0;
}

