
#include "sokoban/sokoban_gui.h"
#include "sokoban/constants.h"
#include "sokoban/sokoban.h"
#include "sokoban/game.h"

#include <gtk/gtk.h>
#include <stdio.h>

int main(int argc, char **argv) {
    gtk_init (&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    GManager *gm = gm_game_manager_init(window);
    gtk_container_add(GTK_CONTAINER(window), gm->stack_master);

    gtk_widget_show_all(window);
    gtk_main();
}
