#include "sokoban/sokboard.h"

#include <stdio.h>

int main() {
    char src[50] = "test_board.txt";
    // scanf("%s", src);
    SobInstance *s = sob_init_from_file(src);
    char c;
    while ((c = getchar()) != '0') { 
        switch (c) {
            case 'h':
                sob_move_player(s, LEFT);
                break;
            case 'j':
                sob_move_player(s, DOWN);
                break;
            case 'k':
                sob_move_player(s, UP);
                break;
            case 'l':
                sob_move_player(s, RIGHT);
                break;
        };
        sob_print_g_inst(s);
    }
}
