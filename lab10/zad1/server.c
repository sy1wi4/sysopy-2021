
#include <arpa/inet.h>
#include "common.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

client *clients[MAX_CLIENTS] = {NULL};
int clients_ctr = 0;


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

int set_network_socket(char *port){
    // create socket
    int sock_fd = socket(AF_INET , SOCK_STREAM, 0);

    if (sock_fd  == -1){
        printf("Could not create INET socket\n");
        exit(1);
    }
    else{
        printf("INET socket created\n");
    }

    // set port and IP
    struct sockaddr_in sock_addr;
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(atoi(port));

    // assigns the address to the socket
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



void* ping(){

}


int check_messages(int local_socket, int network_socket)
{
    struct pollfd *fds = calloc(2 + clients_ctr, sizeof(struct pollfd));
    fds[0].fd = local_socket;
    fds[0].events = POLLIN;
    fds[1].fd = network_socket;
    fds[1].events = POLLIN;
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < clients_ctr; i++)
    {
        fds[i + 2].fd = clients[i]->fd;
        fds[i + 2].events = POLLIN;
    }
    pthread_mutex_unlock(&mutex);
    poll(fds, clients_ctr + 2, -1);
    int retval;
    // check for message
    for (int i = 0; i < clients_ctr + 2; i++)
    {
        if (fds[i].revents & POLLIN)
        {
            retval = fds[i].fd;
            break;
        }
    }
    if (retval == local_socket || retval == network_socket)
    {
        retval = accept(retval, NULL, NULL);
    }
    free(fds);

    return retval;
}

int main(int argc, char* argv[])
{
    if (argc != 3){
        printf("Wrong number of arguments!\n");
        exit(1);
    }
    srand(time(NULL));

    char *port = argv[1];
    char *socket_path = argv[2];

    int local_socket = set_local_socket(socket_path);
    int network_socket =set_network_socket(port);

    pthread_t ping_thread;
    pthread_create(&ping_thread, NULL, &ping, NULL);

    while (true){

        int client_fd = check_messages(local_socket, network_socket);
        char buffer[MAX_MSG_LEN + 1];
        recv(client_fd, buffer, MAX_MSG_LEN, 0);
        printf("%s\n", buffer);

        // parse commands

        pthread_mutex_lock(&mutex);

        // handle commands

        pthread_mutex_unlock(&mutex);
    }
}