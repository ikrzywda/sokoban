#include "sokoban/sokboard.h"

#include <stdio.h>

int main() {
    char src[50] = "test_board.txt";
    // scanf("%s", src);
    Sobokan *s = sob_init_from_file(src);
    char c;
    while ((c = getchar()) != '0') { 
        switch (c) {
            case 'h':
                sob_player_move_up(s, MOVE_LEFT);
                break;
            case 'j':
                sob_player_move_up(s, MOVE_DOWN);
                break;
            case 'k':
                sob_player_move_up(s, MOVE_UP);
                break;
            case 'l':
                sob_player_move_up(s, MOVE_RIGHT);
                break;
        };
        sob_print_board(s);
    }
}
