
#pragma once

#include "routines.h"
#include "Chrono.h"
#include "../services/hardware.h"

class WomRoutine : public Routine
{
public:
    WomRoutine(Hardware* _hardware) {
        hardware = _hardware;
    }

    const char* name() {
        return "wom";
    }

    void before() {
        hardware->reset();
    }

    void tick() {
        if (timer.elapsed() > 10) {
            timer.restart();

            char buf[16];
            sprintf(buf, "%lu", ledBrightness);
            hardware->displayMini->getDriver()->drawStr(0, 7, buf);

            hardware->lensLed1->setColor(ledBrightness);
            hardware->lensLed2->setColor(ledBrightness);
            hardware->lensLed3->setColor(ledBrightness);
            hardware->eyeGlow->setBrightness(ledBrightness);
            hardware->batteryGlow->setBrightness(ledBrightness);

            hardware->stripLeft->setAll(CRGB(ledBrightness, 0, 0));
            hardware->stripRight->setAll(CRGB(0, 0, ledBrightness));
            hardware->stripLipLeft->setAll(CRGB(0, ledBrightness, 0));
            hardware->stripLipRight->setAll(CRGB(0, ledBrightness, 0));
        }

        if (ledBrightness >= 255) {
            direction = -1;
        } else if (ledBrightness < 1) {
            direction = 1;
        }

        ledBrightness += direction;
    }

private:
    Hardware* hardware;

    Chrono timer;
    int direction = 1;
    int ledBrightness = 0;
    uint servoAngle = 0;
};
