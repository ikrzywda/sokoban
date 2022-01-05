#include "sokboard.h"

SokBoard *sokboard_init(int x, int y) {
    SokBoard *board = malloc(sizeof(SokBoard));
    board->width = x;
    board->height = y;
    board->board = malloc(x * y);

    return board;
}

char sokboard_get_field_at(SokBoard *s, int x, int y) {
    if (x > s->width || y > s->height) return -1;
    else return s->board[x * s->width + y];
}

bool sokboard_set_field_at(SokBoard *s, char c, int x, int y) {
    if (x > s->width || y > s->height) return false;
    s->board[x * s->width + y] = c;
    return true;
}

void sokboard_print(SokBoard *board) {
    printf("WIDTH: %d\nHEIGHT: %d\n\n", board->width, board->height);

    for (int i = 0; i < board->height; ++i) {
        for (int j = 0; j < board->width; ++j) {
            putchar(sokboard_get_field_at(board, i, j));
        }
        putchar('\n');
    }
}

void sokboard_fill_with_buffer(SokBoard *board, int buffer_size, char *buffer) {
    char c;
    int i = 0, row = 0, col = 0;

    while ((c = buffer[i++]) != '\0') {
        if (c == '\n') {
            row++; col = 0;
        } else { 
            sokboard_set_field_at(board, c, row, col++);
        }
    }
}

SokBoard *sokboard_read_from_file(const char *source) {
    SokBoard *board;
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
    board = sokboard_init(col_count, max_row_width);
    sokboard_fill_with_buffer(board, buf_size, buffer);
    free(buffer);

    return board;
}
