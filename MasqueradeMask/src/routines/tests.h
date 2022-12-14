#pragma once

#include "routines.h"
#include "Chrono.h"
#include "../part/display-eye.h"
#include "../part/display-mini.h"
#include "../part/led-series.h"
#include "../part/led-single.h"
#include "../part/pwm-led.h"

class TestRoutines : public Routine
{
public:
    TestRoutines(
        LedSingle* _lensLed1,
        LedSingle* _lensLed2,
        LedSingle* _lensLed3,
        PwmLed* _batteryGlow,
        PwmLed* _eyeGlow,
        LedSeries* _stripLeft,
        LedSeries* _stripRight,
        LedSeries* _stripLeftLow,
        LedSeries* _stripRightLow,
        DisplayMini* _displayMini,
        DisplayEye* _displayEye
)
{
        lensLed1 = _lensLed1;
        lensLed2 = _lensLed2;
        lensLed3 = _lensLed3;
        batteryGlow = _batteryGlow;
        eyeGlow = _eyeGlow;
        stripLeft = _stripLeft;
        stripRight = _stripRight;
        stripLeftLow = _stripLeftLow;
        stripRightLow = _stripRightLow;
        displayMini = _displayMini;
        displayEye = _displayEye;
}

    const char* name() {
        return "test";
    };

    void setup() {
        posX=random(0, displayEye->getDriver()->width());
        posY=random(0, displayEye->getDriver()->height());

        timer0.start();
        timer1.start();
        timer2.start();
        timer3.start();
        timer4.start();
        timer5.start();
        timer6.start();
    };

    void before() {
          displayMini->getDriver()->clear();
    };

    void tick() {
        tickDisplayEye();
        tickDisplayMini();
        tickLedStrip();
        tickLeds();
    };

private:
    void tickDisplayEye()
    {
        if (timer2.elapsed() < 50) {
            return;
        }
        scaleX+=random(-1, 1);
        scaleX=min(scaleX, 5);
        scaleX=max(scaleX, 2);

        scaleY+=random(-1, 1);
        scaleY=min(scaleY, 7);
        scaleY=max(scaleY, 2);

        posX+=random(-5, 5);
        posX=min(posX, (int) displayEye->getDriver()->width());
        posX=max(posX, 0);

        posY+=random(-5, 5);
        posY=min(posY, (int) displayEye->getDriver()->height());
        posY=max(posY, 0);

        timer2.restart();
        displayEye->getDriver()->fillScreen(random(0xffff));
        displayEye->getDriver()->setCursor(posX, posY);
        displayEye->getDriver()->setTextColor(random(0xffff), random(0xffff));
        displayEye->getDriver()->setTextSize(scaleX /* x scale */, scaleY /* y scale */, 4 /* pixel_margin */);
        displayEye->getDriver()->println("Hello World!");
    }

    void tickDisplayMini() {
        if (timer4.elapsed() < 100) {
            return;
        }
        timer4.restart();

            displayMini->getDriver()->clearBuffer();					// clear the internal memory
        char buf[16];
        sprintf(buf, "%lu", millis() / 1000);
        displayMini->getDriver()->drawStr(0, 7, buf);
    };

    void tickLedStrip() {
        if (timer5.elapsed() < 500) {
            return;
        }
        timer5.restart();

        stripLeft->setColor(led, CRGB::White);
        stripRight->setColor(led, CRGB::White);
        stripLeftLow->setColor(led, CRGB::White);
        stripRightLow->setColor(led, CRGB::White);
        led++;

        if (led > stripLeft->getCount()) {
            stripLeft->setAll(CRGB::Black);
            stripRight->setAll(CRGB::Black);
            stripLeftLow->setAll(CRGB::Black);
            stripRightLow->setAll(CRGB::Black);
            led = 0;
        }
    }

    void tickLeds() {
        if (timer6.elapsed() < 10) {
            return;
        }
        timer6.restart();

        if (batteryGlow->getBrightness() < 255) {
            batteryGlow->setBrightness(batteryGlow->getBrightness() + 1);
        } else if (eyeGlow->getBrightness() < 255) {
            eyeGlow->setBrightness(eyeGlow->getBrightness() + 1);
        } else {
            batteryGlow->setBrightness(0);
            eyeGlow->setBrightness(0);
        }

        lensLed1->setColor(CRGB( batteryGlow->getBrightness(), eyeGlow->getBrightness(), 255));
        lensLed2->setColor(CRGB( eyeGlow->getBrightness(), 255, batteryGlow->getBrightness()));
        lensLed3->setColor(CRGB( 255, batteryGlow->getBrightness(), eyeGlow->getBrightness()));
        }


    PwmLed* batteryGlow;
    PwmLed* eyeGlow;

    LedSingle* lensLed1;
    LedSingle* lensLed2;
    LedSingle* lensLed3;

    DisplayEye* displayEye;
    int posX=0;
    int posY=0;
    int scaleX=2;
    int scaleY=2;

    DisplayMini* displayMini;
    
    LedSeries* stripLeft;
    LedSeries* stripRight;
    LedSeries* stripLeftLow;
    LedSeries* stripRightLow;
    int led=0;

    Chrono timer0;
    Chrono timer1;
    Chrono timer2;
    Chrono timer3;
    Chrono timer4;
    Chrono timer5;
    Chrono timer6;
};
