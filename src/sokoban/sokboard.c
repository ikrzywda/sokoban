#include "sokboard.h"

SobInstance *sob_new_instance(int x, int y) {
    SobInstance *new_instance = malloc(sizeof(SobInstance));
    new_instance->width = x;
    new_instance->height = y;
    new_instance->board = malloc(x * y);
    new_instance->crates_left = 0;
    new_instance->moves = 0;

    return new_instance;
}

char sob_get_field_at(SobInstance *s, int x, int y) {
    if (x > s->width || y > s->height) 
        return -1;
    else 
        return s->board[x * s->width + y];
}

bool sob_set_field_at(SobInstance *s, char c, int x, int y) {
    if (x > s->width || y > s->height) return false;
    s->board[x * s->width + y] = c;
    return true;
}

bool sob_find_player(SobInstance *g_inst) {
    char field;
    for (int i = 0; i < g_inst->height; ++i) {
        for (int j = 0; j < g_inst->width; ++j) {
            if ((field = sob_get_field_at(g_inst, i, j)) == PLAYER
                || field == PLAYER_ON_DEST) {

                g_inst->ppos[0] = i;
                g_inst->ppos[1] = j;
                return true;
            }
        }
    }
    return false;
}

bool sob_count_crates(SobInstance *game_instance) {

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

void sob_print_g_inst(SobInstance *g_inst) {
    printf("WIDTH: %d\nHEIGHT: %d\n", g_inst->width, g_inst->height);
    printf("PLAYER AT: %d,%d\n", g_inst->ppos[0], g_inst->ppos[1]);
    printf("CRATES_LEFT:%d\n\n", g_inst->crates_left);

    for (int i = 0; i < g_inst->height; ++i) {
        for (int j = 0; j < g_inst->width; ++j) {
            if (g_inst->ppos[0] == i && g_inst->ppos[1] == j) {
                putchar('@');
            } else {
                putchar(sob_get_field_at(g_inst, i, j));
            }
        }
        putchar('\n');
    }
}

void sob_fill_with_buffer(SobInstance *g_inst, int buffer_size, char *buffer) {
    char c;
    int i = 0, row = 0, col = 0;

    while ((c = buffer[i++]) != '\0') {
        if (c == '\n') {
            row++; col = 0;
        } else { 
            sob_set_field_at(g_inst, c, row, col++);
        }
    }
}

SobInstance *sob_init_from_file(const char *source) {
    SobInstance *g_inst;
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
    g_inst = sob_new_instance(col_count, max_row_width);
    sob_fill_with_buffer(g_inst, buf_size, buffer);
    sob_find_player(g_inst);
    sob_count_crates(g_inst);
    free(buffer);

    return g_inst;
}

SobPos *next_pos(SobInstance *g_inst, SobPos pos, SobDirection dir) {
    return NULL;
}

bool sob_move_player(SobInstance *g_inst, SobDirection dir) {
    SobPos next_pos = sob_get_next_pos(g_inst->ppos, dir);
    char field;
    if ((field = sob_get_field_at(g_inst, next_pos[0], next_pos[1])) == CRATE
        || field == CRATE_ON_DEST) {
            if (!sob_move_crate(g_inst, next_pos, dir)) return false;
        } else if (field == WALL) {
            return false;
        }
        
    g_inst->ppos[0] = next_pos[0];
    g_inst->ppos[1] = next_pos[1];

    return true;
}

bool sob_move_crate(SobInstance *g_inst, SobPos pos, SobDirection dir) {
    SobPos next_pos = sob_get_next_pos(g_inst->ppos, dir);
    char field;
    if ((field = sob_get_field_at(g_inst, next_pos[0], next_pos[1])) == CRATE
        || field == WALL || c == CRATE_ON_DEST) {
            return false;
        } else if (field == DEST) {
            g_inst->crates_left--;
        }

    if (sob_get_field_at(g_inst, pos[0], pos[1]) == DEST)
        g_inst->crates_left++;

    g_inst->ppos[0] = next_pos[0];
    g_inst->ppos[1] = next_pos[1];

    return true;
}