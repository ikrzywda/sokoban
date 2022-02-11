#include "sokoban.h"


Sokoban *sa_sokoban_init(int width, int height, int level_index) {
    Sokoban *new_instance = malloc(sizeof(Sokoban));

    new_instance->width = width;
    new_instance->height = height;
    new_instance->board = malloc(width * height);
    new_instance->level_index = level_index;
    memset(new_instance->board, EMPTY, sizeof(char) * width * height);
    new_instance->player_x = -1;
    new_instance->player_y = -1;
    new_instance->crates_left = 0;
    new_instance->moves = 0;
    new_instance->time_start = time(NULL);
    new_instance->time_elapsed = 0;
    new_instance->best_time = -1;
    new_instance->best_moves = -1;
    new_instance->move_history = mv_stack_init();

    return new_instance;
}

// load buffer in this function, make designated helper
Sokoban *sa_sokoban_init_from_buffer(int level_index) {    
    int x, y, offset = 0, row = 0;
    Sokoban *lvl;
    char path[40], *buffer, *line, c;
    if (sa_save_exists(level_index)) {
        sprintf(path, "saves/%d.txt", level_index);
    } else {
        sprintf(path, "levels/%d.txt", level_index);
    }
    buffer = ut_file_buffer(path);

    if (!sa_parse_board(buffer, &x, &y)) return NULL;
    line = strtok(buffer, "\n");
    lvl = sa_sokoban_init(x, y, level_index);
    c = *line;
    char *bgn = line;

    while (line != NULL) {
        while ((c = *line) != '\0') {
            if (c == PLAYER || c == PLAYER_ON_DEST) {
                lvl->player_x = offset - row * lvl->width;
                lvl->player_y = row;
            } else if (c == CRATE) lvl->crates_left++;

            lvl->board[offset] = c;
            printf("%d\t%c\t%d\t%ld\n", offset, lvl->board[offset], c, line-bgn);
            line++; offset++;
        }
        ++row;
        offset = sa_coordinate_to_index(lvl, 0, row);
        line = strtok(NULL, "\n");
    }
    sa_read_metadata(lvl);
    free(buffer);
    return lvl;
}

bool sa_sokoban_free(Sokoban *level) {
    if (level == NULL) return false;
    free(level->board);
    free(level);
    return true;
}

void sa_sokoban_print(Sokoban *s) {
    printf("width: %d, height: %d\n", s->width, s->height);
    printf("player: (%d, %d)\n", s->player_x, s->player_y);
    printf("crates left: %d\n", s->crates_left);
    for (int i = 0; i < s->height; ++i) {
        for (int j = 0; j < s->width; ++j) {
            if (s->board[sa_coordinate_to_index(s, j, i)] == EMPTY) putchar('0');
            else
            putchar(s->board[sa_coordinate_to_index(s, j, i)]);
        }
        putchar('\n');
    }
    putchar('\n');
}

int sa_coordinate_to_index(Sokoban *level, int x, int y) {
    return x + level->width * y;
}

void sa_update_time(Sokoban *level) {
    time_t ct = level->time_start;
    level->time_elapsed = time(NULL) - ct;
}

bool sa_copy_level(Sokoban *level, Sokoban *target) {
    if (level == NULL) return NULL;
    memcpy(target->board, level->board, level->width * level->height);
    target->crates_left = level->crates_left;
    target->height = level->height;
    target->width = level->width;
    target->player_x = level->player_x;
    target->player_y = level->player_y;
    target->time_elapsed = level->time_elapsed;
    target->time_start = level->time_start;
    return true;
}

bool sa_get_delta(Direction d, int *dx, int *dy) {
    switch (d) {
        case UP: *dx = 0; *dy = 1; break;
        case DOWN: *dx = 0; *dy = -1; break;
        case LEFT: *dx = -1; *dy = 0; break;
        case RIGHT: *dx = 1; *dy = 0; break;
        default: return false;
    };
    return true;
}

bool sa_is_in_bound(Sokoban *s, int x, int y) {
    return s->width > x && x >= 0 && s->height > y && y >= 0;
}

int sa_swap(Sokoban *s, int x, int y, Direction d) {
    char *nf, *cf = sa_board_get_field_at(s, x, y);
    int nx, ny;
    if (!sa_get_delta(d, &nx, &ny)) return 0;
    nx = x + nx; ny = y + ny;
    if (*cf == EMPTY || *cf == DEST) return 1;
    if (!sa_is_in_bound(s, nx, ny)) return 0;
    nf = sa_board_get_field_at(s, nx, ny);

    switch (*cf) {
        case PLAYER:
            if (!sa_swap(s, nx, ny, d)) return 0;
            if (*nf == DEST) *nf = PLAYER_ON_DEST;
            else if (*nf == EMPTY) *nf = PLAYER;
            else return 0;
            *cf = EMPTY;
            break;
        case PLAYER_ON_DEST:
            if (!sa_swap(s, nx, ny, d)) return 0;
            if (*nf == DEST) *nf = PLAYER_ON_DEST;
            else if (*nf == EMPTY) *nf = PLAYER;
            else return 0;
            *cf = DEST;
            break;
        case CRATE:
            if (*nf == CRATE || *nf == CRATE_ON_DEST) return 0;
            if (*nf == DEST) { 
                *nf = CRATE_ON_DEST;
                s->crates_left--;
            } else if (*nf == EMPTY) *nf = CRATE; 
            else return false;
            *cf = EMPTY;
            break;
        case CRATE_ON_DEST:
            if (*nf == CRATE || *nf == CRATE_ON_DEST || *nf == WALL) return 0;
            if (*nf == DEST) { 
                *nf = CRATE_ON_DEST;
                s->crates_left--;
            } else if (*nf == EMPTY) *nf = CRATE; 
            else return 0;
            *cf = DEST;
            s->crates_left++;
            break;
        case WALL: return 0;
        default: break;
    };
    return 2;
}

bool sa_move_player(Sokoban *s, Direction d, Move changed_fields, bool revert) {
    int dx, dy, cf;
    if (!sa_get_delta(d, &dx, &dy)) return false;
    if (!revert) mv_stack_push(s->move_history, d);
    memset(changed_fields, -1, sizeof(int) * 3);
    if ((cf = sa_swap(s, s->player_x, s->player_y, d))) {
        changed_fields[0] = s->player_x + s->player_y * s->width;
        s->player_x += dx;
        s->player_y += dy;
        changed_fields[1] = s->player_x + s->player_y * s->width;
        changed_fields[2] = (s->player_x + dx) + (s->player_y + dy) * s->width;
        return true;
    }
    return false;
}

bool sa_revert_move(Sokoban *s, int *d) {
    if (s == NULL) return false;
    if (mv_stack_pop(s->move_history, d)) {
        if (*d == LEFT || *d == RIGHT) *d = (*d == LEFT) ? RIGHT : LEFT;
        if (*d == UP || *d == DOWN) *d = (*d == UP) ? DOWN : UP;
        return true;
    }
    return false;
}

bool sa_parse_board(char *lvl_buffer, int *x, int *y) {
    int height = 0, width = 0;
    int max_width = 0;
    char c;

    while ((c = *lvl_buffer) != '\0') {
        if (c == '\n') {
            height++;
            max_width = width > max_width ? width : max_width;
            width = 0;
        } else if (c == EMPTY || c == WALL || c == PLAYER
                || c == CRATE || c == DEST || c == CRATE_ON_DEST
                || c == PLAYER_ON_DEST) {
            width++;
        } else return false;

        lvl_buffer++;
    }

    if (!max_width) return false;

    *x = max_width;
    *y = height;

    return true;
}

char *sa_board_get_field_at(Sokoban *s, int x, int y) {
    if (x > s->width || y > s->height) 
        return NULL;
    else 
        return &s->board[x + s->width * y];
}

bool sa_is_new_best_moves(Sokoban *level) {
    int current = level->moves;
    int best = level->best_moves < 0 ? level->moves : level->best_moves;
    if (current <= best) {
        level->best_moves = current;
        return true;
    }
    return false;
}

bool sa_is_new_best_time(Sokoban *level) {
    int current = level->time_elapsed;
    int best = level->best_time < 0 ? level->time_elapsed : level->best_time;
    if (current <= best) {
        level->best_time = current;
        return true;
    }
    return false;
}

bool sa_save_exists(int level_index) {
    char path[30];
    sprintf(path, "saves/%d.txt", level_index);
    return ut_file_exists(path);
}

void sa_read_metadata(Sokoban *level) {
    char path[40], buffer[100], c, *datapoint;
    int i = 0;
    sprintf(path, "saves/%d_meta.txt", level->level_index);
    
    FILE *f = fopen(path, "r");

    if (f) {
        while ((c = fgetc(f)) != EOF) buffer[i++] = c;
        buffer[i] = '\0';
    } else {
        return;
    }

    datapoint = strtok(buffer, ",");
    sscanf(datapoint, "%d", &level->best_time);
    datapoint = strtok(buffer, ",");
    sscanf(datapoint, "%d", &level->best_moves);
    datapoint = strtok(buffer, ",");
    sscanf(datapoint, "%ld", &level->time_elapsed);
    datapoint = strtok(buffer, ",");
    sscanf(datapoint, "%d", &level->moves);
}

void sa_save_level(Sokoban *level) {
    sa_level_progress_to_file(level);
    sa_level_meta_to_file(level);
}

void sa_level_progress_to_file(Sokoban *level) {
    char target[40];
    sprintf(target, "saves/%d.txt", level->level_index);
    FILE *tgt = fopen(target, "r");
    char *board = level->board;
    if (tgt) {
        fclose(tgt);
        remove(target);
    }
    tgt = fopen(target, "w");

    for (int i = 0; i < level->height; ++i) {
        for (int j = 0; j < level->width; ++j) {
            if (board[sa_coordinate_to_index(level, j, i)] == EMPTY) 
                fputc(' ', tgt);
            else
                fputc(board[sa_coordinate_to_index(level, j, i)], tgt);
        }
        fputc('\n', tgt);
    }
    fclose(tgt);
}

void sa_level_meta_to_file(Sokoban *level) {
    char target[40];
    sprintf(target, "saves/%d_meta.txt", level->level_index);
    FILE *tgt = fopen(target, "r");
    if (tgt) {
        fclose(tgt);
        remove(target);
    }
    tgt = fopen(target, "w");
    
    fprintf(tgt, "%d,%d,%ld,%d\n", level->best_time, level->best_moves, 
            level->time_elapsed, level->moves);
    fclose(tgt);
}
