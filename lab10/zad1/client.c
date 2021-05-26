//
// Created by sylwia on 5/26/21.
//

#include <errno.h>
#include "common.h"

char* name;
int sock_fd;


void connect_local(char* path){
    printf("connect LOCALLY\n");

    // create unix socket
    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
        printf("Could not create socket\n");
        exit(1);
    }
    else{
        printf("Socket created\n");

    }

    // unix socket address
    struct sockaddr_un sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_un));
    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, path);

    // connect to socket
    if (connect(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1) {
        printf("Error while connecting to socket (%s)\n", strerror(errno));
        exit(1);
    }
}

void connect_inet(char* address, int port_number){
    printf("connect via INTERNET\n");

}


int main(int argc, char* argv[]){

    if (argc < 4){
        printf("Wrong number of arguments!\n");
        exit(1);
    }

    // name
    name = argv[1];

    // connection method - inet/unix
    if (strcmp(argv[2], "unix") == 0){

        // server address
        char* path = argv[3];

        connect_local(path);

    }
    else if (strcmp(argv[2], "inet") == 0){

        // server address
        char* IPv4_address = argv[3];
        int port_number = atoi(argv[4]);

        connect_inet(IPv4_address, port_number);
    }
    else{
        printf("Wrong method - choose [inet] or [unix]!\n");
        exit(1);
    }


    return 0;
}
