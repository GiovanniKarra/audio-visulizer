#ifndef GUI_H_
#define GUI_H_

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <GL/gl.h>

#include "../core/audio_anal.h"

#define PAGE_MAINMENU 0
#define PAGE_PLOTTER 1

GtkWidget *create_mainmenu_page();
GtkWidget *create_plot_page();

#endif