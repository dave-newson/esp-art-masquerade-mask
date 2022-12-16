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

    void setColor(const struct CRGB& color) {
        ledStrip->setColor(lightNum, color);
    }

    CRGB getColor() {
        return ledStrip->getColor(lightNum);
    }

private:
    LedStrip* ledStrip;
    uint lightNum;
};
