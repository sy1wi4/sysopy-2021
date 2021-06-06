
#include <arpa/inet.h>
#include "common.h"

int server_socket;
char buffer[MAX_MSG_LEN+ 1];
char *name;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

char *command, *arg;

void parse_msg(char* msg){
    command = strtok(msg, ":");
    arg = strtok(NULL, ":");
}

void init_server_connection(char *type, char *destination){

    if (strcmp(type, "unix") == 0) {
        server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

        struct sockaddr_un local_sockaddr;
        memset(&local_sockaddr, 0, sizeof(struct sockaddr_un));
        local_sockaddr.sun_family = AF_UNIX;
        strcpy(local_sockaddr.sun_path, destination);

        connect(server_socket, (struct sockaddr *)&local_sockaddr,
                sizeof(struct sockaddr_un));
    }
    else {
        struct addrinfo *info;

        struct addrinfo hints;
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        getaddrinfo("localhost", destination, &hints, &info);

        server_socket =
                socket(info->ai_family, info->ai_socktype, info->ai_protocol);

        connect(server_socket, info->ai_addr, info->ai_addrlen);

        freeaddrinfo(info);

//        int sock_fd = socket(AF_INET , SOCK_STREAM, 0);
//
//        if (sock_fd  == -1){
//            printf("Could not create INET socket\n");
//            exit(1);
//        }
//        else{
//            printf("INET socket created\n");
//        }
//
//        struct sockaddr_in sock_addr;
//        sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//        sock_addr.sin_family = AF_INET;
//        sock_addr.sin_port = htons(atoi(destination));
//
//        if (connect(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1) {
//            printf("Error while connecting to socket (%s)\n", strerror(errno));
//            exit(1);
//        }
    }
}



int main(int argc, char* argv[]){

    if (argc < 4){
        printf("Wrong number of arguments!\n");
        exit(1);
    }

    name = argv[1];
    char *type = argv[2];
    char *destination = argv[3];

    // handle SIGINT

    init_server_connection(type, destination);
    char buffer[MAX_MSG_LEN + 1];
    sprintf(buffer, "add: :%s", name);
    send(server_socket, buffer, MAX_MSG_LEN, 0);

    // listen server

    return 0;
}