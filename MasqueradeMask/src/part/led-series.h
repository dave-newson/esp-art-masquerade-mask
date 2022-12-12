#pragma once

#include "Arduino.h"
#include "led-strip.h"

class LedSeries
{
public:
    LedSeries(LedStrip* _ledStrip, uint _start, uint _end) {
        ledStrip = _ledStrip;
        start = _start;
        end = _end;
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

    uint32_t getColor(uint led) {
        return ledStrip->getColor(start + led);
    }

    uint getCount() {
        return end - start;
    }

private:
    LedStrip* ledStrip;
    uint start;
    uint end;
};
