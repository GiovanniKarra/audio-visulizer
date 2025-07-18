#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint32_t n;
	int sample_rate;
	float *data;
} audio_data;

float *read_giodio_file(const char *filename, int *recv_sample_count, int *recv_sample_rate);
void giodio_file_to_audio(const char *filename, audio_data *ret, bool free_old_data);