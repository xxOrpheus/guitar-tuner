#include "tunings.h"
#include <stddef.h>
#include <stdio.h>
#include "string.h"
#include <math.h>
#include "stdlib.h"

float halfStepDown(float in) {
    return in / 1.0595;
}

float halfStepUp(float in) {
    return in * 1.0595;
}


float Log2(float n) {
    return log(n) / log(2);
}

//https://www.johndcook.com/blog/2016/02/10/musical-pitch-notation/
int notationByFrequency(float freq, char output[16], int bufferLength) {
    float NOTE_A4 = 440;
    float NOTE_C0 = NOTE_A4*pow(2, -4.75);

    const char *name[12] = {
        "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
    };

    int h = round(12 * Log2(freq / NOTE_C0));
    int octave = h / 12;
    int n = (int)h % 12;
    char out[16];
    sprintf(out, "%s%d", name[n], octave);
    strncpy(output, out, bufferLength - 1);
    output[bufferLength-1] = '\0';

    return 0;
}

int frequencyByNotation(char* note, char* buf, int bufLen) {
    // tbc
    return 0;
}

float *getFrequencies(tuning tune) {
    float *ret = malloc(6);
    if(!ret) {
        return NULL;
    }
    float freqs[] = {
        tune.STRING_1_FREQ,
        tune.STRING_2_FREQ,
        tune.STRING_3_FREQ,
        tune.STRING_4_FREQ,
        tune.STRING_5_FREQ,
        tune.STRING_6_FREQ
    };
    for(int i = 0; i < 6; i++) {
        ret[i] = freqs[i];
    }

    return ret;
}

tuning tunes[32];
int tuneCount = 0;

int newTuning_byPointer(tuning tune) {
   // printf("%s %d\n", tune.name, tune_count());
tunes[tuneCount] = tune;
tuneCount = tuneCount + 1;
return tuneCount;
}

int tune_count() {
    return tuneCount;
}

int getTuning_byId(int i, tuning* tune) {
    *tune = tunes[i];
    return 0;
}

int newTuning(float freqs[6], char* name) { // using a wrapper so we can keep track of all tunings
    tuning tune;
    tune.name = name;
    tune.STRING_1_FREQ = freqs[0];
    tune.STRING_2_FREQ = freqs[1];
    tune.STRING_3_FREQ = freqs[2];
    tune.STRING_4_FREQ = freqs[3];
    tune.STRING_5_FREQ = freqs[4];
    tune.STRING_6_FREQ = freqs[5];
    tunes[tuneCount] = tune;
tuneCount = tuneCount + 1;

    return tuneCount;
}


tuning standardTuning =
{
    "E Standard",
    329.63, 246.94, 196, 146.83, 110, 82.41
};

tuning eFlatTuning =
{
    "Half-step Down",
    311.118, 233.072, 184.993, 138.584, 103.823, 77.782
};

tuning dropDTuning;
