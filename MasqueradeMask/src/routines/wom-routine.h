
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
            hardware->displayMini->getDriver()->clearBuffer();
            hardware->displayMini->getDriver()->drawStr(0, 7, buf);

            hardware->eyeGlow->setBrightness(ledBrightness);
            hardware->batteryGlow->setBrightness(ledBrightness);
            
            CRGB renderColor(color);
            renderColor.subtractFromRGB(255 - ledBrightness);

            hardware->lensLed1->setColor(renderColor);
            hardware->lensLed2->setColor(renderColor);
            hardware->lensLed3->setColor(renderColor);

            hardware->stripLeft->setAll(renderColor);
            hardware->stripRight->setAll(renderColor);
            hardware->stripLipLeft->setAll(renderColor);
            hardware->stripLipRight->setAll(renderColor);
        }

        if (ledBrightness >= 255) {
            direction = -1;
        } else if (ledBrightness < 1) {
            direction = 1;
            changeColor();
        }

        ledBrightness += direction;
    }

private:

    Hardware* hardware;

    Chrono timer;
    int direction = 1;
    uint ledBrightness = 0;
    uint servoAngle = 0;
    CRGB color;

    void changeColor()
    {
        CHSV hsv;
        hsv.hue = random(255);
        hsv.val = 255;
        hsv.sat = 240;
        color = hsv;
    }

};
