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

int notationByFrequency(float freq, char* output, int bufferLength);
int frequencyByNotation(char* note, char* buf, int bufLen);
float *getFrequencies(tuning tune);
float stepDown(float in);
float stepUp(float in);
