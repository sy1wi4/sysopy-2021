
#include <arpa/inet.h>
#include "common.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

client* clients[MAX_CLIENTS] = {NULL};
int clients_ctr = 0;

int free_idx = 0;
int waiting_idx = -1;

void print_clients() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == NULL) printf(" | ");
        else printf("%s | ", clients[i]->name);
    }
    printf("\n");
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

int find_client(char* name){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i] != NULL && strcmp(clients[i]->name, name) == 0){
            return i;
        }
    }
    return -1;
}


void delete_client(char* name){
    printf("Deleting client [%s]...\n", name);
    int idx = find_client(name);
    int opponent = clients[idx]->opponent_idx;
    printf("id %d, opponent %d\n", idx,opponent);
    free(clients[idx]->name);
    free(clients[idx]);
    clients[idx] = NULL;
    clients_ctr--;
    if (waiting_idx == idx) waiting_idx = -1;

    // delete opponent if exists
    if (opponent != -1){
        printf("Deleting opponent [%s]...\n", clients[opponent]->name);
        free(clients[opponent]->name);
        free(clients[opponent]);
        clients[opponent] = NULL;
        clients_ctr--;
    }
    print_clients();

}

void delete_not_available_clients(){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i] && ! clients[i]->available){
            printf("not available\n");
            delete_client(clients[i]->name);
        }
    }
}

void send_pings(){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i]){
            printf("Sending ping to %s\n", clients[i]->name);
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


int check_messages(int local_socket, int network_socket)
{
    struct pollfd *fds = calloc(2 + clients_ctr, sizeof(struct pollfd));
    fds[0].fd = local_socket;
    fds[1].fd = network_socket;

    fds[0].events = POLLIN;
    fds[1].events = POLLIN;

    pthread_mutex_lock(&mutex);

    for (int i = 0; i < clients_ctr; i++){
        fds[i + 2].fd = clients[i]->fd;
        fds[i + 2].events = POLLIN;
    }

    pthread_mutex_unlock(&mutex);
    poll(fds, clients_ctr + 2, -1);
    int ret;

    for (int i = 0; i < clients_ctr + 2; i++){
        if (fds[i].revents & POLLIN){
            ret = fds[i].fd;
            break;
        }
    }

    if (ret == local_socket || ret == network_socket)
        ret = accept(ret, NULL, NULL);

    free(fds);
    return ret;
}

bool client_exists(char* name){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i] != NULL && strcmp(clients[i]->name, name) == 0){
            return true;
        }
    }
    return false;
}

int add_client(char* name, int client_fd){
    client *new_client = calloc(1, sizeof(client));
    new_client->name = calloc(64, sizeof(char));
    strcpy(new_client->name, name);
    new_client->fd = client_fd;
    new_client->available = true;
    new_client->opponent_idx = -1;
    clients[free_idx] = new_client;

    clients_ctr++;
    if (waiting_idx == -1){
        // wait for an opponent
        waiting_idx = free_idx;
        send(client_fd, "add:waiting", MAX_MSG_LEN, 0);
        printf("Client %s waiting for opponent...\n", name);
    }
    else{
        // connect

        // TODO: random sign
        send(client_fd, "add:O", MAX_MSG_LEN, 0);
        send(clients[waiting_idx]->fd, "add:X", MAX_MSG_LEN, 0);
        clients[free_idx]->opponent_idx = waiting_idx;
        clients[waiting_idx]->opponent_idx = free_idx;
        printf("Playing: %s vs %s\n", name, clients[waiting_idx]->name);
        waiting_idx = -1;
    }
    free_idx++;
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
    int network_socket =set_network_socket(port);

    pthread_t ping_thread;
    pthread_create(&ping_thread, NULL, &ping, NULL);

    while (true){

        int client_fd = check_messages(local_socket, network_socket);

        char buffer[MAX_MSG_LEN + 1];
        recv(client_fd, buffer, MAX_MSG_LEN, 0);
        printf("new:   %s\n", buffer);

        // parse commands
        char *command = strtok(buffer, ":");
        char *arg = strtok(NULL, ":");
        char *name = strtok(NULL, ":");

        // handle commands
        pthread_mutex_lock(&mutex);
        if (strcmp(command, "add") == 0){
            printf("dodaj\n");
            if (client_exists(name)) {
                send(client_fd, "add:exists", MAX_MSG_LEN, 0);
                close(client_fd);
            }

            else add_client(name, client_fd);

            print_clients();

        }

        else if (strcmp(command, "turn") == 0){
            printf("ruch\n");
        }

        else if (strcmp(command, "ping") == 0){
            printf("Ping back from [%s]\n", name);
            int idx = find_client(name);
            clients[idx]->available = true;
        }

        else if (strcmp(command, "end") == 0){
            printf("koniec\n");
            delete_client(name);
        }


        pthread_mutex_unlock(&mutex);
    }
}