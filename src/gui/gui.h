#ifndef GUI_H_
#define GUI_H_

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <portaudio.h>

#include "../core/fourier.h"
#include "../utils/utils.h"

#define PAGE_MAINMENU 0
#define PAGE_PLOTTER 1

#define MAX_FREQ_BINS 1000

typedef struct {
	int bin_count;
	audio_data signal;
	uint32_t time_index;
	gboolean paused;
	PaStream *stream;
} plot_params;

GtkWidget *create_mainmenu_page();
GtkWidget *create_plot_page();

#endif