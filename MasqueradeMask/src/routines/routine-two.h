
#pragma once

#include "routines.h"
#include "../part/display-mini.h"

class RoutineTwo : public Routine
{
public:
    RoutineTwo(DisplayMini* _displayMini) {
        displayMini = _displayMini;
    }

    const char* name() {
        return "test-two";
    }

    void before() {
        displayMini->getDriver()->drawStr(0, 7, "Hello World");
    }

    void tick() {
        // Noop
    }

private:
    DisplayMini* displayMini;
};
