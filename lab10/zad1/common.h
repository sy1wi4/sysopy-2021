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


#define MAX_MSG_LEN 128
#define MAX_CLIENTS 7

typedef struct client {
    int fd;
    char* name;
} client;


void print_clients(client clients[MAX_CLIENTS]){
    for (int i = 0; i < MAX_CLIENTS; i++){
        printf("%s | ", clients[i].name);
    }
    printf("\n");

    for (int i = 0; i < MAX_CLIENTS; i++){
        printf("%d | ", clients[i].fd);
    }
    printf("\n");
}


#endif //LAB10_COMMON_H
