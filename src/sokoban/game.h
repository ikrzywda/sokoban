#ifndef GAME_H
#define GAME_H

#include "constants.h"
#include "sokoban.h"
#include "sokoban_gui.h"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum GSignal {
    MAIN_MENU = 0,
    GAME = 1,
    END_SCREEN = 2,
} GSignal;

typedef struct GManager {
    SokobanGame *game_instance;
    int gui_state;
    bool box_game_empty;
    GtkWidget *window_master;
    GtkWidget *stack_master;
    GtkWidget *box_game;
    GtkWidget *box_menu;
    GtkWidget *box_endscreen;
} GManager;

typedef struct GLevelSelectData {
    int level_path_index;
    GManager *gm;
} GLevelSelectData;

void gm_clear_box(GtkWidget *widget);
GLevelSelectData *gm_level_select_data_init(int index, GManager *gm);

void _gm_select_level(GtkWidget *widget, gpointer data);
void _gm_return_to_menu(GtkWidget *widget, gpointer data);
void _gm_update_widgets(gpointer *data);
void _gm_abandon_game(GtkWidget *widget, gpointer data);

GtkWidget *gm_menu_init(GManager *gm);
GtkWidget *gm_endscreen_init(GManager *gm);
void gm_game_window(SokobanGame *game, GManager *gm);

GManager *gm_game_manager_init(GtkWidget *window);

#endif 
