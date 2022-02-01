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

void sg_tiles_refresh(SokobanGame *game) {
    int size = game->data->width * game->data->height;
    GdkPixbuf *pixbuf;
    int asset_index;

    for (int i = 0; i < size; ++i) {
        asset_index = sg_field_type_to_asset_index(game->data->board[i]);
        pixbuf = game->assets[asset_index];
        gtk_image_set_from_pixbuf(GTK_IMAGE(game->tiles[i]), pixbuf);
    }
}

void sg_tiles_update(SokobanGame *game, int changed_fields[3]) {
    char buffer[30];
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
    sprintf(buffer, "crates left: %d", game->data->crates_left);
    gtk_label_set_text(GTK_LABEL(game->label_crates), buffer);
    game->data->moves++;
    sprintf(buffer, "moves: %d", game->data->moves);
    gtk_label_set_text(GTK_LABEL(game->label_moves), buffer);
    printf("%d %d %d\n", changed_fields[0], changed_fields[1], changed_fields[2]);
}

void _sg_time_label_update(gpointer data) {
    SokobanGame *game = (SokobanGame*)data;
    char buffer[30];
    sa_update_time(game->data);
    sprintf(buffer, "time: %lu", game->data->time_elapsed);
    gtk_label_set_text(GTK_LABEL(game->label_time), buffer);
}

void _sg_restart_game(GtkWidget *widget, gpointer data) {
    SokobanGame *game = (SokobanGame*)data;
    sa_copy_level(game->original, game->data);
    game->data->time_start = time(NULL);
    game->data->moves = 0;
    sg_tiles_refresh(game);
}

void _sg_abandon_game(GtkWidget *widget, gpointer data){
    GList *children, *i;
    GtkWidget *master_box = (GtkWidget*)data;

    gtk_container_foreach(GTK_CONTAINER(master_box), (GtkCallback)gtk_widget_destroy,
                          NULL);
}

SokobanGame *sg_sokoban_game_init(Sokoban *level) {
    SokobanGame *g = malloc(sizeof(SokobanGame));
    g->data = level;
    g->original = sokoban_init(level->height, level->width);
    if (!sa_copy_level(level, g->original)) return NULL;
    generate_assets(g->assets);
    g->tiles = sg_init_tiles(g->assets, level);
    g->label_crates = gtk_label_new(NULL);
    g->label_time = gtk_label_new(NULL);
    g->label_moves = gtk_label_new(NULL);
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
        case GDK_KEY_h:
            d = LEFT;
            break;
        case GDK_KEY_l:
            d = RIGHT;
            break;
        default: return;
    };

    if (move_player(game->data, d, changed_fields))
        sg_tiles_update(game, changed_fields);
}

void sg_init_game_window(GtkWidget *window, Sokoban *level) {

    SokobanGame *game = sg_sokoban_game_init(level);

    GtkWidget *box_master = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *box_controls = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *game_board = gtk_grid_new();
    GtkWidget *button_restart = gtk_button_new_with_label("RESTART");
    GtkWidget *button_save = gtk_button_new_with_label("SAVE & EXIT");
    GtkWidget *button_abandon = gtk_button_new_with_label("ABANDON");

    for (int i = 0; i < game->data->height; ++i) {
        for (int j = 0; j < game->data->width; ++j) {
            gtk_grid_attach(GTK_GRID(game_board), game->tiles[i + game->data->width * j],
            i, j, 1, 1);
        }
    }

    gtk_box_pack_start(GTK_BOX(box_controls), game->label_crates, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box_controls), game->label_time, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box_controls), game->label_moves, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box_controls), button_restart, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box_controls), button_save, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box_controls), button_abandon, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box_master), box_controls, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box_master), game_board, FALSE, TRUE, 10);

    gtk_container_add(GTK_CONTAINER(window), box_master);
    gtk_widget_show_all(window);

    g_signal_connect (G_OBJECT (window), "key_press_event",
                     G_CALLBACK (sg_handle_keypress), game); 
    g_signal_connect(G_OBJECT(button_restart), "clicked", 
                     G_CALLBACK(_sg_restart_game), game);                     
    g_signal_connect(G_OBJECT(button_abandon), "clicked", 
                     G_CALLBACK(_sg_abandon_game), box_master);                     
    g_timeout_add(1000, (GSourceFunc)_sg_time_label_update, (gpointer)game);
}