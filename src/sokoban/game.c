#include "game.h"

void gm_clear_box(GtkWidget *widget){
    GList *children, *i;
    GtkWidget *master_box = widget;
    gtk_container_foreach(GTK_CONTAINER(master_box), (GtkCallback)gtk_widget_destroy,
                          NULL);
}

GLevelSelectData *gm_level_select_data_init(int index, GManager *gm) {

    GLevelSelectData *new = malloc(sizeof(GLevelSelectData));
    new->level_path_index = index;
    new->gm = gm;

    return new;
}

void _gm_select_level(GtkWidget *widget, gpointer data) {
    GLevelSelectData *blueprint = (GLevelSelectData*)data;
    GManager *gm = blueprint->gm;

    Sokoban *s;
    char src[50], buffer[1000], c;
    int i = 0, index = blueprint->level_path_index;

    s = sa_sokoban_init_from_buffer(index);
   
    if (gm->box_game_empty) {
        gm->game_instance = sg_sokoban_game_init(s);
        gm_game_window(gm->game_instance, gm);

        g_signal_connect (G_OBJECT (gm->window_master), "key_press_event",
                        G_CALLBACK (sg_handle_keypress), gm->game_instance); 
        gm->box_game_empty = false;
    } else {
        sg_sokoban_game_update(s, gm->game_instance);
    }
    gm->gui_state = GAME;
}

void _gm_return_to_menu(GtkWidget *widget, gpointer data) {
    GManager *gm = (GManager*)data;
    gm->gui_state = MAIN_MENU;
}

void _gm_update_widgets(gpointer *data) {
    GManager *gm = (GManager*)data;
    GtkStack *master = GTK_STACK(gm->stack_master);
    char buffer[20];
    if (gm->gui_state == GAME && !gm->game_instance->data->crates_left) {
        sprintf(buffer, "saves/%d.txt", gm->game_instance->data->level_index);
        ut_remove_file(buffer);
        gm_game_over_endscreen(gm);
        gm->gui_state = END_SCREEN;
    }
    gtk_stack_set_visible_child_name(master, kMasterStackIDs[gm->gui_state]);
    gtk_widget_show_all(gm->window_master);
}

void _gm_abandon_game(GtkWidget *widget, gpointer data) {
    GManager *gm = (GManager*)data;
    gm_exit_endscreen(gm, true);
    gm->gui_state = END_SCREEN;
}

void _gm_save_game(GtkWidget *widget, gpointer data) {
    GManager *gm = (GManager*)data;
    char save_path[40];
    sprintf(save_path, "saves/%d.txt", gm->game_instance->data->level_index);
    sa_save_level(gm->game_instance->data);
    gm_exit_endscreen(gm, false);
    gm->gui_state = END_SCREEN;
}

void gm_game_over_endscreen(GManager *gm) {
    gm_clear_box(gm->box_endscreen);
    GtkWidget *box = gm->box_endscreen;
    char buffer[40];
    GtkWidget *success = gtk_image_new_from_file("assets/sokoban_success.png");
    GtkWidget *button_return_to_menu = gtk_button_new_with_label("MAIN MENU");

    sprintf(buffer, "%s: %d", 
            sa_is_new_best_moves(gm->game_instance->data) ? "moves (new best)" : "moves",
            gm->game_instance->data->moves);
    GtkWidget *label_moves = gtk_label_new_with_mnemonic(buffer);
    sprintf(buffer, "%s: %d", 
            sa_is_new_best_time(gm->game_instance->data) ? "time (new best)" : "time",
            gm->game_instance->data->time_elapsed);
    GtkWidget *label_time = gtk_label_new_with_mnemonic(buffer);
    sprintf(buffer, "best moves: %d", gm->game_instance->data->best_moves);
    GtkWidget *label_best_moves = gtk_label_new_with_mnemonic(buffer);
    sprintf(buffer, "best time: %d", gm->game_instance->data->best_time);
    GtkWidget *label_best_time = gtk_label_new_with_mnemonic(buffer);

    gtk_box_pack_start(GTK_BOX(box), success, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box), label_moves, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box), label_time, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box), label_best_moves, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box), label_best_time, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box), button_return_to_menu, 
                       FALSE, TRUE, 10);
    g_signal_connect(button_return_to_menu, "clicked",
                     G_CALLBACK(_gm_return_to_menu), gm);
    sa_level_meta_to_file(gm->game_instance->data);
}

void gm_exit_endscreen(GManager *gm, bool abandoned) {
    gm_clear_box(gm->box_endscreen);
    GtkWidget *box = gm->box_endscreen;
    char buffer[20];
    GtkWidget *game_over = gtk_image_new_from_file("assets/sokoban_game_over.png");
    GtkWidget *button_return_to_menu = gtk_button_new_with_label("MAIN MENU");

    sprintf(buffer, "%s", abandoned ? "Game Abandoned" : "Progress Saved!");
    GtkWidget *label_message = gtk_label_new_with_mnemonic(buffer);

    gtk_box_pack_start(GTK_BOX(box), game_over, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box), label_message, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box), button_return_to_menu, 
                       FALSE, TRUE, 10);
    g_signal_connect(button_return_to_menu, "clicked",
                     G_CALLBACK(_gm_return_to_menu), gm);
}

GtkWidget *gm_menu_init(GManager *gm) {

    GtkWidget *lvl_selectors[LEVEL_COUNT];
    GtkWidget *button_exit = gtk_button_new_with_label("EXIT");
    GLevelSelectData *lvl_selectors_data[LEVEL_COUNT];
    GtkWidget *title = gtk_image_new_from_file("assets/sokoban_title.png");
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
        lvl_selectors_data[i] = gm_level_select_data_init(i, gm);
        g_signal_connect(lvl_selectors[i], "clicked",
                G_CALLBACK(_gm_select_level), lvl_selectors_data[i]);
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) {
            gtk_grid_attach(GTK_GRID(lvl_selector_grid), lvl_selectors[j + i * 4], 
                            i, j, 1, 1);
        }
    }
    g_signal_connect(button_exit, "clicked",
            gtk_main_quit, NULL);

    gtk_box_pack_start(GTK_BOX(box_master), title, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box_master), lvl_selector_grid, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box_master), button_exit, FALSE, TRUE, 10);

    return box_master;
}

GtkWidget *gm_endscreen_init(GManager *gm) {
    GtkWidget *box_master = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *label = gtk_label_new_with_mnemonic("hello");
    GtkWidget *button_return_to_menu = gtk_button_new_with_label("MAIN MENU");

    gtk_box_pack_start(GTK_BOX(box_master), label, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box_master), button_return_to_menu, 
                       FALSE, TRUE, 10);
    g_signal_connect(button_return_to_menu, "clicked",
                     G_CALLBACK(_gm_return_to_menu), gm);
    return box_master;
}

void gm_game_window(SokobanGame *game, GManager *gm) {
    
    GtkWidget *box_master = gm->box_game;
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

    g_signal_connect(G_OBJECT(button_restart), "clicked", 
                     G_CALLBACK(_sg_restart_game), game);                     
    g_signal_connect(G_OBJECT(button_abandon), "clicked", 
                     G_CALLBACK(_gm_abandon_game), gm);                     
    g_signal_connect(G_OBJECT(button_save), "clicked", 
                     G_CALLBACK(_gm_save_game), gm);                     
    g_timeout_add(1000, (GSourceFunc)_sg_time_label_update, (gpointer)game);
    
    gm->box_game_empty = false;
}

GManager *gm_game_manager_init(GtkWidget *window) {
    GManager *new = malloc(sizeof(GManager)); 
    new->game_instance = NULL;
    new->gui_state = MAIN_MENU;
    new->window_master = window;
    new->stack_master = gtk_stack_new();
    new->box_game = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    new->box_game_empty = true;
    new->box_menu = gm_menu_init(new);
    new->box_endscreen = gm_endscreen_init(new);

    gtk_stack_add_named(GTK_STACK(new->stack_master), new->box_endscreen,
            kMasterStackIDs[END_SCREEN]);
    
    gtk_stack_add_named(GTK_STACK(new->stack_master), new->box_menu,
            kMasterStackIDs[MAIN_MENU]);

    gtk_stack_add_named(GTK_STACK(new->stack_master), new->box_game,
            kMasterStackIDs[GAME]);

    g_timeout_add(100, (GSourceFunc)_gm_update_widgets, (gpointer)new);

    return new;
}
