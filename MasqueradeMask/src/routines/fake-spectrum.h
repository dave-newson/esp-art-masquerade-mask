
#pragma once

#include "routines.h"
#include "../services/hardware.h"

class FakeSpectrumRoutine : public Routine
{
public:
    FakeSpectrumRoutine(Hardware* _hardware) {
        hardware = _hardware;
    }

    const char* name() {
        return "fake-specrum";
    }

    void before() {
        hardware->reset();
        hardware->batteryGlow->setBrightness(0);
        hardware->eyeGlow->setBrightness(0);

        hardware->lensLed1->setColor(CRGB::Black);
        hardware->lensLed2->setColor(CRGB::Black);
        hardware->lensLed3->setColor(CRGB::Black);
    }

    void tick() {

        if(timer.elapsed() < 1000/60) {
            return;
        }
        timer.restart();

        // Specrum data
        emulateAudio();

        // Specrum display
        hardware->displayMini->getDriver()->clearBuffer();
        for (uint i=0; i<16; i++) {
            hardware->displayMini->getDriver()->drawLine(i*4, 32, i*4, 32-getScaledBand(bands[i], 32));
            hardware->displayMini->getDriver()->drawLine((i*4)+1, 32, (i*4)+1, 32-getScaledBand(bands[i], 32));
        }

        // Two-band LEDs
        uint ledCount = hardware->stripLeft->getCount();
        hardware->stripLeft->setAll(CRGB::Black);
        hardware->stripRight->setAll(CRGB::Black);
        
        int volume;
        for (uint i=0; i < ledCount; i++) {
            hardware->stripRight->setColor(ledCount - i, (getScaledBand(bands[0], ledCount) > i) ? CRGB::Red : CRGB::Black);
            hardware->stripLeft->setColor(i, (getScaledBand(bands[15], ledCount) > i) ? CRGB::Red : CRGB::Black);

            if (getScaledBand(peaks[0], ledCount) == i) {
                hardware->stripRight->setColor(ledCount - i, CRGB::White);
            }

            if (getScaledBand(peaks[15], ledCount) == i) {
                hardware->stripLeft->setColor(i, CRGB::White);
            }
        }
        
        hardware->batteryGlow->setBrightness(bands[2]);
        hardware->eyeGlow->setBrightness(bands[3]);

        hardware->lensLed1->setColor(CRGB::Red * bands[4]);
        hardware->lensLed2->setColor(CRGB::Green * bands[5]);
        hardware->lensLed3->setColor(CRGB::Blue * bands[6]);
    }

private:
    void emulateAudio()
    {
        for (uint i=0; i<16; i++) {

            // reduce band
            bands[i] = max(0, bands[i] - 6);
            
            // Reduce peak
            peaks[i] = max(0, peaks[i] - 2);

            // 5% of new Band position
            if (random(100) < 20) {
                bands[i] = (int) max(random(255), (long) bands[i]);
            }

            // Bump peak
            peaks[i] = max(bands[i], peaks[i]);
        }
    }

    int getScaledBand(int bandValue, int max)
    {
        float step = bandValue;
        step /= 255;
        step *= max;
        step = ceil(step);
        return (int) step;
    }

    Hardware* hardware;
    Chrono timer;

    int bands[16];
    int peaks[16];
};
