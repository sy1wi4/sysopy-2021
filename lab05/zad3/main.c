//
// Created by sylwia on 4/15/21.
//

#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>


void create_empty_line(char* line){
    for (int i = 0; i < 255; i++){
        line[i] = ' ';
    }
    line[255] = '\n';
}

void create_empty_file(char* filename, int rows){
    FILE* file = fopen(filename, "w");
    char* line = calloc(256, sizeof(char));
    create_empty_line(line);
    for (int i = 0; i < rows - 1; i++){
        fprintf(file, "%s", line);
    }
    line[255] = '\0';
    fprintf(file, "%s", line);

    fclose(file);
    free(line);
}


void many_to_one(){
    char result[] = "consumer_files/res_many_to_one.txt";
    char cons_N[] = "10";
    char prod_N[] = "5";
    char* input_files[] = {"producer_files/1.txt", "producer_files/2.txt", "producer_files/A.txt", "producer_files/B.txt", "producer_files/C.txt"};
    int producers = 5;

    pid_t pid = fork();
    if (pid == 0){
        execl("./consumer", "./consumer", "pipe", result, cons_N, NULL);
        printf("!!! Return not expected, execl() error !!!\n");
    }

    create_empty_file(result, producers);

    for (int i = 0; i < producers; i++){
        pid = fork();
        char row[2];
        sprintf(row,"%d", i);
        if (pid == 0){
            printf("%d\n", execl("./producer", "./producer", "pipe", row, input_files[i], prod_N,NULL));
        }
    }

    for (int i = 0; i < producers + 1 ; i++){
        wait(NULL);
    }
}

void one_to_many(){
    char result[] = "consumer_files/res_one_to_many.txt";
    char cons_N[] = "10";
    char prod_N[] = "5";
    char input_file[] = "producer_files/A.txt";
    int consumers = 5;

    pid_t pid;
    for (int i = 0; i < consumers; i++){
        pid = fork();
        if (pid == 0){
            execl("./consumer", "./consumer", "pipe", result, cons_N, NULL);
            printf("!!! Return not expected, execl() error !!!\n");
        }
    }

    create_empty_file(result, 1);


    char row[2];
    sprintf(row,"%d", 0);
    pid = fork();
    if (pid == 0){
        printf("%d\n", execl("./producer", "./producer", "pipe", row, input_file, prod_N,NULL));
    }


    for (int i = 0; i < consumers + 1 ; i++){
        wait(NULL);
    }
}


void many_to_many(){
    char result[] = "consumer_files/res_many_to_many.txt";
    char cons_N[] = "10";
    char prod_N[] = "5";
    char* input_files[] = {"producer_files/1.txt", "producer_files/2.txt", "producer_files/A.txt", "producer_files/B.txt", "producer_files/C.txt"};
    int producers = 5;
    int consumers = 5;

    pid_t pid;
    for (int i = 0; i < consumers; i++){
        pid = fork();
        if (pid == 0){
            execl("./consumer", "./consumer", "pipe", result, cons_N, NULL);
            printf("!!! Return not expected, execl() error !!!\n");
        }
    }

    create_empty_file(result, producers);

    for (int i = 0; i < producers; i++){
        pid = fork();
        char row[2];
        sprintf(row,"%d", i);
        if (pid == 0){
            printf("%d\n", execl("./producer", "./producer", "pipe", row, input_files[i], prod_N,NULL));
        }
    }

    for (int i = 0; i < producers + consumers; i++){
        wait(NULL);
    }
}


int main(){

    mkfifo("pipe", 0666);

    printf("###############################\n");
    printf("########  MANY TO ONE  ########\n");
    printf("###############################\n\n");
    many_to_one();

    printf("###############################\n");
    printf("########  ONE TO MANY  ########\n");
    printf("###############################\n\n");
    one_to_many();

    printf("###############################\n");
    printf("########  MANY TO MANY  #######\n");
    printf("###############################\n\n");
    many_to_many();

    return 0;
}

