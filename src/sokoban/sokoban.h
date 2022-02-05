#ifndef SOKOBAN_H
#define SOKOBAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef enum {
    EMPTY = ' ',
    WALL = '#',
    PLAYER = '@',
    CRATE = '$',
    DEST = '.',
    CRATE_ON_DEST = '!',
    PLAYER_ON_DEST = '^',
} FieldType;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
} Direction;

typedef struct Sokoban {
    int width, height;
    char *board;
    int level_index;
    int player_x, player_y;
    int crates_left, moves;
    time_t time_start, time_elapsed;
    int best_time, best_moves;
} Sokoban;


Sokoban *sokoban_init(int width, int height, int level_index);
Sokoban *sokoban_init_from_buffer(char *buffer, int level_index);
bool sa_sokoban_free(Sokoban *level);

void sokoban_print(Sokoban *s);
int sa_coordinate_to_index(Sokoban *level, int x, int y);

void sa_update_time(Sokoban *level);
bool sa_copy_level(Sokoban *level, Sokoban *target);

bool get_delta(Direction d, int *dx, int *dy);
bool is_in_bound(Sokoban *s, int x, int y);

bool move_player(Sokoban *s, Direction d, int changed_fields[3]);
bool move_crate(Sokoban *s, Direction d);
int swap(Sokoban *s, int x, int y, Direction d);

bool parse_board(char *lvl_buffer, int *x, int *y);
char *board_get_field_at(Sokoban *s, int x, int y);

bool sa_is_new_best_moves(Sokoban *level);
bool sa_is_new_best_time(Sokoban *level);

void sa_read_metadata(Sokoban *level);
void sa_save_level(Sokoban *level);
void sa_level_progress_to_file(Sokoban *level);
void sa_level_meta_to_file(Sokoban *level);

#endif
