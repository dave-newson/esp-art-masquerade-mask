#include "tests.h"
#include "math.h"
#include "Arduino.h"

TestRoutines::TestRoutines(
    uint debugLed,
    uint pwmLed1,
    uint pwmLed2,
    uint pwmLed3,
    U8G2_SSD1306_64X32_1F_F_HW_I2C* _u8g2,
    Arduino_GFX* _gfx,
    Servo* _servo1,
    Servo* _servo2,
    Servo* _servo3,
    Servo* _servo4,
    CRGB* _leds,
    uint _ledCount
)
{
    pinDebugLed = debugLed;
    pinPwmLed1 = pwmLed1;
    pinPwmLed2 = pwmLed2;
    pinPwmLed3 = pwmLed3;
    u8g2 = _u8g2;
    gfx = _gfx;
    servo1 = _servo1;
    servo2 = _servo2;
    servo3 = _servo3;
    servo4 = _servo4;
    leds = _leds;
    ledCount = _ledCount;
}

void TestRoutines::begin()
{
    posX=random(0, gfx->width());
    posY=random(0, gfx->height());

    timer0.start();
    timer1.start();
    timer2.start();
    timer3.start();
    timer4.start();
    timer5.start();
    timer6.start();
}

void TestRoutines::tickDebugLed()
{
  if (timer0.elapsed() < 500) return;

  timer0.restart();
  digitalWrite(pinDebugLed, !digitalRead(pinDebugLed));
}

void TestRoutines::tickLed1()
{
  if (timer6.elapsed() < 10) {
    return;
  }
  timer6.restart();

  if (led1Brightness < 255) {
    led1Brightness++;
  } else if (led2Brightness < 255) {
    led2Brightness++;
  } else if (led3Brightness < 255) {
    led3Brightness++;
  } else {
    led1Brightness = 0;
    led2Brightness = 0;
    led3Brightness = 0;
  }

  analogWrite(pinPwmLed1, led1Brightness);
  analogWrite(pinPwmLed2, led2Brightness);
  analogWrite(pinPwmLed3, led3Brightness);
}

void TestRoutines::tickLedSeq()
{
  if (timer5.elapsed() < 500) {
    return;
  }
  timer5.restart();
 
  leds[led] = CRGB::White;
  led++;

  if (led > ledCount) {
    led = 0;
    FastLED.clear();
  }
  FastLED.show();
}

void TestRoutines::tickOledMini()
{
  if (timer4.elapsed() < 100) {
    return;
  }
  timer4.restart();

  u8g2->clearBuffer();					// clear the internal memory
  char buf[16];
  sprintf(buf, "%lu", millis() / 1000);
  u8g2->drawStr(0, 7, buf);
  u8g2->sendBuffer();
}

void TestRoutines::tickOledGlitch()
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
  posX=min(posX, (int) gfx->width());
  posX=max(posX, 0);

  posY+=random(-5, 5);
  posY=min(posY, (int) gfx->height());
  posY=max(posY, 0);

  timer2.restart();
  gfx->fillScreen(random(0xffff));
  gfx->setCursor(posX, posY);
  gfx->setTextColor(random(0xffff), random(0xffff));
  gfx->setTextSize(scaleX /* x scale */, scaleY /* y scale */, 4 /* pixel_margin */);
  gfx->println("Hello World!");
}


void TestRoutines::tickServo1()
{
  if (timer3.elapsed() < 1000) {
    return;
  }

  timer3.restart();

  if (servoState == 1) {
    servo1->write(0);
    servo2->write(0);
    servo3->write(0);
    servo4->write(0);
  } else if (servoState == 2) {
    servo1->write(180);
  } else if (servoState == 3) {
    servo2->write(180);
  } else if (servoState == 4) {
    servo3->write(180);
  } else if (servoState == 5) {
    servo4->write(180);
    servoState = 0;
  }
  servoState++;
}

