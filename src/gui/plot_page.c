#include "gui.h"
#include "gl_utils.h"


/* RENDERING PANEL */

gboolean render(GtkGLArea *area, GdkGLContext *context, gpointer data) {
	gl_state *state = g_object_get_data(G_OBJECT(area), "gl_state");
	plot_params *params = (plot_params*)data;

	int width = gtk_widget_get_allocated_width(GTK_WIDGET(area));
	int height = gtk_widget_get_allocated_height(GTK_WIDGET(area));

	glViewport(0, 0, width, height);

	size_t n = params->bin_count;

	complex double freq_data[n];
	float real_data[n];
	dftf_para(params->signal.data+params->time_index, freq_data, n, 16);
	for (size_t i = 0; i < n; i++) {
		real_data[i] = cabs(freq_data[i]);
	}

	float vertices[3*n];
	// double min_y = (double)INT64_MAX;
	// double max_y = 0.0;
	// for (size_t i = 0; i < n; i++) {
	// 	x_coord[i] = width*(i+1)*1.0/(n+1);
	// 	if (data[i] < min_y) min_y = data[i];
	// 	if (data[i] > max_y) max_y = data[i];
	// }

	for (size_t i = 0; i < n; i++) {
		double angle = i*2*M_PI/(n-1);
		double radius = real_data[i]/100+0.6;
		vertices[3*i] = radius*cos(angle);
		vertices[3*i+1] = radius*sin(angle);
		vertices[3*i+2] = 0.0;

		// if (ratio > 1.0) vertices[3*i] /= ratio;
		// else if (ratio < 1.0) vertices[3*i+1] *= ratio;
	}

	prepare_to_draw(state, vertices, sizeof(vertices));
	solid_color_background(0, 0, 0, 0);

	// printf("max: %lf\n", max_y);
	glLineWidth(2);
	glDrawArrays(GL_LINE_STRIP, 0, n);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		printf("OpenGL error: 0x%x\n", err);
	}

	params->time_index += 16*1e-3*params->signal.sample_rate;

	return TRUE;
}


void on_realize(GtkGLArea *area) {
	gtk_gl_area_make_current(area);
	if (gtk_gl_area_get_error(area)) return;

	if (!gladLoadGL()) {
		fprintf(stderr, "Failed to initialize GLAD\n");
		return;
	}

	gl_state *state = g_new0(gl_state, 1);
	state->shaderProgram = create_shader_program("../src/gui/default.vert", "../src/gui/default.frag");
	create_gl_buffers(state, MAX_FREQ_BINS*5);

	g_object_set_data_full(G_OBJECT(area), "gl_state", state, g_free);
}


gboolean tick(gpointer data) {
	GtkGLArea *area = GTK_GL_AREA(data);
	gtk_gl_area_queue_render(area);
	return TRUE;
}


GtkWidget *create_render_panel(plot_params *params) {
	GtkWidget *plotting_area = gtk_gl_area_new();
	g_signal_connect(plotting_area, "realize", G_CALLBACK(on_realize), NULL);
	g_signal_connect(plotting_area, "render", G_CALLBACK(render), params);
	g_timeout_add(16, tick, plotting_area);
	return plotting_area;
}


/* SIDE PARAMETER PANEL */


// BIN COUNT

void bin_count_changed(GtkAdjustment *self, gpointer data) {
	plot_params *params = (plot_params*)data;
	params->bin_count = gtk_adjustment_get_value(self);
}


GtkWidget *create_bins_slider(plot_params *params) {
	GtkWidget *bin_slider = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	GtkWidget *slider_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	int min_value = 20;
	int max_value = MAX_FREQ_BINS;
	int init_value = params->bin_count;
	GtkAdjustment *adjustment = gtk_adjustment_new(init_value, min_value, max_value, 1, 10, 0);
	g_signal_connect(adjustment, "value-changed", G_CALLBACK(bin_count_changed), params);

	GtkWidget *slider = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);
	gtk_scale_set_digits(GTK_SCALE(slider), 0);
	
	GtkWidget *number_display = gtk_spin_button_new(adjustment, 1, 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(number_display), init_value);

	gtk_box_append(GTK_BOX(slider_box), slider);
	gtk_box_append(GTK_BOX(slider_box), number_display);
	gtk_widget_set_hexpand(slider_box, TRUE);
	gtk_widget_set_hexpand(slider, TRUE);

	gtk_box_append(GTK_BOX(bin_slider), gtk_label_new("Bin Count"));
	gtk_box_append(GTK_BOX(bin_slider), slider_box);

	return bin_slider;
}


// FILE CHOOSER

void set_audio_file(const char *filename, plot_params *params) {
	giodio_file_to_audio(filename, &params->signal, TRUE);
	params->time_index = 0;
}


void choose_file_response(GtkDialog *dialog, int response, gpointer data) {
	plot_params *params = (plot_params*)((void**)data)[0];
	GtkWidget *file_name_field = (GtkWidget*)((void**)data)[1];

	if (response == GTK_RESPONSE_ACCEPT) {
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
		GFile *file = gtk_file_chooser_get_file(chooser);
		const char *filepath = g_file_get_path(file);
		set_audio_file(filepath, params);
		printf("this should workd?\n");
		gtk_editable_set_text(GTK_EDITABLE(file_name_field), filepath);
	}
	gtk_window_destroy(GTK_WINDOW(dialog));
}


void choose_audio_file(GtkButton *self, gpointer data) {
	GtkWidget *diag = gtk_file_chooser_dialog_new(
		"test",
		GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(self))),
		GTK_FILE_CHOOSER_ACTION_OPEN,
		"Open",
		GTK_RESPONSE_ACCEPT,
		NULL
	);
	g_signal_connect(G_OBJECT(diag), "response", G_CALLBACK(choose_file_response), data);
	gtk_window_present(GTK_WINDOW(diag));
}


GtkWidget *create_audio_file_loader(plot_params *params) {
	GtkWidget *loader = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	GtkWidget *button = gtk_button_new_with_label("Choose");
	
	GtkWidget *file_name_field = gtk_entry_new();
	gtk_entry_set_buffer(
		GTK_ENTRY(file_name_field), 
		gtk_entry_buffer_new("Test", 4)
	);
	gtk_editable_set_editable(GTK_EDITABLE(file_name_field), FALSE);
	gtk_widget_set_hexpand(file_name_field, TRUE);
	
	gtk_box_append(GTK_BOX(loader), file_name_field);
	gtk_box_append(GTK_BOX(loader), button);

	void **signal_args = g_new(void*, 2);
	g_object_set_data_full(G_OBJECT(loader), "signal_data", signal_args, g_free);
	signal_args[0] = params;
	signal_args[1] = file_name_field;
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(choose_audio_file), signal_args);
	
	return loader;
}


GtkWidget *create_side_panel(plot_params *params) {
	GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	
	gtk_box_append(GTK_BOX(sidebar), gtk_button_new_with_label("Yo"));
	gtk_box_append(GTK_BOX(sidebar), create_bins_slider(params));
	gtk_box_append(GTK_BOX(sidebar), create_audio_file_loader(params));

	return sidebar;
}


/* MERGED */

GtkWidget *create_plot_page() {
	GtkWidget *grid = gtk_grid_new();
	plot_params *params = g_new0(plot_params, 1);
	g_object_set_data_full(G_OBJECT(grid), "params", params, g_free);
	
	params->bin_count = 1000;
	giodio_file_to_audio("../song.giodio", &params->signal, FALSE);

	GtkWidget *plotting_area = create_render_panel(params);
	
	GtkWidget *sidebar = create_side_panel(params);

	gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_attach(GTK_GRID(grid), sidebar, 0, 0, 3, 1);
	gtk_grid_attach(GTK_GRID(grid), plotting_area, 3, 0, 7, 1);
	
	return grid;
}