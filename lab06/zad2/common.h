//
// Created by sylwia on 4/23/21.
//

#ifndef LAB06_COMMON_H
#define LAB06_COMMON_H

#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>
#include <mqueue.h>
#include <sys/msg.h>

// mtypes:
#define STOP 1
#define DISCONNECT 2
#define LIST 3
#define CONNECT 4
#define INIT 5
#define CHAT 6

#define MAX_CLIENTS 10
#define MAX_LEN 128
#define MAX_MSG 10

#define SERVER_Q "/server_q"


typedef struct {
    long type;    // message's type as specified by the sending process
    char text[MAX_LEN];
    pid_t sender_pid;
    int q_id;
    int sender_id;
    int to_connect_id;
} message;


typedef struct {
    int id;
    int q_id;
    int connected_id;
    bool connected;
    bool available;   // for connection
    char q_name[MAX_LEN];
} client;


void send_msg(int q_id, message* msg){
    if (mq_send(q_id, (char *) msg, sizeof(message), msg->type) == -1){
        printf("Error while sending message!\n");
        exit(1);
    }
}


void receive_msg(int q_id, message* msg, unsigned int* type){
    if(mq_receive(q_id, (char *) msg, sizeof(message), type) == -1){
        printf("Error while receiving message!\n");
        exit(1);
    }
}



char* get_client_q_name(){
    char* name = calloc(MAX_LEN, sizeof(char));
    sprintf(name, "/client_q_%d", getpid());
    return name;
}

#endif //LAB06_COMMON_H
