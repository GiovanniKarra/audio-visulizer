#include <stdio.h>
#include <portaudio.h>

#include "audio.h"


void init_portaudio() {
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		printf("Initialization error : %s\n", Pa_GetErrorText(err));
	}
}


int audio_player_callback(
	const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData
) {
	audio_data *audio = (audio_data*)userData;
	float *out = (float*)outputBuffer;
	for (int i = 0; i < framesPerBuffer; i++) {
		out[i] = audio->paused ? 0 : audio->data[audio->time_index+i];
	}
	if (!audio->paused) audio->time_index += framesPerBuffer;
	return 0;
}


void play_audio_signal(PaStream **stream, audio_data *audio) {
	PaError err = Pa_AbortStream(*stream);
	if (err != paNoError) {
		printf("Abort stream error : %s\n", Pa_GetErrorText(err));
	}
	err = Pa_OpenDefaultStream(
		stream,
		0,
		1,
		paFloat32,
		audio->sample_rate,
		1024,
		audio_player_callback,
		(void*)audio
	);
	if (err != paNoError) {
		printf("Default stream error : %s\n", Pa_GetErrorText(err));
	}
	err = Pa_StartStream(*stream);
	if (err != paNoError) {
		printf("Start stream error : %s\n", Pa_GetErrorText(err));
	}
}
