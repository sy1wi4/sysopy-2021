//
// Created by sylwia on 4/23/21.
//

#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include "common.h"
#include <stdlib.h>
#include <string.h>


int server_q;
int client_q;
int connected_queue;
int id;
bool connected = false;
char* connected_q_name;



void init(){
    message msg = {.q_id = client_q, .type = INIT};
    strcpy(msg.text, get_client_q_name());
    send_msg(server_q, &msg);
}

void send_LIST(){
    printf("Client send LIST\n");
    message msg = {.type = LIST};
    send_msg(server_q, &msg);
}

void send_STOP(){
    // send STOP to server, then server will close client_q,
    // then close and delete client_q here

    printf("Client send STOP\n");
    message msg = {.sender_id = id, .type = STOP};
    send_msg(server_q, &msg);

    mq_close(server_q);
    mq_close(client_q);
    mq_unlink(get_client_q_name());
    exit(0);
}

void send_CONNECT(int id_to_connect){
    printf("Client send CONNECT\n");
    message msg = {.sender_id = id, .to_connect_id = id_to_connect, .type = CONNECT};
    send_msg(server_q, &msg);
    connected = true;
}

void send_DISCONNECT(){
    printf("Client send DISCONNECT\n");
    message msg = {.sender_id = id, .type = DISCONNECT};
    send_msg(server_q, &msg);
    connected = false;
}


void send_CHAT_msg(){
    if (!connected){
        printf("You are not connected, stupid!\n");
        return;
    }

    printf("Enter msg:\n");
    message msg = {.sender_id = id, .type = CHAT};
    fgets(msg.text, MAX_LEN, stdin);

    send_msg(connected_queue, &msg);
    printf("SENT!\n");
}


void handle_STOP(){
    // close and remove client_q
    // close server_q

    printf("Client received STOP\n");
    mq_close(client_q);
    mq_unlink(get_client_q_name());
    mq_close(server_q);
    exit(0);
}


void handle_INIT(message* msg){
    // sender_id is id assigned on server
    printf("Client received INIT\n");
    id = msg->sender_id;
    printf("Assigned id: %d\n", id);
}


void handle_CONNECT(message* msg){
    printf("Client received CONNECT\n");
    connected = true;
    connected_q_name = msg->text;
    // open connected_q
    connected_queue = mq_open(connected_q_name, O_WRONLY);
    if (connected_queue == -1){
        printf("Error while opening connected client queue\n");
        exit(-1);
    }
    printf("Client [%d  %s] connected with [%d  %s]\n",
           client_q, get_client_q_name(), connected_queue, connected_q_name);

    printf("__________CHAT__________\n");
}


void handle_DISCONNECT(message* msg){
    printf("Client received DISCONNECT\n");
    connected = false;
}


void handle_CHAT_msg(message* msg){
    printf("NEW MSG:\n");
    printf("%s\n", msg->text);
}


void handle_SIGINT(){
    send_STOP();
}

bool is_empty(int q){
    struct mq_attr attr;
    mq_getattr(q, &attr);
    if (attr.mq_curmsgs == 0) return true;
    else return false;
}


void catcher(){

    while (!is_empty(client_q)){
        message msg;
        unsigned int type;
        receive_msg(client_q, &msg, &type);

        switch (msg.type)
        {
            case INIT:
                handle_INIT(&msg);
                break;
            case STOP:
                handle_STOP();
                exit(0);
            case CONNECT:
                handle_CONNECT(&msg);
                break;
            case DISCONNECT:
                handle_DISCONNECT(&msg);
                break;
            case CHAT:
                handle_CHAT_msg(&msg);
                break;
            default:
                break;
        }
    }
}

// check if there is any input available
bool check_input(){
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return (FD_ISSET(0, &fds));
}


int main(){

    printf("---- CLIENT HERE ----\n");

    signal(SIGINT, handle_SIGINT);

    // server
    server_q = mq_open(SERVER_Q, O_WRONLY);
    printf("server queue id: %d\n", server_q);

    // client
    struct mq_attr attr;
    attr.mq_maxmsg = MAX_MSG;
    attr.mq_msgsize = sizeof(message);
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;

    // set O_NONBLOCK so that mq_receive() call does not block process
    client_q = mq_open(get_client_q_name(),O_RDONLY | O_CREAT | O_EXCL | O_NONBLOCK, 0666, &attr);
    char* name = get_client_q_name();
    printf("client queue name: %s\n", name);
    printf("client queue id: %d\n", client_q);


    init();
    int id_to_connect;
    char buffer[MAX_LEN];

    while(true){
        if (check_input()) {
            fgets(buffer, MAX_LEN, stdin);

            if (strcmp("LIST\n", buffer) == 0) {
                send_LIST();
            } else if (strcmp("STOP\n", buffer) == 0) {
                send_STOP();
                exit(0);
            } else if (strcmp("CONNECT\n", buffer) == 0) {
                printf("Enter client ID: ");
                fgets(buffer, MAX_LEN, stdin);
                id_to_connect = atoi(buffer);
                if (id_to_connect == id) {
                    printf("You cannot connect with yourself!\n");
                    continue;
                }
                send_CONNECT(id_to_connect);
            } else if (strcmp("DISCONNECT\n", buffer) == 0) {
                if (!connected) {
                    printf("You are not connected!\n");
                    continue;
                }
                send_DISCONNECT();
            } else if (strcmp("CHAT\n", buffer) == 0){
                if (!connected) {
                    printf("You are not connected!\n");
                    continue;
                }
                send_CHAT_msg();
            }

            else {
                printf("Unknown command: %s\n", buffer);
            }
        }

        sleep(1);
        catcher();
    }

}