//
// Created by sylwia on 4/23/21.
//

#ifndef LAB06_COMMON_H
#define LAB06_COMMON_H

#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>


#define ID 'S'
#define CLIENT_ID getpid()

// mtypes:
#define STOP 1
#define DISCONNECT 2
#define LIST 3
#define CONNECT 4
#define INIT 5
#define CHAT 6

#define MAX_CLIENTS 10
#define MAX_LEN 128

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
} client;

void send_msg(int q_id, message* msg){
    if(msgsnd(q_id, msg, sizeof(message) - sizeof(long), 0) == -1){
        printf("Error while sending message!\n");
        exit(1);
    }
}

// if no message of the requested type is available and IPC_NOWAIT isn't
// specified in msgflg, the calling process is blocked
void receive_msg(int q_id, message* msg, long type){
    if(msgrcv(q_id, msg, sizeof(message) - sizeof(long), type,0) == -1){
        printf("Error while receiving message!\n");
        exit(1);
    }
}

void receive_msg_nowait(int q_id, message* msg, long type){
    if(msgrcv(q_id, msg, sizeof(message) - sizeof(long), type,IPC_NOWAIT) == -1){
        printf("Error while receiving message (NOWAIT)!\n");
        exit(1);
    }
}

char* get_home_path(){
    char* path = getenv("HOME");
    if (path == NULL) {
        path = getpwuid(getuid())->pw_dir;
    }
    return path;
}

void delete_queue(int q) {
    msgctl(q, IPC_RMID, NULL);
}

#endif //LAB06_COMMON_H
