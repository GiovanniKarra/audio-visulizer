#include "fourier.h"
#include <pthread.h>
#include <stddef.h>


int dftc(const complex double *input_buf, complex double *output_buf, const size_t buf_size) {
	for (size_t i = 0; i < buf_size; i++) {
		complex double sum = 0;
		for (size_t j = 0; j < buf_size; j++) {
			sum += input_buf[j]*cexp(-I*2*M_PI*i*j/buf_size);
		}
		output_buf[i] = sum;
	}
	return 0;
}

int dftd(const double *input_buf, complex double *output_buf, const size_t buf_size) {
	for (size_t i = 0; i < buf_size; i++) {
		complex double sum = 0;
		if (i < buf_size/2) {
			for (size_t j = 0; j < buf_size; j++) {
				sum += input_buf[j]*cexp(-I*2*M_PI*i*j/buf_size);
			}
			output_buf[i] = sum;
		}
		else {
			output_buf[i] = output_buf[buf_size-i];
		}
	}
	return 0;
}

int dftf(const float *input_buf, complex double *output_buf, const size_t buf_size) {
	for (size_t i = 0; i < buf_size; i++) {
		complex double sum = 0;
		if (i < buf_size/2) {
			for (size_t j = 0; j < buf_size; j++) {
				sum += input_buf[j]*cexp(-I*2*M_PI*i*j/buf_size);
			}
			output_buf[i] = sum;
		}
		else {
			output_buf[i] = output_buf[buf_size-i];
		}
	}
	return 0;
}


typedef struct {
	size_t n;
	size_t start_index;
	size_t full_buff_size;
	const float *input_buf;
	complex double *output_buf;
} dft_params_t;


void *dftf_thread(void *arg) {
	dft_params_t *param = (dft_params_t*)arg;
	for (size_t i = param->start_index; i < param->start_index+param->n; i++) {
		complex double sum = 0;
		for (size_t j = 0; j < param->full_buff_size; j++) {
			sum += param->input_buf[j]*cexp(-I*2*M_PI*i*j/param->full_buff_size);
		}
		param->output_buf[i] = sum;
	}
	return NULL;
}

int dftf_para(const float *input_buf, complex double *output_buf, const size_t buf_size, const uint8_t nthreads) {
	pthread_t threads[nthreads];
	size_t n_per_thread = buf_size/2/nthreads;
	size_t rest = (buf_size/2)%n_per_thread;
	dft_params_t params[nthreads];
	for (size_t i = 0; i < nthreads; i++) {
		size_t n = n_per_thread + (i+1 == nthreads? rest: 0);
		params[i] = (dft_params_t){n, n_per_thread*i, buf_size, input_buf, output_buf};
		pthread_create(&threads[i], NULL, dftf_thread, (void*)&params[i]);
	}
	for (size_t i = 0; i < nthreads; i++) {
		pthread_join(threads[i], NULL);
	}
	for (size_t i = buf_size/2; i < buf_size; i++) {
		output_buf[i] = output_buf[buf_size-i];
	}
	return 0;
}

complex double dft_at(const size_t i, const complex double *input_buf, const size_t buf_size) {
	complex double sum = 0;
	for (size_t j = 0; j < buf_size; j++) {
		sum += input_buf[j]*cexp(-I*2*M_PI*i*j/buf_size);
	}
	return sum;
}


// int main() {
// 	int n = 100;
// 	complex double sine[n];
// 	complex double four[n];
// 	for (int i = 0; i < n; i++) {
// 		sine[i] = sin(-5+i*2*5.0/(n-1));
// 		printf("%lf\t", creal(sine[i]));
// 	}
// 	printf("\n\n === FOURIER === \n\n");

// 	dft(sine, four, n);
// 	for (int i = 0; i < n; i++) {
// 		printf("%lf\t", creal(four[i]));
// 	}
// 	printf("\n");

// 	return 0;
// }