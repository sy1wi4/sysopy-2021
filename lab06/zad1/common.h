//
// Created by sylwia on 4/23/21.
//

#ifndef LAB06_COMMON_H
#define LAB06_COMMON_H

#define MSG_LEN 128
#define ID 'S'

// mtypes:
#define STOP 1
#define DISCONNECT 2
#define LIST 3
#define CONNECT 4
#define INIT 5

#define MAX_CLIENTS 10

typedef struct {
    long mtype;    // message's type as specified by the sending process
    char mtext[MSG_LEN];
} message;

typedef struct {
    int id;
    int connected_id;
    int q_id;
} client;

#endif //LAB06_COMMON_H
