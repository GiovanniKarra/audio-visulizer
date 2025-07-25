#include "audio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>


int giodio_file_to_audio(const char *filename, audio_data *ret) {
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		perror("Giodio file read");
		return 1;
	}

	uint32_t sample_count;
	uint32_t sample_rate;
	fread(&sample_count, sizeof(uint32_t), 1, file);
	fread(&sample_rate, sizeof(uint32_t), 1, file);
	
	// if (ret->data != NULL) free(ret->data);
	// float *data = (float*)malloc(sizeof(float)*sample_count);
	float *data = (float*)realloc(ret->data, sizeof(float)*sample_count);
	if (data == NULL) {
		perror("malloc");
		return 1;
	}
	fread(data, sizeof(float), sample_count, file);

	fclose(file);

	ret->n = sample_count;
	ret->sample_rate = sample_rate;
	ret->channels = 1;
	ret->data = data;

	return 0;
}


int wav_file_to_audio(const char *filename, audio_data *ret) {
	SF_INFO sinfo = {0};
	SNDFILE *file = sf_open(filename, SFM_READ, &sinfo);
	if (file == NULL) {
		perror("SINFO read");
		return 1;
	}
	
	sf_count_t item_count = sinfo.frames*sinfo.channels;
	// if (ret->data != NULL) free(ret->data);
	// float *data = (float*)malloc(sizeof(float)*item_count);
	float *data = (float*)realloc(ret->data, sizeof(float)*item_count);
	if (data == NULL) {
		perror("realloc");
		return 1;
	}
	sf_count_t read_items = sf_read_float(file, data, item_count);

	sf_close(file);

	ret->n = sinfo.frames;
	ret->sample_rate = sinfo.samplerate;
	ret->channels = sinfo.channels;
	ret->data = data;

	return 0;
}