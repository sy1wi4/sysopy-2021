//
// Created by sylwia on 4/23/21.
//


#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>

#include "common.h"

client clients[MAX_CLIENTS];
int server_q;


void stop_server(){

    // close all opened queues and
    // send stop to all connected clients
    printf("STOPPING SERVER...\n");
    message msg;
    int client_q;
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i].connected) {
            printf("Sending stop to client, id: %d\n", i);
            client_q = clients[i].q_id;
            msg.type = STOP;
            send_msg(client_q, &msg);

            mq_close(clients[i].q_id);
        }
    }

    mq_close(server_q);
    mq_unlink(SERVER_Q);
    printf("\nSERVER STOPPED!\n");
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
    exit(0);
}

void list_clients(message* msg){
    printf("\nserver:  LIST received\n");

    printf("--------------------------------------------");
    printf("\nCLIENTS:\n");
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i].connected){
            printf("id: %d  q_id: %d   status: %s available\n",
                   clients[i].id, clients[i].q_id, clients[i].available ? "" : "not");
        }
    }
    printf("-------------------------------------------\n");

}

void connect_with_client(message* msg){

    printf("\nserver:  CONNECT received\n");

    int q_id_to_connect = clients[msg->to_connect_id].q_id;
    int q_id_sender = clients[msg->sender_id].q_id;

    // send back q_id and q_name of the client to connect to
    message msg_back = {.q_id = q_id_to_connect, .type = CONNECT, .sender_id = msg->to_connect_id};
    strcpy(msg_back.text, clients[msg->to_connect_id].q_name);
    send_msg(q_id_sender, &msg_back);
    printf("q_id of %d send to queue: %d\n", q_id_to_connect, q_id_sender);

    // send q_id and q_name of sender to the client to connect to
    message to_send = {.q_id = q_id_sender, .type = CONNECT, .sender_id = msg->sender_id};
    strcpy(to_send.text, clients[msg->sender_id].q_name);
    send_msg(q_id_to_connect, &to_send);
    printf("q_id of %d send to queue: %d\n", q_id_sender, q_id_to_connect);


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

    // send DISCONNECT to connected client
    message to_send = {.type=DISCONNECT};
    send_msg(clients[connected_id].q_id, &to_send);
}

void stop_sender(message* msg){
    printf("\nserver:  STOP received\n");

    int client_id = msg->sender_id;
    clients[client_id].connected = false;

    mq_close(clients[client_id].q_id);
}

void init_client(message* msg){
    printf("\nserver:  INIT received\n");

    int id = assign_id();
    if (id == -1){
        printf("Server is full!");
        exit(1);
    }
    client new_client = {.connected = true, .available = true, .id = id};
    strcpy(new_client.q_name, msg->text);
    clients[id] = new_client;
    clients[id].q_id = mq_open(new_client.q_name, O_WRONLY);
    if (clients[id].q_id == -1){
        printf("Error while creating client queue!\n");
        exit(1);
    }
    else{
        printf("id: %d, Q_ID ON SEEEERVER: %d\n", id, clients[id].q_id);
    }

    // send back assigned id
    message msg_back = {.type = INIT, .sender_id = id, .sender_pid = getpid()};
    send_msg(clients[id].q_id, &msg_back);
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
    printf("---- SERVER HERE ----\n");

    atexit(stop_server);

    // set up the attribute structure
    struct mq_attr attr;
    attr.mq_maxmsg = MAX_MSG;
    attr.mq_msgsize = sizeof(message);
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;

    // open a queue with the attribute structure
    server_q = mq_open(SERVER_Q,  O_RDONLY | O_CREAT | O_EXCL, 0666, &attr);

    if (server_q == -1){
        printf("Error while creating queue! %s\n", strerror(errno));
        return -1;
    }

    printf("server queue descriptor: %d\n", server_q);


    init_clients();

    signal(SIGINT, handle_SIGINT);

    // receive messages from queue
    message msg;
    unsigned int type;
    while(true){
        receive_msg(server_q, &msg, &type);

        handle_msg(&msg);

    }

}