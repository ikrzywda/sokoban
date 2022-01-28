#include "sokoban/sokboard.h"

#include <stdio.h>

int main() {
    char src[50] = "test_board.txt";
    char buffer[1000];
    FILE *f = fopen(src, "r");
    char c;
    int i = 0;
    while ((c = fgetc(f)) != EOF) buffer[i++] = c;
    buffer[i] = '\0';
    Sokoban *s = sokoban_init_from_buffer(buffer);
    sokoban_print(s);
    while ((c = getchar()) != '0') { 
        switch (c) {
            case 'h':
                move_player(s, LEFT);
                break;
            case 'j':
                move_player(s, DOWN);
                break;
            case 'k':
                move_player(s, UP);
                break;
            case 'l':
                move_player(s, RIGHT);
                break;
        };
        sokoban_print(s);
        if (!s->crates_left) {
            printf("SUCCESS!\n");
            return 0;
        }
    }
    return 0;
}