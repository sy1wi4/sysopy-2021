//
// Created by sylwia on 5/17/21.
//

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>


#define REINDEER_NUM 9
#define ELVES_NUM 10

#define ELF_WORKING_TIME rand()%4+2
#define SANTA_SOLVING_PROBLEM rand()%2+1

#define REINDEER_ON_HOLIDAY rand()%6+2
#define DELIVERING_TOYS rand()%3+1


int reindeer_back = 0;
int elves_with_problem = 0;
int deliveries = 0;
int reindeer_waiting = false;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex_r_wait = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_r_wait   = PTHREAD_COND_INITIALIZER;



void* santa(void* arg){

    // waiting for 9 reindeer
    while (deliveries < 3){
        pthread_mutex_lock(&mutex);

        if (reindeer_back < REINDEER_NUM){
            pthread_cond_wait(&cond, &mutex);
        }

        printf("Mikołaj: budzę się\n");

        deliveries++;
        printf("Mikołaj: dostarczam zabawki [delivery no: %d]\n", deliveries);
        sleep(DELIVERING_TOYS);
        reindeer_back = 0;

        // notify waiting reindeer that other reindeer are no longer waiting for santa
        pthread_mutex_lock(&mutex_r_wait);
        reindeer_waiting = false;
        pthread_cond_broadcast(&cond_r_wait);
        pthread_mutex_unlock(&mutex_r_wait);

        printf("Mikołaj: zasypiam\n\n\n");
        pthread_mutex_unlock(&mutex);
    }
    exit(0);
}

_Noreturn void* reindeer(void* arg){
    int ID = *((int *) arg);

    while(true){

        // if reindeer are waiting for Santa wait here
        pthread_mutex_lock(&mutex_r_wait);
        while (reindeer_waiting) {
            pthread_cond_wait(&cond_r_wait, &mutex_r_wait);
        }
        pthread_mutex_unlock(&mutex_r_wait);

        sleep(REINDEER_ON_HOLIDAY);


        pthread_mutex_lock(&mutex);
        reindeer_back++;
        printf("Renifer: czeka [%d] reniferów na Mikołaja, %d\n", reindeer_back, ID);
        reindeer_waiting = true;

        if (reindeer_back == REINDEER_NUM) {
            printf("Renifer: wybudzam Mikołaja, %d\n", ID);
            pthread_cond_broadcast(&cond);
        sleep(DELIVERING_TOYS);
        }
        pthread_mutex_unlock(&mutex);
    }
}

void* elf(void* arg){
    int ID = *((int *) arg);
    printf("Elf %d here \n", ID);
}

int main(){
    srand(time(NULL));


    pthread_mutex_init(&mutex, NULL);

    // Santa thread
    pthread_t santa_t;
    pthread_create(&santa_t, NULL, &santa, NULL);

    // reindeer threads
    int* reindeer_IDs = calloc(REINDEER_NUM, sizeof(int));
    pthread_t* reindeer_t = calloc(REINDEER_NUM, sizeof(pthread_t));
    for (int i = 0; i < REINDEER_NUM; i++){
        reindeer_IDs[i] = i;
        pthread_create(&reindeer_t[i], NULL, &reindeer, &reindeer_IDs[i]);
    }

//    // elves threads
//    int* elves_IDs = calloc(ELVES_NUM, sizeof(int));
//    pthread_t* elves_t = calloc(ELVES_NUM, sizeof(pthread_t));
//    for (int i = 0; i < ELVES_NUM; i++){
//        elves_IDs[i] = i;
//        pthread_create(&elves_t[i], NULL, &elf, elves_IDs[i]);
//    }

    // wait for threads to finish

    pthread_join(santa_t, NULL);

    for (int i = 0; i < REINDEER_NUM; i++){
        pthread_join(reindeer_t[i], NULL);
    }

//    for (int i = 0; i < ELVES_NUM; i++){
//        pthread_join(elves_t[i], NULL);
//    }


    return 0;
}