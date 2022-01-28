#include "sokoban/sokboard.h"
#include <gtk/gtk.h>
#include <stdio.h>

static void init(GtkApplication *app, gpointer data) {
    GtkWidget *window = gtk_application_window_new(app);
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    //gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    GdkPixbuf *pb = gdk_pixbuf_new_from_file_at_scale("shrek.png", 100, 100,
    TRUE, NULL);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            gtk_grid_attach(GTK_GRID(grid), gtk_image_new_from_pixbuf(pb),
            i, j, 1, 1);
        }
    }
    gtk_widget_show_all(window);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    int status;
    GtkApplication *app = gtk_application_new("org.sokoban", 
    G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(init), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
    /*char src[50] = "test_board.txt";
    char buffer[1000];
    FILE *f = fopen(src, "r");
    char c;
    int i = 0;
    while ((c = fgetc(f)) != EOF) buffer[i++] = c;
    buffer[i] = '\0';
    Sokoban *s = sokoban_init_from_buffer(buffer);
    sokoban_print(s);
    while ((c = getchar()) != '0') { 
        switch (c) {
            case 'h':
                move_player(s, LEFT);
                break;
            case 'j':
                move_player(s, DOWN);
                break;
            case 'k':
                move_player(s, UP);
                break;
            case 'l':
                move_player(s, RIGHT);
                break;
        };
        sokoban_print(s);
        if (!s->crates_left) {
            printf("SUCCESS!\n");
            return 0;
        }
    }*/
    return 0;
}