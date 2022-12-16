
#pragma once

#include "routines.h"
#include "../services/hardware.h"
#include "Chrono.h"

class RainbowRoutine : public Routine
{
public:
    RainbowRoutine(Hardware* _hardware) {
        hardware = _hardware;
    }

    const char* name() {
        return "rainbow";
    }

    void before() {
        hardware->reset();
    }

    void tick() {

        // 60fps at most
        if (timer1.elapsed() < 1000/60) {
            return;
        }
        timer1.restart();

        step++;
        offset = 0;
        uint i;

        for (i=0; i < hardware->stripLeft->getCount(); i++) {
            hardware->stripLeft->setColor(i, calcHue());
        }

        for (i=0; i < hardware->stripLipLeft->getCount(); i++) {
            hardware->stripLipLeft->setColor(i, calcHue());
        }

        for (i=0; i < hardware->stripLipRight->getCount(); i++) {
            hardware->stripLipRight->setColor(i, calcHue());
        }

        for (i=0; i < hardware->stripRight->getCount(); i++) {
            hardware->stripRight->setColor(i, calcHue());
        }

        hardware->batteryGlow->setBrightness(CRGB(hardware->stripRight->getColor(16)).r);
        hardware->eyeGlow->setBrightness(CRGB(hardware->stripLeft->getColor(16)).g);

        hardware->lensLed1->setColor(hardware->stripLeft->getColor(10));
        hardware->lensLed2->setColor(hardware->stripLeft->getColor(16));
        hardware->lensLed3->setColor(hardware->stripLeft->getColor(20));
    }

    void changeSpeed() {
        speed += 8.f;
        if (speed > 40) speed = 5.f;
    }

private:

    CRGB calcHue() {
        offset++;

        float hue = 1.f;

        // Time
        hue += step * speed;

        // Offset
        hue += offset * size;

        CHSV hsv;
        hsv.hue = hue;
        hsv.val = 255;
        hsv.sat = 240;

        return hsv;
    };

    Hardware* hardware;

    Chrono timer1;

    uint step = 0;
    float speed = 5.f;
    float size = 8.f;
    uint offset = 0;
};

