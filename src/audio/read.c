#include "audio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


int giodio_file_to_audio(const char *filename, audio_data *ret) {
	FILE *file = fopen(filename, "r");

	uint32_t sample_count;
	uint32_t sample_rate;
	fread(&sample_count, sizeof(uint32_t), 1, file);
	fread(&sample_rate, sizeof(uint32_t), 1, file);
	
	float *data = (float*)realloc(ret->data, sizeof(float)*sample_count);
	fread(data, sizeof(float), sample_count, file);

	ret->n = sample_count;
	ret->sample_rate = sample_rate;
	ret->data = data;

	return 0;
}


void wav_file_to_audio(const char *filename, audio_data *ret) {
}