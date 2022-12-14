#pragma once

#include "../part/button-touch.h"
#include "../part/display-mini.h"
#include "../part/led-strip.h"
#include "../part/led-single.h"
#include "../part/led-series.h"
#include "../part/pwm-led.h"

// This class eixsts so I can be lazy and not #include each component in every routine.
class Hardware
{

public:

    PwmLed* batteryGlow;
    PwmLed* eyeGlow;
    LedSingle* lensLed1;
    LedSingle* lensLed2;
    LedSingle* lensLed3;
    DisplayMini* displayMini;   
    LedStrip* ledStrip; 
    LedSeries* stripLeft;
    LedSeries* stripRight;
    LedSeries* stripLipLeft;
    LedSeries* stripLipRight;
    ButtonTouch* buttonLeft;
    ButtonTouch* buttonRight;

    void tick() {
        displayMini->tick();
        ledStrip->tick();
        buttonLeft->tick();
        buttonRight->tick();
    }
};
