#include "sokboard.h"

Sobokan *sob_new_instance(int x, int y) {
    Sobokan *new_instance = malloc(sizeof(Sobokan));
    new_instance->width = x;
    new_instance->height = y;
    new_instance->board = malloc(x * y);
    new_instance->crates_left = 0;
    new_instance->moves = 0;

    return new_instance;
}

char sob_get_field_at(Sobokan *s, int x, int y) {
    if (x > s->width || y > s->height) 
        return -1;
    else 
        return s->board[x * s->width + y];
}

bool sob_set_field_at(Sobokan *s, char c, int x, int y) {
    if (x > s->width || y > s->height) return false;
    s->board[x * s->width + y] = c;
    return true;
}

bool sob_find_player(Sobokan *board) {
    char field;
    for (int i = 0; i < board->height; ++i) {
        for (int j = 0; j < board->width; ++j) {
            if ((field = sob_get_field_at(board, i, j)) == PLAYER
                || field == PLAYER_ON_DEST) {

                board->ppos[0] = i;
                board->ppos[1] = j;
                return true;
            }
        }
    }
    return false;
}

bool sob_count_crates(Sobokan *game_instance) {

    int crate_count = 0;

    for (int i = 0; i < game_instance->height; ++i) {
        for (int j = 0; j < game_instance->width; ++j) {
            switch (sob_get_field_at(game_instance, i, j)) {
                case DEST:
                    game_instance->crates_left++;
                    break;
                case CRATE:
                    crate_count++;
                    break;
                case PLAYER_ON_DEST:
                    game_instance->crates_left++;
                    break;
                default: break;
            };
        }
    }

    return (crate_count == game_instance->crates_left) ? true : false;
}

void sob_print_board(Sobokan *board) {
    printf("WIDTH: %d\nHEIGHT: %d\n", board->width, board->height);
    printf("PLAYER AT: %d,%d\n", board->ppos[0], board->ppos[1]);
    printf("CRATES_LEFT:%d\n\n", board->crates_left);

    for (int i = 0; i < board->height; ++i) {
        for (int j = 0; j < board->width; ++j) {
            if (board->ppos[0] == i && board->ppos[1] == j) {
                putchar('@');
            } else {
                putchar(sob_get_field_at(board, i, j));
            }
        }
        putchar('\n');
    }
}

void sob_fill_with_buffer(Sobokan *board, int buffer_size, char *buffer) {
    char c;
    int i = 0, row = 0, col = 0;

    while ((c = buffer[i++]) != '\0') {
        if (c == '\n') {
            row++; col = 0;
        } else { 
            sob_set_field_at(board, c, row, col++);
        }
    }
}

Sobokan *sob_init_from_file(const char *source) {
    Sobokan *board;
    int buf_size = 1000, i = 0;
    int max_row_width = 0, row_width = 0, col_count = 0;
    char *buffer = malloc(buf_size), c;
    FILE *src = fopen(source, "r");
    
    while ((c = fgetc(src)) != EOF) {
        if (c == '\n') {
            if (max_row_width < row_width) max_row_width = row_width;
            row_width = 0;
            col_count++;
        } else {
            row_width++;
        }

        if (i > buf_size) {
            buf_size += 200;
            buffer = realloc(buffer, buf_size);
        }

        buffer[i++] = c;
    }
    buffer[i] = '\n';

    fclose(src);
    board = sob_new_instance(col_count, max_row_width);
    sob_fill_with_buffer(board, buf_size, buffer);
    sob_find_player(board);
    sob_count_crates(board);
    free(buffer);

    return board;
}

bool sob_get_next_pos(const int direction, BoardCoordinates obj_pos, 
                      BoardCoordinates next_pos) {

    int dx = 0, dy = 0;

    switch (direction) {
        case MOVE_UP:
            --dy; break;
        case MOVE_DOWN:
            ++dy; break;
        case MOVE_LEFT:
            ++dx; break;
        case MOVE_RIGHT:
            --dx; break;
        default: return false;
    };
    
    next_pos[0] = obj_pos[0] + dx;
    next_pos[1] = obj_pos[1] + dy;

    return true;
}
/*
    switch (sob_get_field_at(game_instance, obj_pos[0], obj_pos[1])) {
        case PLAYER: break;
        case CRATE: break;
        case PLAYER_ON_DEST: break;
        case CRATE_ON_DEST: break;
        default: return false;
    };

    return false;
*/

bool sob_player_move_up(Sobokan *g_inst, const int direction) {

    BoardCoordinates next_pos;
    sob_get_next_pos(direction, g_inst->ppos, next_pos);

    switch (sob_get_field_at(g_inst, next_pos[0], next_pos[1])) {
        case WALL:  printf("WALL"); return false;
        default:    break;
    };

    g_inst->ppos[0] = next_pos[0];
    g_inst->ppos[1] = next_pos[1];

    return true;
}
