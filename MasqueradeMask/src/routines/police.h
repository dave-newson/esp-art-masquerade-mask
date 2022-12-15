
#pragma once

#include "routines.h"
#include "../services/hardware.h"
#include "Chrono.h"

#define FLASH_MS 50
#define STYLE_CHANGE_MS 3000

class PoliceRoutine : public Routine
{
    enum flashStyle {
        flashAlternate = 1, // Flash LEFT only blue, then RIGHT only red
        flashSplitPair = 2, // Flash half LEFT blue, RIGHT red, then the other LEFT & RIGHT
        steadyBar = 3 // Hold LEFT BLUE on, then RIGHT RED on
    };

public:
    PoliceRoutine(Hardware* _hardware) {
        hardware = _hardware;
    }

    const char* name() {
        return "police";
    }

    void before() {
        hardware->reset();
        hardware->lensLed1->setColor(CRGB::Yellow);
        hardware->lensLed2->setColor(CRGB::Yellow);
        hardware->lensLed3->setColor(CRGB::Yellow);
        hardware->eyeGlow->setBrightness(255);
        hardware->batteryGlow->setBrightness(255);

        count = hardware->stripLeft->getCount();
        half = ceil(count / 2);

        timerFlashes.restart();
        timerAlt.restart();
        timerStyle.restart();
    }

    void tick() {

        bool isDirty = false;

        if (timerFlashes.elapsed() > FLASH_MS) {
            timerFlashes.restart();
            flashState = !flashState;
            isDirty = true;
        }

        if (timerAlt.elapsed() > 500) {
            timerAlt.restart();
            altState = !altState;
            isDirty = true;
        }

        if (!isDirty) return;

        // Deal with animation
        switch (style) {
            case flashStyle::flashAlternate:
                hardware->stripLeft->setAll(altState ? (flashState ? CRGB::Blue : CRGB::Black) : CRGB::Black);
                hardware->stripRight->setAll(altState ? CRGB::Black : (flashState ? CRGB::Black : CRGB::Red));
                break;
            case flashStyle::flashSplitPair:
                for (uint i=0; i < half; i++) {
                    hardware->stripLeft->setColor(i, (altState ? (flashState ? CRGB::Blue : CRGB::Black) : CRGB::Black));
                    hardware->stripRight->setColor(i, (altState ? CRGB::Black : (flashState ? CRGB::Black : CRGB::Red)));
                    hardware->stripLeft->setColor(half+i, (altState ? CRGB::Black : (flashState ? CRGB::Blue : CRGB::Black)));
                    hardware->stripRight->setColor(half+i, (altState ? (flashState ? CRGB::Black : CRGB::Red) : CRGB::Black));
                }
                break;
            case flashStyle::steadyBar:
                hardware->stripLeft->setAll(altState ? CRGB::Blue : CRGB::Black);
                hardware->stripRight->setAll(altState ? CRGB::Black : CRGB::Red);
                break;
        }

        switch (textState) {
            case 0:
                hardware->displayMini->getDriver()->clearBuffer();
                hardware->displayMini->getDriver()->drawStr(0, 14, "WOOP WOOP");
                break;
            case 1:
                hardware->displayMini->getDriver()->clearBuffer();
                hardware->displayMini->getDriver()->drawStr(0, 7,  "THAT'S THE");
                hardware->displayMini->getDriver()->drawStr(0, 14, " SOUND OF ");
                hardware->displayMini->getDriver()->drawStr(0, 21, "THE POLICE");
                break;
            case 2:
                hardware->displayMini->getDriver()->clearBuffer();
                hardware->displayMini->getDriver()->drawStr(0, 14, "WOOP WOOP");
                break;
            case 3:
                hardware->displayMini->getDriver()->clearBuffer();
                hardware->displayMini->getDriver()->drawStr(0, 7,  "THAT'S THE ");
                hardware->displayMini->getDriver()->drawStr(0, 14, " SOUND OF ");
                hardware->displayMini->getDriver()->drawStr(0, 21, "THE  BEAST");
                break;
        }

        // Change style
        if (timerStyle.elapsed() > STYLE_CHANGE_MS) {
            timerStyle.restart();

            style++;
            style = (style > 3) ? 1 : style;

            textState++;
            textState = (textState > 3) ? 0 : textState;
        }
    }

private:
    Hardware* hardware;

    uint textState = 0;

    Chrono timerFlashes;
    bool flashState = false;

    Chrono timerAlt;
    bool altState = false;

    Chrono timerStyle;
    uint style = 1;

    uint count;
    uint half;

};
