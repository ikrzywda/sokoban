#include "utils.h"

#include <stdio.h>


bool ut_file_exists(char *path) {
    FILE *f = fopen(path, "r");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
}

char *ut_file_buffer(char *path) {
    if (!ut_file_exists(path)) return NULL;
    char *buffer = malloc(LEVEL_BUFFER_SIZE), c;
    FILE *f = fopen(path, "r");
    int i = 0;

    while ((c = fgetc(f)) != EOF) buffer[i++] = c;
    buffer[i] = '\0';
    fclose(f);
    return buffer;
}

bool ut_remove_file(char *path) {
    if (!ut_file_exists(path)) return false;
    remove(path); 
    return true;
}
