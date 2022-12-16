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

    void setColor(uint led, const struct CRGB& color) {
        if (led > ledsCount) return;
        leds[led] = color;
        isDirty = true;
    }

    CRGB getColor(uint led) {
        if (led > ledsCount) return CRGB::Black;
        return leds[led];
    }

private:
    CRGB* leds;
    uint ledsCount;
    bool isDirty = false;
};
