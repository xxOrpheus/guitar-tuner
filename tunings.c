#include "tunings.h"
#include <stddef.h>

float stepDown(float in) {
    return in / 1.0595;
}

float stepUp(float in) {
    return in * 1.0595;
}

float *getFrequencies(tuning tune) {
    float freqs[] = {
        tune.STRING_1_FREQ,
        tune.STRING_2_FREQ,
        tune.STRING_3_FREQ,
        tune.STRING_4_FREQ,
        tune.STRING_5_FREQ,
        tune.STRING_6_FREQ
    };

    return freqs;
}

int setFrequencies(float freqs[], tuning tune) {
    size_t sz = sizeof(freqs) / sizeof(freqs[0]);
    if(sz != 6) {
        return -1;
    }
    tune.STRING_1_FREQ = freqs[0];
    tune.STRING_2_FREQ = freqs[1];
    tune.STRING_3_FREQ = freqs[2];
    tune.STRING_4_FREQ = freqs[3];
    tune.STRING_5_FREQ = freqs[4];
    tune.STRING_6_FREQ = freqs[5];
    return 0;
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
