//
// Created by sylwia on 5/11/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>

#define ROW_LEN 256

int** image;
int** negative_image;
int w, h;
int threads_num;

void print_arr(int** arr){
    for (int i = 0; i < h; i++){
        printf("line %d: \n", i);
        for (int j = 0; j < w; j++){
            printf("%d ", arr[i][j]);
        }
        printf("\n\n\n");
    }
}

void load_image(char* filename){
    FILE* f = fopen(filename, "r");
    if (f == NULL){
        printf("Error while opening input file\n");
        exit(1);
    }

//    file format:

//    P2   - "magic number"
//    W H  -  width height
//    M    -  max pixel value
//    ...

    char* buffer = calloc(ROW_LEN, sizeof(char));

    // skip "magic number" (P2)
    fgets(buffer, ROW_LEN, f);

    bool get_M;
    bool read_image = false;

    while(!read_image && fgets(buffer, ROW_LEN, f)){
        if (buffer[0] == '#') continue;

        else if (!read_image && !get_M){
            sscanf(buffer, "%d %d\n", &w, &h);
            printf("w: %d, h: %d\n", w, h);

            get_M = true;
        }

        else {
            // check max pixel value
            int M;
            sscanf(buffer, "%d \n", &M);
            printf("M: %d\n", M);
            if (M != 255){
                printf("Max grey value must be 255!\n");
                exit(1);
            }
            read_image = true;
        }
    }

    // read image
    image = calloc(h, sizeof(int *));
    for (int i = 0; i < h; i++) {
        image[i] = calloc(w, sizeof(int));
    }

    int pixel;

    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            fscanf(f, "%d", &pixel);
            image[i][j] = pixel;
        }
    }

//    print_arr(image);
    fclose(f);
}


void* numbers_method(void* arg){
    int idx = *((int *) arg);
    printf("Hello from numbers method,   %d\n", idx);

    // pixels from range [256/k*idx, 256/k*(idx+1)], where k is number of threads
    // last thread takes the rest of the range (up to 255 inclusive)
    int from = 256 / threads_num * idx;
    int to = (idx != threads_num - 1) ? (256 / threads_num * (idx + 1) ) : 256;

    printf("from [%d] to [%d]\n", from, to);

    int pixel;
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            pixel = image[i][j];
            if (pixel >= from && pixel < to){
                negative_image[i][j] = 255 - pixel;
//                printf("changed: %d, %d", pixel, negative_image[i][j]);
            }
        }
    }
}


void* block_method(void* arg) {
    int idx = *((int *) arg);
    printf("Hello from block method,   %d\n", idx);

    int x_from = (idx) * ceil(w / threads_num);
    int x_to = (idx != threads_num - 1) ? ((idx + 1)* ceil(w / threads_num) - 1) : w - 1;

    printf("from: %d to: %d \n", x_from, x_to);

    int pixel;
    for (int i = 0; i < h; i++){
        for (int j = x_from; j <= x_to; j++){
            pixel = image[i][j];
            negative_image[i][j] = 255 - pixel;
        }
    }

}


void save_negative(char* filename){
    FILE* f = fopen(filename, "w");

    if (f == NULL){
        printf("Error while opening output file\n");
        exit(1);
    }

    fprintf(f, "P2\n");
    fprintf(f, "%d %d\n", w, h);
    fprintf(f, "255\n");

    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++) {
            fprintf(f, "%d ", negative_image[i][j]);
        }
        fprintf(f, "\n");
    }

}


int main(int argc, char* argv[]){

    if (argc != 5){
        printf("Wrong number of arguments!\n");
        exit(1);
    }

    threads_num = atoi(argv[1]);
    char* method = argv[2];
    char* input_file = argv[3];
    char* output_file = argv[4];

    if ((strcmp(method, "numbers") != 0) && (strcmp(method, "block") != 0)) {
        printf("Unknown method!\n");
        exit(1);
    }

    load_image(input_file);

    negative_image = calloc(h, sizeof(int *));
    for (int i = 0; i < h; i++) {
        negative_image[i] = calloc(w, sizeof(int));
    }

    // create and start threads
    pthread_t* threads = calloc(threads_num, sizeof(pthread_t));
    int* threads_idx = calloc(threads_num, sizeof(int));

    for(int i = 0; i < threads_num; i++){
        threads_idx[i] = i;

        // idx as function argument

        if (strcmp(method, "numbers") == 0){
            pthread_create(&threads[i], NULL, &numbers_method, &threads_idx[i]);
        }

        else if (strcmp(method, "block") == 0){
            pthread_create(&threads[i], NULL, &block_method, &threads_idx[i]);
        }
    }


    // wait for threads to finish
    for(int i = 0; i < threads_num; i++) {
        pthread_join(threads[i], NULL);
    }


    save_negative(output_file);

    return 0;
}

