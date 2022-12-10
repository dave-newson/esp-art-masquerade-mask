#pragma once

#include "Arduino.h"
#include "led-strip.h"

class LedSingle
{
public:
    LedSingle(LedStrip* _ledStrip, uint _lightNum) {
        ledStrip =_ledStrip;
        lightNum = _lightNum;
    };

    void begin() {
        setColor(CRGB::Black);        
    };

    void setColor(uint32_t colorcode) {
        ledStrip->setColor(lightNum, colorcode);
    }

    uint32_t getColor() {
        return ledStrip->getColor(lightNum);
    }

private:
    LedStrip* ledStrip;
    uint lightNum;
};
