typedef struct tuning
{
    char* name;
    float STRING_1_FREQ;
    float STRING_2_FREQ;
    float STRING_3_FREQ;
    float STRING_4_FREQ;
    float STRING_5_FREQ;
    float STRING_6_FREQ;
} tuning;

tuning standardTuning;
tuning dropDTuning;
tuning eFlatTuning;

tuning newTuning(float freqs[6], char* name);
int setTuning(tuning tune);

int notationByFrequency(float freq, char output[16], int bufferLength);
int frequencyByNotation(char* note, char* buf, int bufLen);
float *getFrequencies(tuning tune);
float halfStepDown(float in);
float halfStepUp(float in);
