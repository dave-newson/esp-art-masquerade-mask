#pragma once

#include "Chrono.h"
#include <arduinoFFT.h>

// Microphone Audio Sampler
#define PIN_MIC 2
#define SAMPLES         1024          // Must be a power of 2
#define SAMPLING_FREQ   40000         // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define AMPLITUDE       1000          // Depending on your audio source level, you may need to alter this value. Can be used as a 'sensitivity' control.
#define NOISE           500           // Used as a crude noise filter, values below this are ignored
#define NUM_BANDS 8

class MicTest
{
public:
    MicTest();
    void tick();

    Chrono timer;

    arduinoFFT FFT;
    unsigned int sampling_period_us;
    byte peak[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};              // The length of these arrays must be >= NUM_BANDS
    int oldBarHeights[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int bandValues[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    double vReal[SAMPLES];
    double vImag[SAMPLES];
    unsigned long newTime;

};
