
#include "sokoban/sokoban_gui.h"
#include "sokoban/constants.h"
#include "sokoban/sokoban.h"
#include "sokoban/game.h"

#include <gtk/gtk.h>
#include <stdio.h>

int main(int argc, char **argv) {
    for (int i = 0; i < ASSET_COUNT; ++i) {
        printf("%s\n", kAssetPath[i]);
    }

    gtk_init (&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    //SokobanGame *game_data = sg_sokoban_game_init(s);
    //GtkWidget *game = sg_game_window(game_data);
    //gtk_container_add(GTK_CONTAINER(window), game);
    //g_signal_connect (G_OBJECT (window), "key_press_event",
    //                 G_CALLBACK (sg_handle_keypress), game_data); 
    //sg_init_game_window(window, s);
    //mn_menu(window);
    GManager *gm = gm_game_manager_init(window);
    gtk_container_add(GTK_CONTAINER(window), gm->stack_master);

    gtk_stack_get_child_by_name(GTK_STACK(gm->stack_master), 
                                "end screen");


    gtk_widget_show_all(window);
    gtk_main();
}
