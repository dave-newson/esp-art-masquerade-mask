#pragma once

#include "Arduino.h"
#include "Chrono.h"

typedef std::function<void(void)> buttonFunc;

class ButtonTouch
{
    enum state {
        idle = 0,
        idleDebounce = 1,
        touch = 2,
        touchDebounce = 3
    };

public:
    ButtonTouch(uint _pin, int _threshold = 20)
    {
        pin = _pin;
        touchThreshold = _threshold;
    }

    void begin()
    {
        pinMode (pin, INPUT);
    }

    void tick()
    {
        if (status == state::idle) {
            status = state::touchDebounce;
            timer.restart();

            int touchValue = touchRead(pin);

            // Not touching
            if (touchThreshold < touchValue) {
                return;
            }

            status = state::touch;
            return;
        }

        if (status == state::touch) {
            int touchValue = touchRead(pin);

            // Still touching
            if (touchThreshold > touchValue) {
                return;
            }

            // Stopped touching
            if (holdCallback != nullptr && timer.elapsed() > holdThresholdMillis) {
                holdCallback();
            } else if (tapCallback != nullptr) {
                tapCallback();
            }

            status = state::touchDebounce;
            timer.restart();
            return;
        }

        if (status == state::idleDebounce && timer.elapsed() > 100) {
            status = state::idle;
            return;
        }

        if (status == state::touchDebounce && timer.elapsed() > 200) {
            status = state::idle;
            return;
        }

    }

    void setTouchCallback(buttonFunc callback) {
        tapCallback = callback;
    }

    void setHoldCallback(buttonFunc callback, uint _holdThresholdMillis) {
        holdCallback = callback;
        holdThresholdMillis = _holdThresholdMillis;
    }

private:
    uint pin;
    uint touchThreshold;
    uint status = state::idle;
    uint holdThresholdMillis;
    Chrono timer;
    buttonFunc tapCallback = nullptr;
    buttonFunc holdCallback = nullptr;
};
