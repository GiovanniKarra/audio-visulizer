#include <stdio.h>
#include <string.h>
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
	if (audio->paused) {
		memset(out, 0, framesPerBuffer*audio->channels*sizeof(float));
	}
	else {
		memcpy(out, audio->data+audio->time_index*audio->channels, framesPerBuffer*audio->channels*sizeof(float));
		audio->time_index += framesPerBuffer;
	}
	return 0;
}


void play_audio_signal(PaStream **stream, audio_data *audio) {
	// PaError err = Pa_AbortStream(*stream);
	// if (err != paNoError) {
	// 	printf("Abort stream error : %s\n", Pa_GetErrorText(err));
	// }
	PaError err = Pa_OpenDefaultStream(
		stream,
		0,
		audio->channels,
		paFloat32,
		audio->sample_rate,
		2048,
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
