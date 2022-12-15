
#pragma once

#include "routines.h"
#include "../services/hardware.h"

class HelloWorldRoutine : public Routine
{
public:
    HelloWorldRoutine(Hardware* _hardware) {
        hardware = _hardware;
    }

    const char* name() {
        return "test-two";
    }

    void before() {
        hardware->reset();
        hardware->displayMini->getDriver()->drawStr(0, 7, "Hello World");
    }

    void tick() {
        // Noop
    }

private:
    Hardware* hardware;
};
