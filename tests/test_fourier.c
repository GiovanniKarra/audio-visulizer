#include "../src/core/fourier.h"


void print_fourier(complex double *array, int size) {
	printf("[");
	for (int i = 0; i < size; i++) {
		printf("%lf ", cabs(array[i]));
	}
	printf("]\n");
}

void test_fourier() {
	float sin[] = {1.0, 0.0, -1.0, 0.0, 1.0};
	complex double fourier[5];
	dftf(sin, fourier, 5, 0.0);
	print_fourier(fourier, 5);
}

int main() {
	test_fourier();
	return 0;
}