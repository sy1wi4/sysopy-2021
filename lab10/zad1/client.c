
#include <arpa/inet.h>
#include "common.h"

int server_socket;
char msg[MAX_MSG_LEN+ 1];
char *name;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

char *command, *arg;

void parse_msg(char* message){
    command = strtok(message, ":");
    arg = strtok(NULL, ":");
}

void connect_local(char* path){
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_un));
    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, path);

    if (connect(server_socket, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1) {
        printf("Error while connecting to LOCAL socket (%s)\n", strerror(errno));
        exit(1);
    }
}

void connect_inet(char* port){

    struct addrinfo *info;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo("localhost", port, &hints, &info);

    server_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);

    if (connect(server_socket, info->ai_addr, info->ai_addrlen) == -1){
        printf("Error while connecting to INET socket (%s)\n", strerror(errno));
        exit(1);
    }

    freeaddrinfo(info);
}


void listen_for_msg(){
    while(true){
        printf("CZEKAM\n");
        recv(server_socket, msg, MAX_MSG_LEN, 0);
        parse_msg(msg);

        pthread_mutex_lock(&mutex);
        // handle commands
        if (strcmp(command, "add") == 0){
            printf("dodaj tu\n");
        }

        else if (strcmp(command, "turn") == 0){
            printf("ruch\n");

        }

        else if (strcmp(command, "ping") == 0){
            printf("ping\n");

        }

        else if (strcmp(command, "end") == 0){
            printf("koniec\n");

        }
    }
}

void end(){
    char message[MAX_MSG_LEN];
    sprintf(message, "end: :%s", name);
    send(server_socket, message, MAX_MSG_LEN, 0);
    exit(0);
}

int main(int argc, char* argv[]){

    if (argc < 4){
        printf("Wrong number of arguments!\n");
        exit(1);
    }

    name = argv[1];

    // connection method - inet/unix
    if (strcmp(argv[2], "unix") == 0){
        char* path = argv[3];
        connect_local(path);
    }

    else if (strcmp(argv[2], "inet") == 0){
        char* port = argv[3];
        connect_inet(port);
    }

    else{
        printf("Wrong method - choose [inet] or [unix]!\n");
        exit(1);
    }


    // handle SIGINT
    signal(SIGINT, end);


    char msg[MAX_MSG_LEN];
    sprintf(msg, "add: :%s", name);
    send(server_socket, msg, MAX_MSG_LEN, 0);

    // listen_for_msg server
    listen_for_msg();

    return 0;
}