#ifndef SOKOBAN_GUI_H
#define SOKOBAN_GUI_H

#include "sokoban.h"
#include "constants.h"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct SokobanGame {
    Sokoban *data, *original;
    GdkPixbuf *assets[ASSET_COUNT];
    GtkWidget **tiles;
    GtkWidget *label_crates;
    GtkWidget *label_time;
    GtkWidget *label_moves;
} SokobanGame;

int sg_field_type_to_asset_index(char field);

void generate_assets(GdkPixbuf *assets[ASSET_COUNT]);
GtkWidget **sg_init_tiles(GdkPixbuf *assets[ASSET_COUNT], Sokoban *level);
void sg_print_tiles(SokobanGame *game);
void sg_tiles_update(SokobanGame *game, int changed_fields[3]);
void sg_tiles_refresh(SokobanGame *game);

void _sg_time_label_update(gpointer data);
void _sg_restart_game(GtkWidget *widget, gpointer data);
void _sg_abandon_game(GtkWidget *widget, gpointer data);

SokobanGame *sg_sokoban_game_init(Sokoban *level);
void sg_sokoban_game_update(Sokoban *level, SokobanGame *game);

void sg_handle_keypress(GtkWidget *window, GdkEventKey *event, gpointer data);

void sg_init_game_window(GtkWidget *window, Sokoban *level);

#endif
