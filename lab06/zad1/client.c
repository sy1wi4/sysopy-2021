//
// Created by sylwia on 4/23/21.
//

#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>
#include <signal.h>
#include "common.h"
#include <stdlib.h>
#include <string.h>


int server_q;
int client_q;
int to_connect_q;
int id;


void handle_SIGINT(){
    printf("Delete server queue...");
    msgctl(server_q, IPC_RMID, NULL);
    exit(0);
}

void delete_client_q(){
    delete_queue(client_q);
}

void init(){
    message msg = {.q_id = client_q, .type = INIT};
    send_msg(server_q, &msg);
}

void send_LIST(){
    message msg = {.type = LIST};
    send_msg(server_q, &msg);
}

int main(){

    printf("---- CLIENT HERE ----\n");

    signal(SIGINT, handle_SIGINT);
    atexit(delete_client_q);


    // server
    key_t key_s = ftok(get_home_path(), ID);
    if (key_s == -1){
        printf("Error while generating key!\n");
        return -1;
    }

    server_q = msgget(key_s, 0);
    if (server_q == -1){
        printf("Error while creating server queue!\n");
        return -1;
    }

    printf("server queue id: %d\n", server_q);

    // client
    key_t key_c = ftok(get_home_path(), CLIENT_ID);
    if (key_c == -1){
        printf("Error while generating key!\n");
        return -1;
    }

    client_q = msgget(key_c,IPC_CREAT | IPC_EXCL | 0666);
    if (client_q == -1){
        printf("Error while creating client queue!\n");
        return -1;
    }

    printf("client queue id: %d\n", client_q);



    init();

    char buffer[MAX_LEN];
    while((fgets (buffer, MAX_LEN - 2, stdin))){
        printf("line: %s\n", buffer);
        if (strcmp("LIST\n", buffer) == 0){
            printf("send list\n");
            send_LIST();
        }
    }

    msgctl(server_q, IPC_RMID, NULL);
    msgctl(client_q, IPC_RMID, NULL);


    return 0;
}