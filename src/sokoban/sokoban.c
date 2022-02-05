#include "sokoban.h"


Sokoban *sokoban_init(int width, int height) {
    Sokoban *new_instance = malloc(sizeof(Sokoban));

    new_instance->width = width;
    new_instance->height = height;
    new_instance->board = malloc(width * height);
    memset(new_instance->board, EMPTY, sizeof(char) * width * height);
    new_instance->player_x = -1;
    new_instance->player_y = -1;
    new_instance->crates_left = 0;
    new_instance->moves = 0;
    new_instance->time_start = time(NULL);
    new_instance->time_elapsed = 0;

    return new_instance;
}


Sokoban *sokoban_init_from_buffer(char *buffer) {    
    int x, y, offset = 0, row = 0;
    Sokoban *lvl;
    if (!parse_board(buffer, &x, &y)) return NULL;
    char *line = strtok(buffer, "\n"), c;
    lvl = sokoban_init(x, y);

    while (line != NULL) {
        while ((c = *line) != '\0') {
            if (c == PLAYER || c == PLAYER_ON_DEST) {
                lvl->player_x = offset - row * lvl->width;
                lvl->player_y = row;
            } else if (c == CRATE) lvl->crates_left++;
            lvl->board[offset++] = c;
            line++;
        }
        printf("%s\n", line);
        offset = sa_coordinate_to_index(lvl, 0, ++row);
        line = strtok(NULL, "\n");
    }
    return lvl;
}

bool sa_sokoban_free(Sokoban *level) {
    if (level == NULL) return false;
    free(level->board);
    free(level);
    return true;
}

void sokoban_print(Sokoban *s) {
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

bool get_delta(Direction d, int *dx, int *dy) {
    switch (d) {
        case UP: *dx = 0; *dy = 1; break;
        case DOWN: *dx = 0; *dy = -1; break;
        case LEFT: *dx = -1; *dy = 0; break;
        case RIGHT: *dx = 1; *dy = 0; break;
        default: return false;
    };
    return true;
}

bool is_in_bound(Sokoban *s, int x, int y) {
    return s->width > x && x >= 0 && s->height > y && y >= 0;
}

int swap(Sokoban *s, int x, int y, Direction d) {
    char *nf, *cf = board_get_field_at(s, x, y);
    int nx, ny, ret_val = 2;
    if (!get_delta(d, &nx, &ny)) return 0;
    nx = x + nx; ny = y + ny;
    if (*cf == EMPTY || *cf == DEST) return 1;
    if (!is_in_bound(s, nx, ny)) return 0;
    nf = board_get_field_at(s, nx, ny);

    switch (*cf) {
        case PLAYER:
            if (!(ret_val = swap(s, nx, ny, d))) return 0;
            printf("\n%d\n", ret_val);
            if (*nf == DEST) *nf = PLAYER_ON_DEST;
            else if (*nf == EMPTY) *nf = PLAYER;
            else return 0;
            *cf = EMPTY;
            break;
        case PLAYER_ON_DEST:
            if (!(ret_val = swap(s, nx, ny, d))) return 0;
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

// TODO: fix going out of bounds

bool move_player(Sokoban *s, Direction d, int changed_fields[3]) {
    int dx, dy;
    int cf;
    memset(changed_fields, -1, sizeof(int) * 3);
    if (!get_delta(d, &dx, &dy)) return false;
    if ((cf = swap(s, s->player_x, s->player_y, d))) {
        changed_fields[0] = s->player_x + s->player_y * s->width;
        s->player_x += dx;
        s->player_y += dy;
        changed_fields[1] = s->player_x + s->player_y * s->width;
        if (cf == 2) {
            changed_fields[2] = (s->player_x + dx) + (s->player_y + dy) * s->width;
            printf("three: %d %d %d\n", changed_fields[0], changed_fields[1], changed_fields[2]);
        } else
            printf("two: %d %d\n", changed_fields[0], changed_fields[1]);
        return true;
    }
    return false;
}

bool parse_board(char *lvl_buffer, int *x, int *y) {
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

char *board_get_field_at(Sokoban *s, int x, int y) {
    if (x > s->width || y > s->height) 
        return NULL;
    else 
        return &s->board[x + s->width * y];
}
