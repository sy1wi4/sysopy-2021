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
int connected_queue;
int id;
bool connected = false;



void delete_client_q(){
    delete_queue(client_q);
}

void init(){
    message msg = {.q_id = client_q, .type = INIT};
    send_msg(server_q, &msg);

    message received;
    receive_msg(client_q, &received, INIT);
    printf("--- CLIENT ID ---\n%d\n\n", received.sender_id);
    id = received.sender_id;
}

void send_LIST(){
    printf("Client send LIST\n");
    message msg = {.type = LIST};
    send_msg(server_q, &msg);
}

void send_STOP(){
    // send back STOP to server, then server will to stop itself
    printf("Client send STOP\n");
    message msg = {.sender_id = id, .type = STOP};
    send_msg(server_q, &msg);
    msgctl(client_q, IPC_RMID, NULL);
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
    printf("Client received STOP\n");
    // TODO: send back STOP to server
}


void handle_CONNECT(message* msg){
    printf("Client received CONNECT\n");
    connected = true;
    connected_queue = msg->q_id;
    printf("Client [%d] connected with [%d]\n", id, msg->sender_id);

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
    printf("Delete server queue...");
    msgctl(server_q, IPC_RMID, NULL);
    exit(0);
}

bool is_empty(int q){
    struct msqid_ds buf;
    msgctl(q, IPC_STAT, &buf);

    if (buf.msg_qnum != 0) return false;
    else return true;
}

void catcher(){

    while (!is_empty(client_q)){
        message msg;
        receive_msg_nowait(client_q, &msg, 0);

        switch (msg.type)
        {
            case STOP:
                handle_STOP();
                exit(0);
                break;
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
    int id_to_connect;
    char buffer[MAX_LEN];

    while(true){
        if (check_input()) {
            fgets(buffer, MAX_LEN, stdin);

            printf("line: %s\n", buffer);
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