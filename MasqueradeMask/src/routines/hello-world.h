
#pragma once

#include "routines.h"
#include "../services/hardware.h"

class HelloWorldRoutine : public Routine
{
public:
    HelloWorldRoutine(Hardware* _hardware) {
        hardware = _hardware;
    }

    const char* name() {
        return "test-two";
    }

    void before() {
        hardware->reset();
        color = CRGB::White;
    }

    void tick() {
        if (!isDirty) {
            return;
        }

        hardware->displayMini->getDriver()->drawStr(0, 7, "Hello World");
        hardware->batteryGlow->setBrightness(255);
        hardware->eyeGlow->setBrightness(255);
        hardware->lensLed1->setColor(color);
        hardware->lensLed2->setColor(color);
        hardware->lensLed3->setColor(color);
        hardware->stripLeft->setAll(color);
        hardware->stripRight->setAll(color);
        hardware->stripLipLeft->setAll(color);
        hardware->stripLipRight->setAll(color);

    }

    void changeColor()
    {
        CHSV hsv;
        hsv.hue = random(255);
        hsv.val = 255;
        hsv.sat = 240;

        color = hsv;
        isDirty = true;
    }

private:
    Hardware* hardware;

    bool isDirty = true;
    CRGB color = CRGB::White;
};
