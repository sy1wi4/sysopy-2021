//
// Created by sylwia on 5/26/21.
//

#include <stdbool.h>
#include <sys/poll.h>
#include <pthread.h>
#include "common.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

client clients[MAX_CLIENTS];
int idx_to_add = 0;
char* command;
char* arg;


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

    // listen for connections on a socket
    if ((listen(sock_fd, MAX_CLIENTS)) == -1){
        printf("Listen on INET socket failed\n");
        exit(1);
    }

    printf("INET socket fd: %d\n", sock_fd);

    return sock_fd;
}

int wait_for_messages(int local_socket, int inet_socket) {
    // monitor both sockets at once

    // set of file descriptors to be monitored
//    struct pollfd fds[2 + MAX_CLIENTS];
    struct pollfd *fds = calloc(2 + MAX_CLIENTS, sizeof(struct pollfd));

    fds[0].fd = local_socket;
    fds[1].fd = inet_socket;
    fds[0].events = POLLIN;
    fds[1].events = POLLIN;
    pthread_mutex_lock(&mutex);


    for (int i = 0; i < MAX_CLIENTS; i++) {
        fds[i + 2].fd = clients[i].fd;
        fds[i + 2].events = POLLIN;
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
            printf("NEW MSG CLIENT, fd: %d\n", fd);

            if(fd == local_socket || fd == inet_socket){
                printf("NEW MSG, fd: %d\n", fd);
                fd = accept(fd, NULL, NULL);
            }
            break;
        }
    }
    free(fds);
    return fd;
}


void parse_msg(char* msg){
    printf("msg -> %s\n", msg);

    char* buff = calloc(sizeof(char), MAX_MSG_LEN);
    strcpy(buff, msg);

    command = strtok(buff," ");
    arg = strtok(NULL, " ");

    printf("parsed msg -> command: %s, arg: %s,  <%s> <%s>\n", command, arg, msg, buff);
}

void add_client(int client_fd, char* name){
    // ????????????????????????
    printf("idx %d  fd %d\n", idx_to_add, client_fd);
    clients[idx_to_add].fd = client_fd;
//    char* buff = calloc(sizeof(char), MAX_MSG_LEN);
//    strcpy(buff, name);
//    clients[idx_to_add].name = buff;

    printf("Added client od idx [%d].\n", idx_to_add);

    // TODO
    idx_to_add++;

    print_clients(clients);

}

bool check_name(char* name){
    for(int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].fd != -1 && strcmp(clients[i].name, name) == 0){
            printf("Client [%s] exist!\n", name);
            return false;
        }
    }
    return true;
}


void init_clients(){
    for(int i = 0; i < MAX_CLIENTS; i++){
        clients[i].fd = -1;
        clients[i].name = NULL;
    }
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

    init_clients();


    while(true){
        printf("x\n");
        int client_fd = wait_for_messages(local_socket, inet_socket);
        printf("new messss  (%d)\n", client_fd);
        char msg[MAX_MSG_LEN];

        // if no messages are available at the socket, wait for a msg to arrive
        recv(client_fd, msg, MAX_MSG_LEN, 0);
        printf("message: %s\n", msg);


//        if (client_fd) {
            parse_msg(msg);

            pthread_mutex_lock(&mutex);
            if (strcmp(command, "add") == 0){
                if (check_name(arg)) {
                    add_client(client_fd, arg);
                }
                else {
                    char buffer[MAX_MSG_LEN];
                    sprintf(buffer, "name_taken %s", arg);
                    send(client_fd, buffer, MAX_MSG_LEN , 0);

                }
            }

            printf("ELO KONIEC \n\n");

//        }
        pthread_mutex_unlock(&mutex);

    }

}