#ifndef UTILS_H
#define UTILS_H

#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool ut_file_exists(char *path);
char *ut_file_buffer(char *path);
bool ut_remove_file(char *path);


#endif
