//
// Created by sylwia on 3/24/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>


char* update_path(char* path, char* current){
    char* new_path = calloc(sizeof(char), strlen(path) + strlen(current));
    sprintf(new_path, "%s/%s", path, current);
    return new_path;
}

bool check_if_contains(FILE* file, char* str){
    char* line = calloc(sizeof(char), 256);
    while(fgets(line, 256 * sizeof(char), file)){
        if(strstr(line, str)) return true;

    }
    return false;
}

void search_dir(char* dir_name, char* str, int depth){
    if (depth == 0) exit(0);

    DIR* dir;
    FILE* file;
    struct dirent* dp;
    char* path;
    char* extension;
    bool contains;

    if ((dir = opendir (dir_name)) == NULL) {
        perror ("Cannot open directory");
        exit (1);
    }

    while ((dp = readdir(dir))){
        if ((strcmp(dp->d_name, "..") == 0) || (strcmp(dp->d_name, ".") == 0)) continue;
        path = update_path(dir_name, dp->d_name);
        extension = strrchr(dp->d_name,'.');

        // encountered directory
        if (dp->d_type == DT_DIR && fork() == 0){
            search_dir(path, str, depth - 1);
            exit(0);
        }

        // only text files
        else if (extension != NULL && strcmp(extension, ".txt") == 0) {

            file = fopen(path, "r");
            if (file == NULL) {
                perror("Cannot open file");
                exit(1);
            }
            contains = check_if_contains(file, str);
            if (contains){
                printf("\npath: %s\n", path);
                printf("PID:  %d \nPPID: %d\n", getpid(), getppid());
            }
        }

        wait(NULL);
        free(path);
    }
    closedir(dir);
}

int main(int argc, char* argv[]){
    //  make ARGS=". b 4"

    if (argc != 4){
        printf("Wrong number of arguments");
        return -1;
    }

    else{
        char* dir_name = argv[1];
        char* str = argv[2];
        int depth = atoi(argv[3]);

        search_dir(dir_name, str, depth);
    }

    return 0;
}

