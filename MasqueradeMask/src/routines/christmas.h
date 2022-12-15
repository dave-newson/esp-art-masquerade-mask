
#pragma once

#include "routines.h"
#include "../services/hardware.h"
#include "Chrono.h"

class ChristmasRoutine : public Routine
{
public:
    ChristmasRoutine(Hardware* _hardware) {
        hardware = _hardware;
    }

    const char* name() {
        return "christmas";
    }

    void before() {
        hardware->reset();
        hardware->eyeGlow->setBrightness(255);
        hardware->batteryGlow->setBrightness(255);
        timer1.restart();
        setLights();
        setMsg();
    }

    void tick() {

        if (timer1.elapsed() > 2000) {
            timer1.restart();
            offset = !offset;
            setLights();
            setMsg();
        }

    }

    void setLights()
    {
        for (uint i=0; i < hardware->stripLeft->getCount(); i++) {
            hardware->stripLeft->setColor(i, getColor());
        }

        for (uint i=0; i < hardware->stripLipLeft->getCount(); i++) {
            hardware->stripLipLeft->setColor(i, getColor());
        }

        for (uint i=0; i < hardware->stripLipRight->getCount(); i++) {
            hardware->stripLipRight->setColor(i, getColor());
        }

        for (uint i=0; i < hardware->stripRight->getCount(); i++) {
            hardware->stripRight->setColor(i, getColor());
        }
    }

    void setMsg() {
        hardware->displayMini->getDriver()->clearBuffer();
        if (msgState) {
            hardware->displayMini->getDriver()->drawStr(0, 12, "   MERRY  ");
            hardware->displayMini->getDriver()->drawStr(0, 22, " CHRISTMAS");
        } else {
            hardware->displayMini->getDriver()->drawStr(0, 12, " YA FILTHY  ");
            hardware->displayMini->getDriver()->drawStr(0, 22, "  ANIMAL    ");
        }
        msgState = !msgState;
    }

private:
    CRGB getColor() {
        offset = !offset;
        return offset ? CRGB::Green : CRGB::Red;
    }

    Hardware* hardware;

    Chrono timer1;
    bool offset = false;
    bool msgState = true;
};
