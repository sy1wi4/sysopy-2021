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
int deliveries = 0;
bool reindeer_waiting = false;

int elves_with_problem = 0;
int elves_queue[3];
bool elves_queue_full = false;

pthread_mutex_t mutex_r = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_r   = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex_r_wait = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_r_wait   = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex_e = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_e   = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex_e_wait = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_e_wait   = PTHREAD_COND_INITIALIZER;

void reset_queue(int* queue){
    for (int i = 0; i < 3; i++){
        queue[i] = -1;
    }
}

void* santa(void* arg){

    // waiting for 3 elves
    while(true){
        pthread_mutex_lock(&mutex_e);

        while (elves_with_problem < 3){
            pthread_cond_wait(&cond_e, &mutex_e);
        }
        printf("Mikołaj: rozwiązuje problemy elfów [%d][%d][%d]\n", elves_queue[0], elves_queue[1], elves_queue[2]);
        sleep(SANTA_SOLVING_PROBLEM);

        // notify waiting elves that elves_queue is empty
        pthread_mutex_lock(&mutex_e_wait);
        reset_queue(elves_queue);
        elves_queue_full = false;
        elves_with_problem = 0;
        pthread_cond_broadcast(&cond_e_wait);
        pthread_mutex_unlock(&mutex_e_wait);

        printf("Mikołaj: zasypiam\n\n");
        pthread_mutex_unlock(&mutex_e);
    }



//    // waiting for 9 reindeer
//    while (deliveries < 3){
//        pthread_mutex_lock(&mutex_r);
//
//        if (reindeer_back < REINDEER_NUM){
//            pthread_cond_wait(&cond_r, &mutex_r);
//        }
//
//        printf("Mikołaj: budzę się\n");
//
//        deliveries++;
//        printf("Mikołaj: dostarczam zabawki [delivery no: %d]\n", deliveries);
//        sleep(DELIVERING_TOYS);
//        reindeer_back = 0;
//
//        // notify waiting reindeer that other reindeer are no longer waiting for santa
//        pthread_mutex_lock(&mutex_r_wait);
//        reindeer_waiting = false;
//        pthread_cond_broadcast(&cond_r_wait);
//        pthread_mutex_unlock(&mutex_r_wait);
//
//        printf("Mikołaj: zasypiam\n\n\n");
//        pthread_mutex_unlock(&mutex_r);
//    }
//    exit(0);
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


        pthread_mutex_lock(&mutex_r);
        reindeer_back++;
        printf("Renifer: czeka [%d] reniferów na Mikołaja, %d\n", reindeer_back, ID);
        reindeer_waiting = true;

        if (reindeer_back == REINDEER_NUM) {
            printf("Renifer: wybudzam Mikołaja, %d\n", ID);
            pthread_cond_broadcast(&cond_r);
        }
        pthread_mutex_unlock(&mutex_r);
    }
}

void* elf(void* arg){
    int ID = *((int *) arg);

    while(true){

        sleep(ELF_WORKING_TIME);

        // wait while elves_queue is empty
        pthread_mutex_lock(&mutex_e_wait);
        while (elves_queue_full) {
            pthread_cond_wait(&cond_e_wait, &mutex_e_wait);
            printf("Elf: czeka na powrót elfów, ID: %d\n", ID);
        }
        pthread_mutex_unlock(&mutex_e_wait);



        pthread_mutex_lock(&mutex_e);

        if (elves_with_problem < 3){
            printf("Elf: czeka [%d] elfów na Mikołaja, ID: %d\n", elves_with_problem, ID);
            elves_queue[elves_with_problem] = ID;
            elves_with_problem++;
            if (elves_with_problem == 3){
                printf("Elf: wybudzam Mikołaja, ID: %d\n", ID);
                elves_queue_full = true;
                pthread_cond_broadcast(&cond_e);
            }
        }

        pthread_mutex_unlock(&mutex_e);
    }
}

int main(){
    srand(time(NULL));

    reset_queue(elves_queue);

    // Santa thread
    pthread_t santa_t;
    pthread_create(&santa_t, NULL, &santa, NULL);

    // reindeer threads
//    int* reindeer_IDs = calloc(REINDEER_NUM, sizeof(int));
//    pthread_t* reindeer_t = calloc(REINDEER_NUM, sizeof(pthread_t));
//    for (int i = 0; i < REINDEER_NUM; i++){
//        reindeer_IDs[i] = i;
//        pthread_create(&reindeer_t[i], NULL, &reindeer, &reindeer_IDs[i]);
//    }

    // elves threads
    int* elves_IDs = calloc(ELVES_NUM, sizeof(int));
    pthread_t* elves_t = calloc(ELVES_NUM, sizeof(pthread_t));
    for (int i = 0; i < ELVES_NUM; i++){
        elves_IDs[i] = i;
        pthread_create(&elves_t[i], NULL, &elf, &elves_IDs[i]);
    }

    // wait for threads to finish

    pthread_join(santa_t, NULL);

//    for (int i = 0; i < REINDEER_NUM; i++){
//        pthread_join(reindeer_t[i], NULL);
//    }

    for (int i = 0; i < ELVES_NUM; i++){
        pthread_join(elves_t[i], NULL);
    }


    return 0;
}