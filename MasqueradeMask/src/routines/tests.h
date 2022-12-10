#pragma once

#include "Chrono.h"
#include "../part/display-eye.h"
#include "../part/display-mini.h"
#include "../part/fins.h"
#include "../part/led-series.h"
#include "../part/led-single.h"
#include "../part/pwm-led.h"

class TestRoutines
{
public:
    TestRoutines(
        LedSingle* lensLed1,
        LedSingle* lensLed2,
        LedSingle* lensLed3,
        PwmLed* batteryGlow,
        PwmLed* eyeGlow,
        PwmLed* finsGlow,
        LedSeries* stripLeft,
        LedSeries* stripRight,
        Fins* fins,
        DisplayMini* displayMini,
        DisplayEye* displayEye
    );
    void begin();

    void tick() {
        tickDisplayEye();
        tickFins();
        tickDisplayMini();
        tickLedStrip();
        tickLeds();
    };

private:
    void tickDisplayEye();
    void tickFins();
    void tickDisplayMini();
    void tickLedStrip();
    void tickLeds();

    PwmLed* batteryGlow;
    PwmLed* eyeGlow;
    PwmLed* finsGlow;

    LedSingle* lensLed1;
    LedSingle* lensLed2;
    LedSingle* lensLed3;

    DisplayEye* displayEye;
    int posX=0;
    int posY=0;
    int scaleX=2;
    int scaleY=2;

    DisplayMini* displayMini;
    
    Fins* fins;
    int servoState=0;

    LedSeries* stripLeft;
    LedSeries* stripRight;
    int led=0;

    Chrono timer0;
    Chrono timer1;
    Chrono timer2;
    Chrono timer3;
    Chrono timer4;
    Chrono timer5;
    Chrono timer6;
};
