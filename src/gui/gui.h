#ifndef GUI_H_
#define GUI_H_

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <portaudio.h>

#include "../core/fourier.h"
#include "../audio/audio.h"

#define PAGE_MAINMENU 0
#define PAGE_PLOTTER 1

#define MAX_FREQ_BINS 5000

typedef struct {
	int bin_count;
	audio_data signal;
	PaStream *stream;
} plot_params;

GtkWidget *create_mainmenu_page();
GtkWidget *create_plot_page();

#endif