#pragma once

#include "Chrono.h"
#include <Arduino_GFX_Library.h>
#include <U8g2lib.h>
#include "FastLED.h"
#include <ESP32Servo.h>
#include <arduinoFFT.h>

class TestRoutines
{
public:
    TestRoutines(
        uint debugLed,
        uint pwmLed1,
        uint pwmLed2,
        uint pwmLed3,
        U8G2_SSD1306_64X32_1F_F_HW_I2C* u8g2,
        Arduino_GFX* gfx,
        Servo* servo1,
        Servo* servo2,
        Servo* servo3,
        Servo* servo4,
        CRGB* leds,
        uint ledCount
    );
    void begin();

    void tickDebugLed();
    void tickOledGlitch();
    void tickServo1();
    void tickOledMini();
    void tickLedSeq();
    void tickLed1();
    void tickMic();

private:

    uint pinDebugLed;
    
    uint pinPwmLed1;
    uint pinPwmLed2;
    uint pinPwmLed3;
    int led1Brightness = 0;
    int led2Brightness = 0;
    int led3Brightness = 0;

    U8G2_SSD1306_64X32_1F_F_HW_I2C* u8g2;
    int posX=0;
    int posY=0;
    int scaleX=2;
    int scaleY=2;

    Arduino_GFX* gfx;
    
    Servo* servo1;
    Servo* servo2;
    Servo* servo3;
    Servo* servo4;
    int servoState=0;

    CRGB* leds;
    uint ledCount;
    int led=0;

    Chrono timer0;
    Chrono timer1;
    Chrono timer2;
    Chrono timer3;
    Chrono timer4;
    Chrono timer5;
    Chrono timer6;
};
