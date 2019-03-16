#include <windows.h>
#include <stdio.h>
#include <dos.h>
#include <SDL.h>
#include <SDL_audio.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "resource.h"
#include <pthread.h>

float sinPos;
float sinStep;
long frequency = 0;
int length = 5000;
void populate(void* data, Uint8 *stream, int len)
{
    int i = 0;
    for(i = 0; i < len; i++)
    {
        stream[i] = (Uint8) (AUDIO_VOLUME * sinf(sinPos)) + 127;
        sinPos += sinStep;
    }
}

int generateTone(float freq, int len)
{

    SDL_AudioSpec spec;
    spec.freq = AUDIO_FREQ;
    spec.format = AUDIO_U8;
    spec.channels = 2;
    spec.samples = AUDIO_SAMPLES;
    spec.callback = (*populate);
    spec.userdata = NULL;

    if(SDL_OpenAudio(&spec, NULL) < 0)
    {
        printf("SDL Error: %s \n", SDL_GetError());
        return 1;
    }

    sinPos = 0;
    sinStep = 2 * M_PI * freq / AUDIO_FREQ;

    SDL_PauseAudio(0);
    Sleep(len);
    SDL_PauseAudio(1);
    SDL_CloseAudio();
    return 0;
}

int stopTone()
{
//    SDL_PauseAudio(1);
    SDL_CloseAudio();
 //   Sleep(500);
    return 0;
}
