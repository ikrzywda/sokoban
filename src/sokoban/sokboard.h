#ifndef SOKOBAN_SRC_SOKBOARD_H
#define SOKOBAN_SRC_SOKBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define EMPTY           0
#define WALL            '#'
#define PLAYER          '@'
#define CRATE           '$'
#define DEST            '.'
#define CRATE_ON_DEST   '^'
#define PLAYER_ON_DEST  '!'

#define MOVE_UP       1
#define MOVE_DOWN     -1
#define MOVE_LEFT     2
#define MOVE_RIGHT    -2

typedef int BoardCoordinates[2];

typedef struct Sobokan {
    int width, height;
    char *board;
    BoardCoordinates ppos;
    int crates_left;
    int moves;
    time_t time;
} Sobokan;

void sob_print_board(Sobokan *board);
Sobokan *sob_new_instance();

char sob_get_field_at(Sobokan *s, int x, int y);
bool sob_set_field_at(Sobokan *s, char c, int x, int y);

bool sob_find_player(Sobokan *board);
bool sob_count_crates(Sobokan *game_instance);

void sob_fill_with_buffer(Sobokan *board, int buffer_size, char *buffer);
Sobokan *sob_init_from_file(const char *source);

bool sob_get_next_pos(const int direction, BoardCoordinates obj_pos, 
                      BoardCoordinates next_pos);

bool sob_player_move_up(Sobokan *g_inst, const int direction);

#endif
