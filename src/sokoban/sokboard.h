#ifndef SOKOBAN_SRC_SOKBOARD_H
#define SOKOBAN_SRC_SOKBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define EMPTY   0
#define WALL    1
#define PLAYER  2
#define CRATE   3
#define DEST    4
#define PLAYER_ON_CRATE     PLAYER + CRATE
#define PLAYER_ON_DEST      PLAYER + DEST
#define CRATE_ON_DEST       CRATE + DESTINATION

typedef struct SokBoard {
    int width, height;
    char *board;
} SokBoard;

SokBoard *sokboard_init();
char sokboard_get_field_at(SokBoard *s, int x, int y);
bool sokboard_set_field_at(SokBoard *s, char c, int x, int y);
void sokboard_print(SokBoard *board);
void sokboard_fill_with_buffer(SokBoard *board, int buffer_size, char *buffer);
SokBoard *sokboard_read_from_file(const char *source);

#endif
