//
// Created by sylwia on 5/17/21.
//

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define REINDEER_NUM 9
#define ELVES_NUM 10

#define ELF_WORKING_TIME rand()%4+2
#define SANTA_SOLVING_PROBLEM rand()%2+1

#define REINDEER_ON_HOLIDAY rand()%6+5
#define DELIVERING_TOYS rand()%3+2


int reindeer_back = 0;
int elves_with_problem = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;



void* santa(void* arg){
    printf("Santa here\n");
}

void* reindeer(void* arg){
    printf("Reindeer here\n");
    pthread_mutex_lock(&mutex);
    // sth
    pthread_mutex_unlock(&mutex);
}

void* elf(void* arg){
    printf("Elf here\n");
}

int main(){
    srand(time(NULL));


    pthread_mutex_init(&mutex, NULL);

    // Santa thread
    pthread_t santa_t;
    pthread_create(&santa_t, NULL, &santa, NULL);

    // reindeer threads
    pthread_t* reindeer_t = calloc(REINDEER_NUM, sizeof(pthread_t));
    for (int i = 0; i < REINDEER_NUM; i++){
        pthread_create(&reindeer_t[i], NULL, &reindeer, NULL);
    }

    // elves threads
    pthread_t* elves_t = calloc(ELVES_NUM, sizeof(pthread_t));
    for (int i = 0; i < ELVES_NUM; i++){
        pthread_create(&elves_t[i], NULL, &elf, NULL);
    }



    // wait for threads to finish

    pthread_join(santa_t, NULL);

    for (int i = 0; i < REINDEER_NUM; i++){
        pthread_join(reindeer_t[i], NULL);
    }

    for (int i = 0; i < ELVES_NUM; i++){
        pthread_join(elves_t[i], NULL);
    }


    return 0;
}