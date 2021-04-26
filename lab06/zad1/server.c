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
#include <signal.h>

#include "common.h"

client clients[MAX_CLIENTS];
int server_q;

void delete_server_q(){
    delete_queue(server_q);
}

int assign_id(){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i].connected == false){
            printf("id assigned: %d\n", i);
            return i;
        }
    }
    return -1;
}


// CTRL + C  ->  stop server
void handle_SIGINT(){
    printf("\nserver:  SIGINT received\n");
    printf("Delete server queue...");
    msgctl(server_q, IPC_RMID, NULL);
    exit(0);
}

void list_clients(message* msg){
    printf("\nserver:  LIST received\n");

    printf("--------------------------------------------");
    printf("\nCLIENTS:\n");
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i].connected){
            printf("id: %d   q_id: %d   status: %s available\n", clients[i].id,
                   clients->q_id, clients[i].available ? "" : "not");
        }
    }
    printf("-------------------------------------------\n");

}

void connect_with_client(message* msg){

    printf("\nserver:  CONNECT received\n");

    int q_id_to_connect = clients[msg->to_connect_id].q_id;
    int q_id_sender = clients[msg->sender_id].q_id;

    // send back q_id of the client to connect to
    message msg_back = {.q_id = q_id_to_connect, .type = CONNECT};
    send_msg(q_id_sender, &msg_back);

    // send q_id of sender to the client to connect to
    message to_send = {.q_id = q_id_sender, .type = CONNECT};
    send_msg(q_id_to_connect, &to_send);

    printf("Connect with %d\n",q_id_to_connect);
    printf("Connect with %d\n",q_id_sender);

    clients[msg->to_connect_id].connected_id = msg->sender_id;
    clients[msg->sender_id].connected_id = msg->to_connect_id;

    clients[msg->to_connect_id].available = false;
    clients[msg->sender_id].available = false;

}

void disconnect_sender(message* msg){
    printf("\nserver:  DISCONNECT received\n");

    int sender_id = msg->sender_id;
    int connected_id = clients[sender_id].connected_id;

    clients[sender_id].available = true;
    clients[connected_id].available = true;

    // send disconnect to other client
}

void stop_sender(message* msg){
    printf("\nserver:  STOP received\n");

    int client_id = msg->sender_id;
    clients[client_id].connected = false;
}

void init_client(message* msg){
    printf("\nserver:  INIT received\n");

    int q_id = msg->q_id;
    int id = assign_id();
    if (id == -1){
        printf("Server is full!");
        exit(1);
    }
    client new_client = {.q_id = q_id, .connected = true, .available = true, .id = id};
    clients[id] = new_client;

    message msg_back = {.type = INIT, .sender_id = id, .sender_pid = getpid()};
    send_msg(q_id, &msg_back);
    printf("message sent to %d\n", q_id);
}


void handle_msg(message* msg){
    switch (msg->type) {
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

void init_clients(){
    printf("init clients\n");
    for(int i = 0; i < MAX_CLIENTS; i++){
        clients[i].available = true;
        clients[i].connected = false;
    }
}

int main(){
    // create mew message queue - key to recognize message queue
    // and flag (IPC_CREAT - create queue if not exists, IPC_EXCL, with
    // IPC_CREAT - fails if the queue already exists)
    printf("---- SERVER HERE ----\n");
    atexit(delete_server_q);

    key_t key = ftok(get_home_path(), ID);
    if (key == -1){
        printf("Error while generating key!\n");
        return -1;
    }

    server_q = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
    if (server_q == -1){
        printf("Error while creating queue!\n");
        return -1;
    }

    printf("server queue id: %d\n", server_q);

    init_clients();

    signal(SIGINT, handle_SIGINT);

    // receive messages from queue
    message msg;
    while(true){
        int msgsz = sizeof(message) - sizeof(msg.type);
        if (msgrcv(server_q, &msg, msgsz, -INIT-1, 0) == -1){
            printf("Error while receiving message!\n");
            return -1;
        }

        handle_msg(&msg);

    }


    msgctl(server_q, IPC_RMID, NULL);

}