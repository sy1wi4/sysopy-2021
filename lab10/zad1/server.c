//
// Created by sylwia on 5/26/21.
//

#include "common.h"

void set_local_socket(char* socket_path){
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

    printf("[s] LOCAL socket fd: %d\n", sock_fd);


}

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Wrong number of arguments!\n");
        exit(1);
    }

    int port_number = atoi(argv[1]);
    char* socket_path = argv[2];

    set_local_socket(socket_path);

}