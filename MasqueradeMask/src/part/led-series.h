#pragma once

#include "Arduino.h"
#include "led-strip.h"

class LedSeries
{
public:
    LedSeries(LedStrip* _ledStrip, uint _start, uint _count) {
        ledStrip = _ledStrip;
        start = _start;
        count = _count;
    };

    void begin() {
        setAll(CRGB::Black);
    }

    void setColor(uint led, const struct CRGB& color) {
        if (led > getCount()) return;
        ledStrip->setColor(start + led, color);
    }

    void setAll(const struct CRGB& color) {
        for (uint i = 0; i < getCount(); i++) {
            setColor(i, color);
        }
    }

    CRGB getColor(uint led) {
        return ledStrip->getColor(start + led);
    }

    uint getCount() {
        return count;
    }

private:
    LedStrip* ledStrip;
    uint start;
    uint count;
};
