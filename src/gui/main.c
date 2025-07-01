#include "gui.h"


GMenu *create_menubar() {
	GMenu *menubar = g_menu_new();
	GMenu *filemenu = g_menu_new();

	g_menu_append_submenu(menubar, "_File", G_MENU_MODEL(filemenu));

	return menubar;
}

static void activate(GtkApplication *app, gpointer user_data) {
	GtkWidget *window;

	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Gio Visualizer");
	gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

	GtkWidget *page_mainmenu = create_mainmenu_page();
	GtkWidget *page_plotter = create_plot_page();
	
	GtkWidget *page_stack = gtk_stack_new();
	gtk_stack_add_child(GTK_STACK(page_stack), page_plotter);
	gtk_stack_add_child(GTK_STACK(page_stack), page_mainmenu);

	GMenu *menubar = create_menubar();
	gtk_application_set_menubar(app, G_MENU_MODEL(menubar));
	gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(window), TRUE);

	gtk_window_set_child(GTK_WINDOW(window), page_stack);

	gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
	GtkApplication *app;
	int status;
	
	GSettings *settings = g_settings_new("org.gnome.desktop.interface");
	const gchar *theme = g_settings_get_string(settings, "gtk-theme");
	g_setenv("GTK_THEME", theme, TRUE);

	app = gtk_application_new("be.karra.audiovisu", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
