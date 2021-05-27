//
// Created by sylwia on 5/26/21.
//

#ifndef LAB10_COMMON_H
#define LAB10_COMMON_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define NAME_LEN 32
#define MAX_CLIENTS 15

typedef struct client {
    int fd;
} client;

#endif //LAB10_COMMON_H
