#include "tests.h"
#include "math.h"
#include "Arduino.h"

TestRoutines::TestRoutines(
        LedSingle* _lensLed1,
        LedSingle* _lensLed2,
        LedSingle* _lensLed3,
        PwmLed* _batteryGlow,
        PwmLed* _eyeGlow,
        PwmLed* _finsGlow,
        LedSeries* _stripLeft,
        LedSeries* _stripRight,
        Fins* _fins,
        DisplayMini* _displayMini,
        DisplayEye* _displayEye
)
{
        lensLed1 = _lensLed1;
        lensLed2 = _lensLed2;
        lensLed3 = _lensLed3;
        batteryGlow = _batteryGlow;
        eyeGlow = _eyeGlow;
        finsGlow = _finsGlow;
        stripLeft = _stripLeft;
        stripRight = _stripRight;
        fins = _fins;
        displayMini = _displayMini;
        displayEye = _displayEye;
}

const char* TestRoutines::name()
{
    return "test";
}

void TestRoutines::setup()
{
    posX=random(0, displayEye->getDriver()->width());
    posY=random(0, displayEye->getDriver()->height());

    timer0.start();
    timer1.start();
    timer2.start();
    timer3.start();
    timer4.start();
    timer5.start();
    timer6.start();
}

void TestRoutines::before()
{
  displayMini->getDriver()->clear();
}

void TestRoutines::tickLeds()
{
  if (timer6.elapsed() < 10) {
    return;
  }
  timer6.restart();

  if (batteryGlow->getBrightness() < 255) {
    batteryGlow->setBrightness(batteryGlow->getBrightness() + 1);
  } else if (eyeGlow->getBrightness() < 255) {
    eyeGlow->setBrightness(eyeGlow->getBrightness() + 1);
  } else if (finsGlow->getBrightness() < 255) {
    finsGlow->setBrightness(finsGlow->getBrightness() + 1);
  } else {
    batteryGlow->setBrightness(0);
    eyeGlow->setBrightness(0);
    finsGlow->setBrightness(0);
  }
  lensLed1->setColor(CRGB( batteryGlow->getBrightness(), eyeGlow->getBrightness(), finsGlow->getBrightness()));
  lensLed2->setColor(CRGB( eyeGlow->getBrightness(), finsGlow->getBrightness(), batteryGlow->getBrightness()));
  lensLed3->setColor(CRGB( finsGlow->getBrightness(), batteryGlow->getBrightness(), eyeGlow->getBrightness()));
}

void TestRoutines::tickLedStrip()
{
  if (timer5.elapsed() < 500) {
    return;
  }
  timer5.restart();

    stripLeft->setColor(led, CRGB::White);
    stripRight->setColor(led, CRGB::White);
    led++;

    if (led > stripLeft->getCount()) {
        stripLeft->setAll(CRGB::Black);
        stripRight->setAll(CRGB::Black);
        led = 0;
    }
}

void TestRoutines::tickDisplayMini()
{
  if (timer4.elapsed() < 100) {
    return;
  }
  timer4.restart();

    displayMini->getDriver()->clearBuffer();					// clear the internal memory
  char buf[16];
  sprintf(buf, "%lu", millis() / 1000);
  displayMini->getDriver()->drawStr(0, 7, buf);
}

void TestRoutines::tickDisplayEye()
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


void TestRoutines::tickFins()
{
  if (timer3.elapsed() < 1000) {
    return;
  }

  timer3.restart();

  if (servoState == 1) {
    fins->setAllAngle(0);
  } else if (servoState == 2) {
    fins->setServoAngle(0, 180);
  } else if (servoState == 3) {
    fins->setServoAngle(1, 180);
  } else if (servoState == 4) {
    fins->setServoAngle(2, 180);
  } else if (servoState == 5) {
    fins->setServoAngle(3, 180);
    servoState = 0;
  }
  servoState++;
}

