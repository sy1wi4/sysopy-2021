//
// Created by sylwia on 4/14/21.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 256

void print_output(FILE* file){
    char line[MAX_SIZE];
    while(fgets(line, MAX_SIZE, file) != NULL){
        printf("%s\n", line);
    }
}

void print_ordered_by(char* mode){
    FILE* file;
    char* command;

    // tail +2     -> print data starting from second line
    // head -n -1  -> print data except last line
    // tac         -> reverse order of lines
    // sort -k 2   -> sort by second column


    if (strcmp(mode,"date") == 0) {
        command = "echo | mail -f | tail +2 | head -n -1 | tac";
    }
    else if (strcmp(mode, "sender") == 0){
        command = "echo | mail -f | tail +2 | head -n -1 | sort -k 2";
    }
    else{
        printf("Wrong mode! Choose: [date] or [sender].\n");
        exit(1);
    }

    file = popen(command, "r");

    if (file == NULL){
        printf("popen() error!");
        exit(1);
    }

    printf("\nSORTED EMAILS: ");
    print_output(file);
}

void send_email(char* address, char* object, char* content){
    FILE* file;
    char command[MAX_SIZE];

    snprintf(command, sizeof(command), "echo %s | mail -s %s %s", content, object, address);
    printf("%s\n", command);
    file = popen(command, "r");

    if (file == NULL){
        printf("popen() error!");
        exit(1);
    }

    printf("\n----- Email sent! -----\n");
    printf("TO:      %s\nOBJECT:  %s\nCONTENT: %s\n", address, object, content);

}

int main(int argc, char* argv[]){
    // sylwia@ubuntu TEST message"

    if (argc != 2 && argc != 4){
        printf("Wrong number of arguments!");
        exit(1);
    }


    if (argc == 2){
        // print emails ordered by [date] or [sender]
        printf("Print emails ordered by [%s]...\n", argv[1]);
        print_ordered_by(argv[1]);
    }

    else {
        // send email, args: [address] [subject] [content]
        printf("Send email...\n");
        char* address = argv[1];
        char* subject = argv[2];
        char* content = argv[3];

        send_email(address, subject, content);
    }

}
