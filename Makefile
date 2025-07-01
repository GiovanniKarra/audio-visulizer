

all:
	@gcc `pkg-config --cflags gtk4` -o main src/gui/main.c `pkg-config --libs gtk4`

run:
	@gcc -o main src/core/fourier.c -lm
	@./main

tests:
	@gcc -o tests tests/test_fourier.c src/core/fourier.c
	@./tests