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
    }

    uint getBrightness() {
        return analogRead(pin);
    }

private:
    uint pin;
};
