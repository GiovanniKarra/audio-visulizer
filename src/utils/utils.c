#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


float *read_giodio_file(const char *filename, int *recv_sample_count, int *recv_sample_rate) {
	FILE *file = fopen(filename, "r");

	uint32_t sample_count;
	uint32_t sample_rate;
	fread(&sample_count, sizeof(uint32_t), 1, file);
	fread(&sample_rate, sizeof(uint32_t), 1, file);
	if (recv_sample_rate != NULL) {
		*recv_sample_rate = sample_rate;
	}
	if (recv_sample_count != NULL) {
		*recv_sample_count = sample_count;
	}
	
	float *data = (float*)malloc(sizeof(float)*sample_count);
	fread(data, sizeof(float), sample_count, file);
	
	return data;
}