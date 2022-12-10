#pragma once

#include "Arduino.h"
#include "Arduino_GFX.h"

class DisplayEye
{
public:
    DisplayEye(Arduino_GFX* _driver) {
        driver = _driver;
    };

    void begin() {
        driver->fillScreen(0xffff);
    };

    Arduino_GFX* getDriver() {
        return driver;
    }

private:
    Arduino_GFX* driver;
};
