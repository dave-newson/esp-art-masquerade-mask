#pragma once

#include "Arduino.h"

class Routine
{
public:
    virtual const char* name() = 0;

    virtual void setup() {};

    virtual void before() {};
    virtual void tick() {};
    virtual void after() {};
};

class RoutineController {

private:
    Routine** routines;

    uint routineCount;

    uint current = 0;

    Routine* active;

public:
    RoutineController(Routine** _routines, uint _routineCount) {
        routines = _routines;
        routineCount = _routineCount;
    }

    void begin() {
        for (uint i = 0; i < routineCount; i++) {
            routines[i]->setup();
        }
    }

    void tick() {
        if (active) {
            active->tick();
        }
    }

    void setRoutine(Routine* routine) {
        if (routine == nullptr) {
            return;
        }

        if (active) {
            active->after();
        }

        active = routine;

        active->before();
    }

    void setRoutine(const char* name) {
        for (uint i = 0; i < routineCount; i++) {
            // Change routine
            if (strcmp(routines[i]->name(), name) == 0) {
                current = i;
                setRoutine(routines[i]);
                return;
            }
        }
    }

    void next() {
        current++;
        current = (current >= routineCount) ? 0 : current;
        Routine* next = routines[current];
        setRoutine(next);
    }
};
