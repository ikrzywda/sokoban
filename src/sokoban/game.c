#include "game.h"

GLevelSelectData *gm_level_select_data_init(int index, 
                                            SokobanGame *ginst, int *sig) {
    GLevelSelectData *new = malloc(sizeof(GLevelSelectData));
    new->level_path_index = index;
    new->game_instance = ginst;
    new->sig = sig;

    return new;
}

void _gm_select_level(GtkWidget *widget, gpointer data) {
    GLevelSelectData *blueprint = (GLevelSelectData*)data;

    Sokoban *s;
    FILE *f;
    char src[50], buffer[1000], c;
    int i = 0;

    sprintf(src, "levels/%d.txt", blueprint->level_path_index);
    f = fopen(src, "r");
    printf(src);
    

    if (f) {
        while ((c = fgetc(f)) != EOF) buffer[i++] = c;
        buffer[i] = '\0';
        s = sokoban_init_from_buffer(buffer);
    }
    
    blueprint->game_instance = sg_sokoban_game_init(s);
    *(blueprint->sig) = END_SCREEN;
}


void _gm_update_widgets(gpointer *data) {
    GManager *gm = (GManager*)data;
    GtkStack *master = GTK_STACK(gm->stack_master);
    gtk_stack_set_visible_child_name(master, kMasterStackIDs[gm->gui_state]);
    gtk_widget_show_all(gm->window_master);
}

GtkWidget *gm_menu_init(int *callback, SokobanGame *game_instance) {

    GtkWidget *lvl_selectors[LEVEL_COUNT];
    GLevelSelectData *lvl_selectors_data[LEVEL_COUNT];
    GtkWidget *box_master = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *lvl_selector_grid = gtk_grid_new();
    char buffer[20];

    gtk_grid_set_row_spacing(GTK_GRID(lvl_selector_grid), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(lvl_selector_grid), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(lvl_selector_grid), 5);
    gtk_grid_set_column_homogeneous(GTK_GRID(lvl_selector_grid), TRUE);

    for (int i = 0; i < LEVEL_COUNT; ++i) {
        sprintf(buffer, "%d", i);
        lvl_selectors[i] = gtk_button_new_with_label(buffer);        
        lvl_selectors_data[i] = gm_level_select_data_init(i, game_instance, 
                callback);
        g_signal_connect(lvl_selectors[i], "clicked",
                G_CALLBACK(_gm_select_level), lvl_selectors_data[i]);
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) {
            gtk_grid_attach(GTK_GRID(lvl_selector_grid), lvl_selectors[j + i * 4], 
                            i, j, 2, 2);
        }
    }

    gtk_box_pack_start(GTK_BOX(box_master), lvl_selector_grid, FALSE, TRUE, 10);

    return box_master;
}

GtkWidget *gm_endscreen_init(int *callback) {
    GtkWidget *box_master = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *label = gtk_label_new_with_mnemonic("hello");
    gtk_box_pack_start(GTK_BOX(box_master), label, FALSE, TRUE, 10);
    return box_master;
}

GManager *gm_game_manager_init(GtkWidget *window) {
    GManager *new = malloc(sizeof(GManager)); 
    new->game_instance = NULL;
    new->gui_state = MAIN_MENU;
    new->window_master = window;
    new->stack_master = gtk_stack_new();
    new->box_game = NULL;
    new->box_menu = gm_menu_init(&(new->gui_state), new->game_instance);
    new->box_endscreen = gm_endscreen_init(&(new->gui_state));

    gtk_stack_add_named(GTK_STACK(new->stack_master), new->box_endscreen,
            kMasterStackIDs[END_SCREEN]);
    
    gtk_stack_add_named(GTK_STACK(new->stack_master), new->box_menu,
            kMasterStackIDs[MAIN_MENU]);

    gtk_stack_add_named(GTK_STACK(new->stack_master), new->box_game,
            kMasterStackIDs[GAME]);

    g_timeout_add(100, (GSourceFunc)_gm_update_widgets, (gpointer)new);

    return new;
}
