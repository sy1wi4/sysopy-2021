//
// Created by sylwia on 4/23/21.
//


#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>

#include "common.h"

client* clients[MAX_CLIENTS];

char* get_home_path(){
    char* path = getenv("HOME");
    if (path == NULL) {
        path = getpwuid(getuid())->pw_dir;
    }
    return path;
}

void list_clients(message* msg){

}

void connect_with_client(message* msg){

}

void disconnect_sender(message* msg){

}

void stop_sender(message* msg){

}

void init_client(message* msg){

}

void handle_msg(message* msg){
    switch (msg->mtype) {
        case STOP:
            stop_sender(msg);
            break;

        case DISCONNECT:
            disconnect_sender(msg);
            break;

        case LIST:
            list_clients(msg);
            break;

        case CONNECT:
            connect_with_client(msg);
            break;

        case INIT:
            init_client(msg);
            break;

        default:
            printf("Error while handling message!");
            exit(1);
    }
}



int main(){

    // create mew message queue - key to recognize message queue
    // and flag (IPC_CREAT - create queue if not exists, IPC_EXCL, with
    // IPC_CREAT - fails if the queue already exists)

    key_t key = ftok(get_home_path(), ID);
    if (key == -1){
        printf("Error while generating key!");
        return -1;
    }

    int q_id = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
    if (q_id == -1){
        printf("Error while creating queue!");
        return -1;
    }

    printf("queue id: %d\n", q_id);

    // receive messages from queue
    message msg;
    while(true){
        int msgsz = sizeof(message) - sizeof(msg.mtype);
        if (msgrcv(q_id, &msg, msgsz, -INIT-1, 0) == -1){
            printf("Error while receiving messgae!");
            return -1;
        }

        handle_msg(&msg);

    }


    // delete queue (STOP)
    msgctl(q_id, IPC_RMID, NULL);

}