#ifndef AUDIO_H_
#define AUDIO_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <portaudio.h>

typedef struct {
	uint32_t n;
	int sample_rate;
	int channels;
	float *data;
	uint32_t time_index;
	bool paused;
} audio_data;

int giodio_file_to_audio(const char *filename, audio_data *ret);
int wav_file_to_audio(const char *filename, audio_data *ret);

void play_audio_signal(PaStream **stream, audio_data *audio);
void init_portaudio();

#endif