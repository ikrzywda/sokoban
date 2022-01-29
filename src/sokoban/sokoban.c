#include "sokoban.h"


Sokoban *sokoban_init(int width, int height) {
    Sokoban *new_instance = malloc(sizeof(Sokoban));

    new_instance->width = width;
    new_instance->height = height;
    new_instance->board = malloc(width * height);
    new_instance->player_x = -1;
    new_instance->player_y = -1;
    new_instance->crates_left = 0;
    new_instance->moves = 0;

    return new_instance;
}


Sokoban *sokoban_init_from_buffer(char *buffer) {    
    int x, y;
    Sokoban *new_instance;
    if (!parse_board(buffer, &x, &y)) return NULL;
    new_instance = sokoban_init(x, y);
    char c;

    x = y = 0;
    while ((c = *buffer) != '\0') {
        if (c == '\n') {
            for (; x < new_instance->width; ++x) 
                new_instance->board[x + y * new_instance->width] = ' ';
            x = 0; ++y;
        } else {
            if (c == CRATE) new_instance->crates_left++;
            else if (c == PLAYER_ON_DEST || c == PLAYER) {
                new_instance->player_x = x - 1;
                new_instance->player_y = y;
            }
            new_instance->board[x + y * new_instance->width] = c;
        }
        ++x;
        buffer++;
    }

    return new_instance;
}

void sokoban_print(Sokoban *s) {
    printf("width: %d, height: %d\n", s->width, s->height);
    printf("player: (%d, %d)\n", s->player_x, s->player_y);
    printf("crates left: %d\n", s->crates_left);
    for (int i = 0; i < s->height; ++i) {
        for (int j = 0; j < s->width; ++j) {
            putchar(s->board[j + i * s->width]);
        }
        putchar('\n');
    }
    putchar('\n');
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

bool swap(Sokoban *s, int x, int y, Direction d) {
    int nx, ny;
    if (!get_delta(d, &nx, &ny)) return false;
    nx = x + nx; ny = y + ny;
    if (!is_in_bound(s, nx, ny)) return false;
    char *cf = board_get_field_at(s, x, y),
         *nf = board_get_field_at(s, nx, ny);

    switch (*cf) {
        case PLAYER:
            if (!swap(s, nx, ny, d)) return false;
            if (*nf == DEST) *nf = PLAYER_ON_DEST;
            else if (*nf == EMPTY) *nf = PLAYER;
            else return false;
            *cf = EMPTY;
            break;
        case PLAYER_ON_DEST:
            if (!swap(s, nx, ny, d)) return false;
            if (*nf == DEST) *nf = PLAYER_ON_DEST;
            else if (*nf == EMPTY) *nf = PLAYER;
            else return false;
            *cf = DEST;
            break;
        case CRATE:
            if (*nf == CRATE || *nf == CRATE_ON_DEST) return false;
            if (*nf == DEST) { 
                *nf = CRATE_ON_DEST;
                s->crates_left--;
            } else if (*nf == EMPTY) *nf = CRATE; 
            else return false;
            *cf = EMPTY;
            break;
        case CRATE_ON_DEST:
            if (*nf == CRATE || *nf == CRATE_ON_DEST || *nf == WALL) return false;
            if (*nf == DEST) { 
                *nf = CRATE_ON_DEST;
                s->crates_left--;
            } else if (*nf == EMPTY) *nf = CRATE; 
            else return false;
            *cf = DEST;
            s->crates_left++;
            break;
        case WALL: return false;
        case EMPTY: return true;
        default: return true;
    };

    return true;
}

bool move_player(Sokoban *s, Direction d, int changed_fields[3]) {
    int dx, dy;
    if (!get_delta(d, &dx, &dy)) return false;
    if (swap(s, s->player_x, s->player_y, d)) {
        changed_fields[0] = s->player_x + s->player_y * s->width;
        s->player_x += dx;
        s->player_y += dy;
        changed_fields[1] = s->player_x + s->player_y * s->width;
        changed_fields[2] = (s->player_x + dx) + (s->player_y + dy) * s->width;
        return true;
    }
    return false;
}

bool parse_board(char *lvl_buffer, int *x, int *y) {
    int height = 0, width = 1;
    int max_width = 0;
    char c;

    while ((c = *lvl_buffer) != '\0') {
        if (c == '\n') {
            height++;
            max_width = width > max_width ? width : max_width;
            width = 1;
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