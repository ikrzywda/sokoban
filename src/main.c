#include "sokoban/sokboard.h"

#include <stdio.h>

int main() {
    char src[50];
    scanf("%s", src);
    SokBoard *s = sokboard_read_from_file(src);
    sokboard_print(s);
}
