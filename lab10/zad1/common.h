//
// Created by sylwia on 5/26/21.
//

#ifndef LAB10_COMMON_H
#define LAB10_COMMON_H


#include <netdb.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>

#define MAX_CLIENTS 10
#define MAX_MSG_LEN 256

typedef struct{
    char* name;
    int fd;
    bool available;
    int opponent_idx;
} client;



#endif //LAB10_COMMON_H
