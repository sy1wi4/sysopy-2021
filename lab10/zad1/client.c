//
// Created by sylwia on 5/26/21.
//

#include "common.h"

char* name;

void connect_local(char* path){
    printf("connect LOCALLY\n");

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
    if (strcmp(argv[2], "unix")){

        // server address
        char* path = argv[2];

        connect_local(path);

    }
    else if (strcmp(argv[2], "inet")){

        // server address
        char* IPv4_address = argv[2];
        int port_number = atoi(argv[3]);

        connect_inet(IPv4_address, port_number);
    }
    else{
        printf("Wrong method - choose [inet] or [unix]!\n");
        exit(1);
    }


    return 0;
}
