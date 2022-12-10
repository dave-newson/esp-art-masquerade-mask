#pragma once

#include "Arduino.h"
#include "FastLED.h"

class LedStrip
{
public:
    LedStrip(CRGB* _leds, uint _totalLeds) {
        leds = _leds;
        ledsCount = _totalLeds;
    };

    void begin() {
        FastLED.clear();
    };

    void tick() {
        if (isDirty) {
            FastLED.show();
        }
    }

    void setColor(uint led, uint32_t colorcode) {
        if (led > ledsCount) return;
        leds[led] = colorcode;
        isDirty = true;
    }

    uint32_t getColor(uint led) {
        if (led > ledsCount) return 0x00;
        return leds[led];
    }

private:
    CRGB* leds;
    uint ledsCount;
    bool isDirty = false;
};
