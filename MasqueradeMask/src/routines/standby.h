
#pragma once

#include "routines.h"
#include "../services/hardware.h"
#include "Chrono.h"

class StandbyRoutine : public Routine
{
public:
    StandbyRoutine(Hardware* _hardware) {
        hardware = _hardware;
    }

    const char* name() {
        return "standby";
    }

    void before() {
        hardware->reset();
    }

    void tick() {
        if (timer1.elapsed() < 500) {
            return;
        }

        hardware->displayMini->getDriver()->clearBuffer();

        switch (state) {
            case 1:
            default:
                hardware->displayMini->getDriver()->drawStr(0, 16, "READY");
                break;
            case 2:
                hardware->displayMini->getDriver()->drawStr(0, 16, "READY .");
                break;
            case 3:
                hardware->displayMini->getDriver()->drawStr(0, 16, "READY ..");
                break;
            case 4:
                hardware->displayMini->getDriver()->drawStr(0, 16, "READY ...");
                state=0;
                break;
        }

        timer1.restart();
        state++;
    }

private:
    Hardware* hardware;

    Chrono timer1;
    uint state = 1;
};
