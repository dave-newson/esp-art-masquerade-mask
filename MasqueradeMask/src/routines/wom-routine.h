
#pragma once

#include "routines.h"
#include "../part/led-series.h"
#include "../part/led-single.h"
#include "../part/pwm-led.h"
#include "../part/display-mini.h"
#include "Chrono.h"

class WomRoutine : public Routine
{
public:
    WomRoutine(
        DisplayMini* _displayMini,
        LedSeries* _left,
        LedSeries* _right,
        LedSeries* _leftLow,
        LedSeries* _rightLow,
        LedSingle* _led1,
        LedSingle* _led2,
        LedSingle* _led3,
        PwmLed* _led4,
        PwmLed* _led5
    ) {
        displayMini = _displayMini;
        left = _left;
        right = _right;
        leftLow = _leftLow;
        rightLow = _rightLow;
        led1 = _led1;
        led2 = _led2;
        led3 = _led3;
        led4 = _led4;
        led5 = _led5;
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

            left->setAll(CRGB(ledBrightness, 0, 0));
            right->setAll(CRGB(0, 0, ledBrightness));
            leftLow->setAll(CRGB(0, ledBrightness, 0));
            rightLow->setAll(CRGB(0, ledBrightness, 0));
        }

        if (ledBrightness >= 255) {
            direction = -1;
        } else if (ledBrightness < 1) {
            direction = 1;
        }

        ledBrightness += direction;
    }

private:
    DisplayMini* displayMini;
    LedSeries* left;
    LedSeries* right;
    LedSeries* leftLow;
    LedSeries* rightLow;
    LedSingle* led1;
    LedSingle* led2;
    LedSingle* led3;
    PwmLed* led4;
    PwmLed* led5;

    Chrono timer;
    int direction = 1;
    int ledBrightness = 0;
    uint servoAngle = 0;
};
