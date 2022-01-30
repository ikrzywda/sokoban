#include "sokoban_gui.h"

int sg_field_type_to_asset_index(char field) {
        switch (field) {
            case EMPTY:
                return ASSET_FLOOR;    
            case WALL:
                return ASSET_WALL;    
            case PLAYER:
                return ASSET_WORKER;    
            case CRATE:
                return ASSET_CRATE;    
            case DEST:
                return ASSET_DEST;    
            case CRATE_ON_DEST:
                return ASSET_CRATE_ON_DEST;    
            case PLAYER_ON_DEST:
                return ASSET_WORKER_ON_DEST;    
            default:
                break;
        }
    return ASSET_ERROR;
}

void generate_assets(GdkPixbuf *assets[ASSET_COUNT]) {
    for (int i = 0; i < ASSET_COUNT; ++i) {
        assets[i] = gdk_pixbuf_new_from_file_at_scale(kAssetPath[i], 
                                                        TILE_SIZE, TILE_SIZE,
                                                        TRUE, NULL);
    }
}

GtkWidget **sg_init_tiles(GdkPixbuf *assets[ASSET_COUNT], Sokoban *level) {
    int size = level->width * level->height;
    GtkWidget **tiles = malloc(sizeof(GtkWidget*) * size);
    for (int i = 0; i < size; ++i) {
        // refactor: assign ASSET_* to a variable - DRY
        switch (level->board[i]) {
            case EMPTY:
                tiles[i] = gtk_image_new_from_pixbuf(assets[ASSET_FLOOR]); 
                break;
            case WALL:
                tiles[i] = gtk_image_new_from_pixbuf(assets[ASSET_WALL]); 
                break;
            case PLAYER:
                tiles[i] = gtk_image_new_from_pixbuf(assets[ASSET_WORKER]); 
                break;
            case CRATE:
                tiles[i] = gtk_image_new_from_pixbuf(assets[ASSET_CRATE]); 
                break;
            case DEST:
                tiles[i] = gtk_image_new_from_pixbuf(assets[ASSET_DEST]); 
                break;
            case CRATE_ON_DEST:
                tiles[i] = gtk_image_new_from_pixbuf(assets[ASSET_CRATE_ON_DEST]); 
                break;
            case PLAYER_ON_DEST:
                tiles[i] = gtk_image_new_from_pixbuf(assets[ASSET_WORKER_ON_DEST]); 
                break;
            default:
                tiles[i] = gtk_image_new_from_pixbuf(assets[ASSET_ERROR]); 
                break;
        }
    }
    return tiles;
}

void sg_tiles_update(SokobanGame *game, int changed_fields[3]) {
    GdkPixbuf *pixbuf;
    int asset_index;
    char *board = game->data->board;
    for (int i = 0; i < 3; ++i) {
        if (changed_fields > 0) {
            asset_index = sg_field_type_to_asset_index(board[changed_fields[i]]);
            pixbuf = game->assets[asset_index];
            gtk_image_set_from_pixbuf(GTK_IMAGE(game->tiles[changed_fields[i]]), pixbuf);
        }
    }
}

SokobanGame *sg_sokoban_game_init(Sokoban *level) {
    SokobanGame *g = malloc(sizeof(SokobanGame));
    g->data = level;
    generate_assets(g->assets);
    g->tiles = sg_init_tiles(g->assets, level);
    return g;
}

void sg_handle_keypress(GtkWidget *window, GdkEventKey *event, gpointer data) {
    SokobanGame *game = (SokobanGame*)data;
    int changed_fields[3];
    Direction d;
    switch (event->keyval) {
        case GDK_KEY_j:
            d = UP;
            break;
        case GDK_KEY_k:
            d = DOWN;
            break;
        case GDK_KEY_l:
            d = LEFT;
            break;
        case GDK_KEY_h:
            d = RIGHT;
            break;
        default: return;
    };
    if (move_player(game->data, d, changed_fields))
        sg_tiles_update(game, changed_fields);
    sokoban_print(game->data);
}

void sg_init_game_window(GtkWidget *window, Sokoban *level) {
    SokobanGame *game = sg_sokoban_game_init(level);

    GtkWidget *grid = gtk_grid_new();
    GtkWidget *grid_master = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid_master);

    for (int i = 0; i < game->data->height; ++i) {
        for (int j = 0; j < game->data->width; ++j) {
            gtk_grid_attach(GTK_GRID(grid), game->tiles[i + game->data->width * j],
            i, j, 1, 1);
        }
    }

    gtk_grid_attach(GTK_GRID(grid), grid, 2, 2, 1, 1);
    gtk_widget_show_all(grid_master);
    g_signal_connect (G_OBJECT (window), "key_press_event",
                      G_CALLBACK (sg_handle_keypress), game); 
}