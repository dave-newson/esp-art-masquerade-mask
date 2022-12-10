#pragma once

#include "Arduino.h"
#include "U8g2lib.h"

class DisplayMini
{
public:
    DisplayMini(U8G2* _driver) {
        driver = _driver;
    };

    void begin() {
        driver->clearBuffer();
        driver->sendBuffer();
    };

    U8G2* getDriver() {
        isDirty = true;
        return driver;
    }

    void tick() {
        if (isDirty) {
            driver->sendBuffer();
        }
    }

private:
    U8G2* driver;
    bool isDirty = false;
};
