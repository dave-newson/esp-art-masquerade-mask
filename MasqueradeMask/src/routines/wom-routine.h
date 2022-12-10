
#pragma once

#include "routines.h"
#include "../part/fins.h"
#include "../part/led-series.h"
#include "../part/led-single.h"
#include "../part/pwm-led.h"
#include "../part/display-mini.h"
#include "Chrono.h"

class WomRoutine : public Routine
{
public:
    WomRoutine(DisplayMini* _displayMini, Fins* _fins, LedSeries* _left, LedSeries* _right, LedSingle* _led1, LedSingle* _led2, LedSingle* _led3, PwmLed* _led4, PwmLed* _led5, PwmLed* _led6) {
        displayMini = _displayMini;
        fins = _fins;
        left = _left;
        right = _right;
        led1 = _led1;
        led2 = _led2;
        led3 = _led3;
        led4 = _led4;
        led5 = _led5;
        led6 = _led6;
    }

    const char* name() {
        return "wom";
    }

    void before() {
        displayMini->getDriver()->clear();
    }

    void tick() {
        if (timer.elapsed() > 10) {
            timer.restart();

            char buf[16];
            sprintf(buf, "%lu", ledBrightness);
            displayMini->getDriver()->drawStr(0, 7, buf);

            led1->setColor(ledBrightness);
            led2->setColor(ledBrightness);
            led3->setColor(ledBrightness);
            led4->setBrightness(ledBrightness);
            led5->setBrightness(ledBrightness);
            led6->setBrightness(ledBrightness);
        }

        if (ledBrightness > 255) {
            direction = -1;
        } else if (ledBrightness < 0) {
            direction = 1;
        }

        ledBrightness += direction;
    }

private:
    DisplayMini* displayMini;
    Fins* fins;
    LedSeries* left;
    LedSeries* right;
    LedSingle* led1;
    LedSingle* led2;
    LedSingle* led3;
    PwmLed* led4;
    PwmLed* led5;
    PwmLed* led6;

    Chrono timer;
    int direction = 1;
    int ledBrightness = 0;
};
