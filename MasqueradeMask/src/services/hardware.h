#pragma once

#include "../part/button-touch.h"
#include "../part/display-mini.h"
#include "../part/led-strip.h"
#include "../part/led-single.h"
#include "../part/led-series.h"
#include "../part/pwm-led.h"
#include "../part/display-eye.h"

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
    DisplayEye* displayEye;

    void tick() {
        displayMini->tick();
        ledStrip->tick();
        buttonLeft->tick();
        buttonRight->tick();
    }

    void reset()
    {
        displayMini->getDriver()->clearBuffer();
        displayMini->getDriver()->flush();
        
        batteryGlow->setBrightness(0);
        eyeGlow->setBrightness(0);
        lensLed1->setColor(0);
        lensLed2->setColor(0);
        lensLed3->setColor(0);
        stripLeft->setAll(0);
        stripRight->setAll(0);
        stripLipLeft->setAll(0);
        stripLipRight->setAll(0);
//        displayEye->getDriver()->
    }
};
