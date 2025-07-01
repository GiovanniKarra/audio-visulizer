#include "gui.h"


GtkWidget *create_mainmenu_page(GtkStack *parent_stack) {
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	GtkWidget *title = gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(title), "<span size='xx-large'>Gio >>> Fourier</span>");

	GtkWidget *button = gtk_button_new_with_label("Plotter");

	gtk_box_append(GTK_BOX(vbox), title);

	GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	
	// gtk_widget_set_size_request(sep, 0, 500);
	gtk_box_append(GTK_BOX(vbox), sep);
	
	gtk_box_append(GTK_BOX(vbox), button);

	gtk_widget_set_vexpand(sep, TRUE);

	gtk_widget_set_margin_top(vbox, 50);
	gtk_widget_set_margin_bottom(vbox, 50);
	gtk_widget_set_margin_start(vbox, 50);
	gtk_widget_set_margin_end(vbox, 50);
	// gtk_box_set_homogeneous(GTK_BOX(vbox), TRUE);

	return vbox;
}