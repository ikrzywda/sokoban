#ifndef SOKOBAN_SRC_SOKBOARD_H
#define SOKOBAN_SRC_SOKBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef enum {
    EMPTY = 0,
    WALL = '#',
    PLAYER = '@',
    CRATE = '$',
    DEST = '.',
    CRATE_ON_DEST = '^',
    PLAYER_ON_DEST = '^',
} SobField;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
} SobDirection;


typedef int SobPos[2];

typedef struct SobInstance {
    int width, height;
    char *board;
    SobPos ppos;
    int crates_left;
    int moves;
    time_t time;
} SobInstance;

void sob_print_g_inst(SobInstance *g_inst);
SobInstance *sob_new_instance();

char sob_get_field_at(SobInstance *s, int x, int y);
bool sob_set_field_at(SobInstance *s, char c, int x, int y);

bool sob_find_player(SobInstance *g_inst);
bool sob_count_crates(SobInstance *game_instance);

void sob_fill_with_buffer(SobInstance *g_inst, int buffer_size, char *buffer);
SobInstance *sob_init_from_file(const char *source);

SobPos *next_pos(SobInstance *g_inst, SobPos pos, const int dir);
bool sob_move_player(SobInstance *g_inst, const int dir);
bool sob_move_crate(SobInstance *g_inst, SobPos pos, const int dir);

#endif
