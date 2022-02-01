#include "game.h"

void mn_menu(GtkWidget *window) {
    GtkWidget *lvl_selectors[LEVEL_COUNT];
    GtkWidget *box_master = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *lvl_selector_grid = gtk_grid_new();
    GdkPixbuf *button_image_src = gdk_pixbuf_new_from_file_at_scale(
                                  kAssetPath[ASSET_CRATE], 100, 100, TRUE, NULL);
    GtkWidget *button_image = gtk_image_new_from_pixbuf(button_image_src);
    char buffer[20];

    gtk_grid_set_row_spacing(GTK_GRID(lvl_selector_grid), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(lvl_selector_grid), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(lvl_selector_grid), 5);
    gtk_grid_set_column_homogeneous(GTK_GRID(lvl_selector_grid), TRUE);

    for (int i = 0; i < LEVEL_COUNT; ++i) {
        sprintf(buffer, "%d", i);
        lvl_selectors[i] = gtk_button_new_with_label(buffer);        
        gtk_button_set_image (GTK_BUTTON (lvl_selectors[i]), button_image);
        gtk_button_set_always_show_image (GTK_BUTTON (lvl_selectors[i]), TRUE);
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) {
            gtk_grid_attach(GTK_GRID(lvl_selector_grid), lvl_selectors[j + i * 4], 
                            i, j, 2, 2);
        }
    }
    gtk_box_pack_start(GTK_BOX(box_master), lvl_selector_grid, FALSE, TRUE, 10);
    gtk_container_add(GTK_CONTAINER(window), box_master);
    gtk_widget_show_all(window);
}

void mn_endscreen(GtkWidget *window, SokobanGame *game) {

}