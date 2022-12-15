#pragma once

#include "routines.h"
#include "Chrono.h"
#include "../services/hardware.h"

class TestRoutines : public Routine
{
public:
    TestRoutines(Hardware* _hardware)
    {
        hardware = _hardware;
    }

    const char* name() {
        return "test";
    };

    void setup() {
//        posX=random(0, hardware->displayEye->getDriver()->width());
//        posY=random(0, hardware->displayEye->getDriver()->height());

        timer0.start();
        timer1.start();
        timer2.start();
        timer3.start();
        timer4.start();
        timer5.start();
        timer6.start();
    };

    void before() {
          hardware->reset();
    };

    void tick() {
//        tickDisplayEye();
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
        posX=min(posX, (int) hardware->displayEye->getDriver()->width());
        posX=max(posX, 0);

        posY+=random(-5, 5);
        posY=min(posY, (int) hardware->displayEye->getDriver()->height());
        posY=max(posY, 0);

        timer2.restart();
        hardware->displayEye->getDriver()->fillScreen(random(0xffff));
        hardware->displayEye->getDriver()->setCursor(posX, posY);
        hardware->displayEye->getDriver()->setTextColor(random(0xffff), random(0xffff));
        hardware->displayEye->getDriver()->setTextSize(scaleX /* x scale */, scaleY /* y scale */, 4 /* pixel_margin */);
        hardware->displayEye->getDriver()->println("Hello World!");
    }

    void tickDisplayMini() {
        if (timer4.elapsed() < 10) {
            return;
        }
        timer4.restart();

        hardware->displayMini->getDriver()->clearBuffer();					// clear the internal memory
        char buf[16];
        sprintf(buf, "%lu", millis() / 100);
        hardware->displayMini->getDriver()->drawStr(0, 7, buf);
    };

    void tickLedStrip() {
        if (timer5.elapsed() < 100) {
            return;
        }
        timer5.restart();

        hardware->stripLeft->setColor(led, CRGB::White);
        hardware->stripRight->setColor(led, CRGB::White);
        hardware->stripLipLeft->setColor(led, CRGB::White);
        hardware->stripLipRight->setColor(led, CRGB::White);
        led++;

        if (led > hardware->stripLeft->getCount()) {
            hardware->stripLeft->setAll(CRGB::Black);
            hardware->stripRight->setAll(CRGB::Black);
            hardware->stripLipLeft->setAll(CRGB::Black);
            hardware->stripLipRight->setAll(CRGB::Black);
            led = 0;
        }
    }

    void tickLeds() {
        if (timer6.elapsed() < 10) {
            return;
        }
        timer6.restart();

        if (hardware->batteryGlow->getBrightness() < 255) {
            hardware->batteryGlow->setBrightness(hardware->batteryGlow->getBrightness() + 1);
        } else if (hardware->eyeGlow->getBrightness() < 255) {
            hardware->eyeGlow->setBrightness(hardware->eyeGlow->getBrightness() + 1);
        } else {
            hardware->batteryGlow->setBrightness(0);
            hardware->eyeGlow->setBrightness(0);
        }

        hardware->lensLed1->setColor(CRGB( hardware->batteryGlow->getBrightness(), hardware->eyeGlow->getBrightness(), 255));
        hardware->lensLed2->setColor(CRGB( hardware->eyeGlow->getBrightness(), 255, hardware->batteryGlow->getBrightness()));
        hardware->lensLed3->setColor(CRGB( 255, hardware->batteryGlow->getBrightness(), hardware->eyeGlow->getBrightness()));
    }

    Hardware* hardware;

    int posX=0;
    int posY=0;
    int scaleX=2;
    int scaleY=2;
    int led=0;

    Chrono timer0;
    Chrono timer1;
    Chrono timer2;
    Chrono timer3;
    Chrono timer4;
    Chrono timer5;
    Chrono timer6;
};
