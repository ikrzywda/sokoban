#ifndef GAME_H
#define GAME_H

#include "constants.h"
#include "sokoban.h"
#include "sokoban_gui.h"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// master struct containing the game and signals for handling 
// all actions - provides all data to mutate the master window
// use gtkstack here? 

typedef enum GSignal {
    MAIN_MENU = 0,
    GAME = 1,
    END_SCREEN = 2,
} GSignal;

typedef struct GLevelSelectData {
    int level_path_index;
    int *sig;
    SokobanGame *game_instance;
} GLevelSelectData;

typedef struct GManager {
    SokobanGame *game_instance;
    int gui_state;
    GtkWidget *window_master;
    GtkWidget *stack_master;
    GtkWidget *box_game;
    GtkWidget *box_menu;
    GtkWidget *box_endscreen;
} GManager;

GLevelSelectData *gm_level_select_data_init(int index, 
                                            SokobanGame *ginst, int *sig);

void _gm_select_level(GtkWidget *widget, gpointer data);
void _gm_update_widgets(gpointer *data);

GtkWidget *gm_menu_init(int *callback, SokobanGame *game_instance);
GtkWidget *gm_endscreen_init(int *callback);

GManager *gm_game_manager_init(GtkWidget *window);

#endif 
