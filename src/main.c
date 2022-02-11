
#include "sokoban/sokoban_gui.h"
#include "sokoban/constants.h"
#include "sokoban/sokoban.h"
#include "sokoban/game.h"
#include "sokoban/move_stack.h"

#include <gtk/gtk.h>
#include <stdio.h>

int test() {
    MoveStack *ms = mv_stack_init();
    Move move;
    for (int i = 0; i < 1000; ++i) {
        for (int k = 0; k < 3; ++k) {
            move[k] = i + k;
        }
        mv_stack_push(ms, &move);
    }
    mv_stack_print(ms);

    while (mv_stack_pop(ms, &move)) {
        printf("%d %d %d\n", move[0], move[1], move[2]);
    }
    mv_stack_free(ms);
}

int main(int argc, char **argv) {
    test();
    gtk_init (&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    GManager *gm = gm_game_manager_init(window);
    gtk_container_add(GTK_CONTAINER(window), gm->stack_master);

    gtk_widget_show_all(window);
    gtk_main();
}


