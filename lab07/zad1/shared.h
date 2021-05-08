//
// Created by sylwia on 5/8/21.
//

#ifndef LAB07_SHARED_H
#define LAB07_SHARED_H

#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>

#define ID 'S'
#define SH_M_SIZE  1024


char* get_home_path(){
    char* path = getenv("HOME");
    if (path == NULL) {
        path = getpwuid(getuid())->pw_dir;
    }
    return path;
}

#endif //LAB07_SHARED_H
