#pragma once

#include "Arduino.h"

class PwmLed
{
public:
    PwmLed(uint _pin) {
        pin = _pin;
    };

    void begin() {
        pinMode(pin, OUTPUT);
        setBrightness(0);
    };

    void setBrightness(int brightness) {
        analogWrite(pin, brightness);
        value = brightness;
    }

    uint getBrightness() {
        return value;
    }

private:
    uint pin;
    uint value = 0;
};
