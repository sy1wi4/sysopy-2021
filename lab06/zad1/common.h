//
// Created by sylwia on 4/23/21.
//

#ifndef LAB06_COMMON_H
#define LAB06_COMMON_H

#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>

#define MSG_LEN 128
#define ID 'S'
#define CLIENT_ID getpid()

// mtypes:
#define STOP 1
#define DISCONNECT 2
#define LIST 3
#define CONNECT 4
#define INIT 5

#define MAX_CLIENTS 10
#define MAX_LEN 128

typedef struct {
    long type;    // message's type as specified by the sending process
    char text[MSG_LEN];
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

void send_msg(int q_id, message* msg_back){
    if(msgsnd(q_id, msg_back, sizeof(message) - sizeof(long), 0) == -1){
        printf("Error while sending message!");
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
