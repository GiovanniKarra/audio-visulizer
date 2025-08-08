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
	uint32_t last_time_index;
	float last_time_tick_update;
} render_data;

typedef struct {
	int bin_count;
	int truncation_factor;
	audio_data signal;
	PaStream *stream;
	render_data render;
	gboolean virtual_frames_enabled;
	int thread_count;
	GdkRGBA background_color;
	GdkRGBA plot_color;
	int radius;
	int scale_down;
	gboolean outline;
} plot_params;

GtkWidget *create_mainmenu_page();
GtkWidget *create_plot_page();

#endif