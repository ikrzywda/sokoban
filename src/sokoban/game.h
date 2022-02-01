#ifndef GAME_H
#define GAME_H

#include "sokoban.h"
#include "sokoban_gui.h"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void mn_menu(GtkWidget *window);
void mn_endscreen(GtkWidget *window, SokobanGame *game);

#endif 