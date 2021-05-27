//
// Created by sylwia on 5/26/21.
//

#include <stdbool.h>
#include <sys/poll.h>
#include <pthread.h>
#include "common.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

client clients[MAX_CLIENTS];

int set_local_socket(char* socket_path){

    // create socket
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sock_fd  == -1){
        printf("Could not create LOCAL socket\n");
        exit(1);
    }
    else{
        printf("LOCAL socket created\n");
    }

    struct sockaddr_un sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_un));
    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, socket_path);

    // unlink the file so the bind will succeed
    unlink(socket_path);

    // assigns the address to the socket
    if ((bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr))) == -1){
        printf("Could not bind LOCAL socket\n");
        exit(1);
    }

    // listen for connections on a socket
    if ((listen(sock_fd, MAX_CLIENTS)) == -1){
        printf("Listen on LOCAL socket failed\n");
        exit(1);
    }

    printf("LOCAL socket fd: %d\n", sock_fd);

    return sock_fd;
}


int set_inet_socket(int port_number){

    // create socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd  == -1){
        printf("Could not create LOCAL socket\n");
        exit(1);
    }
    else{
        printf("INET socket created\n");
    }

    // set port and IP
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port_number);
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // bind to the set port and IP
    if ((bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr))) == -1){
        printf("Could not bind INET socket\n");
        exit(1);
    }

    printf("INET socket fd: %d\n", sock_fd);

    return sock_fd;
}

int wait_for_messages(int local_socket, int inet_socket) {
    // monitor both sockets at once

    // set of file descriptors to be monitored
    struct pollfd fds[2 + MAX_CLIENTS];
    fds[0].fd = local_socket;
    fds[1].fd = inet_socket;
    fds[0].events = POLLIN;
    fds[1].events = POLLIN;
    fds[0].revents = 0;
    fds[1].revents = 0;
    pthread_mutex_lock(&mutex);


    for (int i = 0; i < MAX_CLIENTS; i++) {
        fds[i + 2].fd = clients[i].fd;
        fds[i + 2].events = POLLIN;
        fds[i + 2].revents = 0;
    }

    pthread_mutex_unlock(&mutex);
    if ((poll(fds, MAX_CLIENTS + 2, -1)) == -1) {
        printf("Poll error\n");
        exit(1);
    }

    int fd = 0;

    for (int i = 0; i < MAX_CLIENTS + 2; i++) {
        // sth to read
        if (fds[i].revents & POLLIN) {
            fd = fds[i].fd;
            if(fds[i].fd == local_socket || fds[i].fd == inet_socket){
                printf("NEW MSG, fd: %d\n", fd);
                fd = accept(fd, NULL, NULL);
            }
            break;
        }
    }

    return fd;
}


int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Wrong number of arguments!\n");
        exit(1);
    }

    int port_number = atoi(argv[1]);
    char* socket_path = argv[2];

    int local_socket = set_local_socket(socket_path);
    int inet_socket = set_inet_socket(port_number);

    while(true){
        int client_fd = wait_for_messages(local_socket, inet_socket);
        char buffer[NAME_LEN + 5];
        recv(client_fd, buffer, NAME_LEN + 5, 0);
        if (client_fd) {
            printf("%s\n", buffer);
        }
    }

}