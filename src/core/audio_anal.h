#ifndef _AUDIO_ANAL_H_
#define _AUDIO_ANAL_H_

#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#define UNUSED(x) (void)(x)

int dftc(const complex double *input_buf, complex double *output_buf, size_t buf_size);
int dftd(const double *input_buf, complex double *output_buf, size_t buf_size);
int dftf(const float *input_buf, complex double *output_buf, size_t buf_size);
int dftf_para(const float *input_buf, complex double *output_buf, const size_t buf_size, const uint8_t nthreads);

#endif