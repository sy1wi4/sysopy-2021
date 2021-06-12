#include "common.h"

int server_socket;
int is_client_O;
char buffer[MAX_MSG_LEN + 1];
char *name, *command, *arg;
game_board g_board;
state current_state = START;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int move(game_board *board, int position){
    if (position < 0 || position > 9 || board->objects[position] != FREE)
        return 0;

    board->objects[position] = board->move ? O : X;
    board->move = !board->move;
    return 1;
}

sign check_winner(game_board *board){
    sign column = FREE;

    for (int x = 0; x < 3; x++){
        sign first = board->objects[x];
        sign second = board->objects[x + 3];
        sign third = board->objects[x + 6];

        if (first == second && first == third && first != FREE)
            column = first;
    }

    if (column != FREE)
        return column;

    sign row = FREE;
    for (int y = 0; y < 3; y++){
        sign first = board->objects[3 * y];
        sign second = board->objects[3 * y + 1];
        sign third = board->objects[3 * y + 2];
        if (first == second && first == third && first != FREE)
            row = first;
    }

    if (row != FREE)
        return row;

    sign lower_diagonal = FREE;

    sign first = board->objects[0];
    sign second = board->objects[4];
    sign third = board->objects[8];

    if (first == second && first == third && first != FREE) lower_diagonal = first;
    if (lower_diagonal != FREE) return lower_diagonal;

    sign upper_diagonal = FREE;
    first = board->objects[2];
    second = board->objects[4];
    third = board->objects[6];
    if (first == second && first == third && first != FREE)
        upper_diagonal = first;

    return upper_diagonal;
}

void end(){
    char buffer[MAX_MSG_LEN + 1];
    sprintf(buffer, "end: :%s", name);
    send(server_socket, buffer, MAX_MSG_LEN, 0);
    exit(0);
}

void check_game_status(){
    bool win = false;
    sign winner = check_winner(&g_board);

    if (winner != FREE){
        if ((is_client_O && winner == O) || (!is_client_O && winner == X)) printf("You won!\n");
        else printf("You lost!\n");

        win = true;
    }

    bool draw = true;
    for (int i = 0; i < 9; i++){
        if (g_board.objects[i] == FREE){
            draw = false;
            break;
        }
    }

    if (draw && !win) printf("It's a draw!\n");

    if (win || draw) current_state = QUIT;
}

void parse_command(char* msg){
    command = strtok(msg, ":");
    arg = strtok(NULL, ":");
}

game_board create_board(){
    game_board board = {1,{FREE}};
    return board;
}

void draw(){
    char symbol;

    for (int y = 0; y < 3; y++){
        for (int x = 0; x < 3; x++){
            if (g_board.objects[y * 3 + x] == FREE) symbol = y * 3 + x + 1 + '0';
            else if (g_board.objects[y * 3 + x] == O) symbol = 'O';
            else symbol = 'X';

            printf("  %c  ", symbol);
        }
        printf("\n_________________________\n");
    }
}

void play_game(){
    while (true){
        if (current_state == START){
            if (strcmp(arg, "name_taken") == 0) exit(1);
            else if (strcmp(arg, "no_enemy") == 0) current_state = WAIT_FOR_OPPONENT;
            else{
                g_board = create_board();
                is_client_O = arg[0] == 'O';
                current_state = is_client_O ? MOVE : WAIT_FOR_MOVE;
            }
        }

        else if (current_state == WAIT_FOR_OPPONENT){

            pthread_mutex_lock(&mutex);

            while (current_state != START && current_state != QUIT) pthread_cond_wait(&cond, &mutex);

            pthread_mutex_unlock(&mutex);

            g_board = create_board();
            is_client_O = arg[0] == 'O';
            current_state = is_client_O ? MOVE : WAIT_FOR_MOVE;
        }
        else if (current_state == WAIT_FOR_MOVE){
            printf("Waiting for rivals move\n");

            pthread_mutex_lock(&mutex);
            while (current_state != OPPONENT_MOVE && current_state != QUIT){
                pthread_cond_wait(&cond, &mutex);
            }
            pthread_mutex_unlock(&mutex);
        }
        else if (current_state == OPPONENT_MOVE){
            int pos = atoi(arg);
            move(&g_board, pos);
            check_game_status();
            if (current_state != QUIT){
                current_state = MOVE;
            }
        }
        else if (current_state == MOVE){
            draw();

            int pos;
            do{
                printf("Next move (%c): ", is_client_O ? 'O' : 'X');
                scanf("%d", &pos);
                pos--;
            } while (!move(&g_board, pos));

            draw();

            char buffer[MAX_MSG_LEN + 1];
            sprintf(buffer, "move:%d:%s", pos, name);
            send(server_socket, buffer, MAX_MSG_LEN, 0);

            check_game_status();
            if (current_state != QUIT){
                current_state = WAIT_FOR_MOVE;
            }
        }
        else if (current_state == QUIT) end();
    }
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

void listen_server()
{
    int game_thread_running = 0;
    while (1)
    {
        recv(server_socket, buffer, MAX_MSG_LEN, 0);
        parse_command(buffer);

        pthread_mutex_lock(&mutex);
        if (strcmp(command, "add") == 0)
        {
            current_state = START;
            if (!game_thread_running)
            {
                pthread_t t;
                pthread_create(&t, NULL, (void *(*)(void *))play_game, NULL);
                game_thread_running = 1;
            }
        }
        else if (strcmp(command, "move") == 0)
        {
            current_state = OPPONENT_MOVE;
        }
        else if (strcmp(command, "end") == 0)
        {
            current_state = QUIT;
            exit(0);
        }
        else if (strcmp(command, "ping") == 0)
        {
            sprintf(buffer, "pong: :%s", name);
            send(server_socket, buffer, MAX_MSG_LEN, 0);
        }
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
}


int main(int argc, char* argv[]){

    if (argc < 4){
        printf("Wrong number of arguments!\n");
        exit(1);
    }

    name = argv[1];

    // handle SIGINT
    signal(SIGINT, end);

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


    char msg[MAX_MSG_LEN];
    sprintf(msg, "add: :%s", name);
    send(server_socket, msg, MAX_MSG_LEN, 0);

    // listen_for_msg server
    listen_server();

    return 0;
}