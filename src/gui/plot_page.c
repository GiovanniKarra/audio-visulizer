#include "cairo.h"
#include "gui.h"


void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data) {
    cairo_scale(cr, width, height);
	
	GdkRGBA color;

	// Background
	gdk_rgba_parse(&color, "#FFFF99");
	cairo_rectangle(cr, 0, 0, 1, 1);
	gdk_cairo_set_source_rgba(cr, &color);
	cairo_fill(cr);

	// Line
	gdk_rgba_parse(&color, "#000000");
	cairo_move_to(cr, 0.0, 0.5);
	cairo_line_to(cr, 1.0, 0.5);
	gdk_cairo_set_source_rgba (cr, &color);
	cairo_set_line_width(cr, 5.0/height);

	cairo_stroke(cr);
}


GtkWidget *create_plot_page() {
	GtkWidget *grid = gtk_grid_new();

	GtkWidget *ploting_area = gtk_drawing_area_new();
	gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(ploting_area), draw_function, NULL, NULL);
	
	GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_append(GTK_BOX(sidebar), gtk_button_new_with_label("Yo"));

	gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_attach(GTK_GRID(grid), sidebar, 0, 0, 3, 1);
	gtk_grid_attach(GTK_GRID(grid), ploting_area, 3, 0, 7, 1);

	return grid;
}