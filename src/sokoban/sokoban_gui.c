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

void sg_print_tiles(SokobanGame *game) {
    int size = game->data->width * game->data->height;

    for (int i = 0; i < size; ++i) {
        printf("%d: %p\n", i, game->tiles[i]);
    }
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
    int size = game->data->width * game->data->height;
    char buffer[30];
    GdkPixbuf *pixbuf;
    int asset_index;
    char *board = game->data->board;
    for (int i = 0; i < 3; ++i) {
        if (changed_fields[i] >= 0 && changed_fields[i] < size) {
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
    GList *children;
    GtkWidget *master_box = (GtkWidget*)data;
    gtk_container_foreach(GTK_CONTAINER(master_box), (GtkCallback)gtk_widget_destroy,
                          NULL);
}

SokobanGame *sg_sokoban_game_init(Sokoban *level) {
    SokobanGame *g = malloc(sizeof(SokobanGame));
    g->data = level;
    g->original = sa_sokoban_init(level->height, level->width, level->level_index);
    if (!sa_copy_level(level, g->original)) return NULL;
    generate_assets(g->assets);
    g->tiles = sg_init_tiles(g->assets, level);
    g->label_crates = gtk_label_new(NULL);
    g->label_time = gtk_label_new(NULL);
    g->label_moves = gtk_label_new(NULL);
    return g;
}

void sg_sokoban_game_update(Sokoban *level, SokobanGame *game) {
    if (level == NULL || game == NULL) return;
    sa_sokoban_free(game->data);
    game->data = level;
    sg_tiles_refresh(game);
}


void sg_handle_keypress(GtkWidget *window, GdkEventKey *event, gpointer data) {
    SokobanGame *game = (SokobanGame*)data;
    Move changed_fields;
    Direction d;
    bool field_changed = false;
    switch (event->keyval) {
        case GDK_KEY_j:
            d = UP;
            break;
        case GDK_KEY_Down:
            d = UP;
            break;
        case GDK_KEY_k:
            d = DOWN;
            break;
        case GDK_KEY_Up:
            d = DOWN;
            break;
        case GDK_KEY_h:
            d = LEFT;
            break;
        case GDK_KEY_Right:
            d = RIGHT;
            break;
        case GDK_KEY_l:
            d = RIGHT;
            break;
        case GDK_KEY_Left:
            d = LEFT;
            break;
        case GDK_KEY_u:
            field_changed = sa_revert_move(game->data, &d);
            if (!field_changed) return;
            break;
        default: return;
    };

    if (sa_move_player(game->data, d, changed_fields, field_changed)) {
        sg_tiles_update(game, changed_fields);
    }
}
