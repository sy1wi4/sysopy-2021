#include "common.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

client *clients[MAX_CLIENTS] = {NULL};
int clients_ctr = 0;

int get_opponent(int idx){
    return idx % 2 == 0 ? idx + 1 : idx - 1;
}


int add_client(char *name, int fd){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i] != NULL && strcmp(clients[i]->name, name) == 0) return -1;
    }

    int idx = -1;

    // another waiting client
    for (int i = 0; i < MAX_CLIENTS; i += 2){
        if (clients[i] != NULL && clients[i + 1] == NULL){
            idx = i + 1;
            break;
        }
    }

    // no opponent - first free place
    if (idx == -1){
        for (int i = 0; i < MAX_CLIENTS; i++){
            if (clients[i] == NULL){
                idx = i;
                break;
            }
        }
    }

    if (idx != -1){
        client *new_client = calloc(1, sizeof(client));
        new_client->name = calloc(MAX_MSG_LEN, sizeof(char));
        strcpy(new_client->name, name);
        new_client->fd = fd;
        new_client->available = true;

        clients[idx] = new_client;
        clients_ctr++;
    }

    return idx;
}


int find_client(char *name){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i] != NULL && strcmp(clients[i]->name, name) == 0) return i;
    }
    return -1;
}

void free_client(int index){
    free(clients[index]->name);
    free(clients[index]);
    clients[index] = NULL;
    clients_ctr--;

    int opponent = get_opponent(index);

    // remove opponent if exists
    if (clients[get_opponent(index)] != NULL){
        printf("Removing opponent: %s\n", clients[opponent]->name);
        free(clients[opponent]->name);
        free(clients[opponent]);
        clients[opponent] = NULL;
        clients_ctr--;
    }
}

void remove_client(char *name){
    int idx = -1;
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i] != NULL && strcmp(clients[i]->name, name) == 0) idx = i;
    }

    printf("Removing client: %s\n", name);
    free_client(idx);
}


void delete_not_available_clients(){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i] && ! clients[i]->available){
            printf("not available\n");
            remove_client(clients[i]->name);
        }
    }
}

void send_pings(){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i]){
            send(clients[i]->fd, "ping: ", MAX_MSG_LEN, 0);
            clients[i]->available = false;
        }
    }
}

void* ping(){
    while (true){
        printf("PING\n");
        pthread_mutex_lock(&mutex);
        delete_not_available_clients();
        send_pings();
        pthread_mutex_unlock(&mutex);

        sleep(10);
    }
}

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

    // listen_for_msg for connections on a socket
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

    // listen_for_msg for connections on a socket
    if ((listen(sock_fd, MAX_CLIENTS)) == -1){
        printf("Listen on INET socket failed\n");
        exit(1);
    }

    printf("INET socket fd: %d\n", sock_fd);

    return sock_fd;

}

int check_messages(int local_socket, int network_socket){

    struct pollfd *fds = calloc(2 + clients_ctr, sizeof(struct pollfd));
    fds[0].fd = local_socket;
    fds[0].events = POLLIN;
    fds[1].fd = network_socket;
    fds[1].events = POLLIN;

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < clients_ctr; i++){
        fds[i + 2].fd = clients[i]->fd;
        fds[i + 2].events = POLLIN;
    }

    pthread_mutex_unlock(&mutex);

    poll(fds, clients_ctr + 2, -1);
    int retval;

    for (int i = 0; i < clients_ctr + 2; i++){
        if (fds[i].revents & POLLIN){
            retval = fds[i].fd;
            break;
        }
    }
    if (retval == local_socket || retval == network_socket){
        retval = accept(retval, NULL, NULL);
    }
    free(fds);

    return retval;
}

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Wrong number of arguments!\n");
        exit(1);
    }
    srand(time(NULL));

    char *port = argv[1];
    char *socket_path = argv[2];

    int local_socket = set_local_socket(socket_path);
    int network_socket = set_network_socket(port);

    pthread_t t;
    pthread_create(&t, NULL, &ping, NULL);
    while (true){
        int client_fd = check_messages(local_socket, network_socket);
        char buffer[MAX_MSG_LEN + 1];
        recv(client_fd, buffer, MAX_MSG_LEN, 0);

        printf("%s\n", buffer);
        char *command = strtok(buffer, ":");
        char *arg = strtok(NULL, ":");
        char *name = strtok(NULL, ":");

        pthread_mutex_lock(&mutex);
        if (strcmp(command, "add") == 0){
            int index = add_client(name, client_fd);

            if (index == -1){
                send(client_fd, "add:name_taken", MAX_MSG_LEN, 0);
                close(client_fd);
            }

            else if (index % 2 == 0){
                send(client_fd, "add:no_enemy", MAX_MSG_LEN, 0);
            }

            else{
                int random_num = rand() % 100;
                int first, second;

                if (random_num % 2 == 0){
                    first = index;
                    second = get_opponent(index);
                }

                else{
                    second = index;
                    first = get_opponent(index);
                }

                send(clients[first]->fd, "add:O",
                     MAX_MSG_LEN, 0);
                send(clients[second]->fd, "add:X",
                     MAX_MSG_LEN, 0);
            }
        }

        if (strcmp(command, "move") == 0){
            int move = atoi(arg);
            int player = find_client(name);

            sprintf(buffer, "move:%d", move);
            send(clients[get_opponent(player)]->fd, buffer, MAX_MSG_LEN,
                 0);
        }
        if (strcmp(command, "end") == 0){
            remove_client(name);
        }

        if (strcmp(command, "pong") == 0){
            int player = find_client(name);
            if (player != -1) clients[player]->available = 1;

        }
        pthread_mutex_unlock(&mutex);
    }
}