
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

    char src[50] = "levels/9.txt";
    char buffer[1000];
    FILE *f = fopen(src, "r");
    char c;
    int i = 0;
    while ((c = fgetc(f)) != EOF) buffer[i++] = c;
    buffer[i] = '\0';
    Sokoban *s = sokoban_init_from_buffer(&buffer[0]);
    gtk_init (&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    sg_init_game_window(window, s);
    //mn_menu(window);

    gtk_widget_show_all(window);
    gtk_main();
}