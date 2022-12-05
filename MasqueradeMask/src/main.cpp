#include <Arduino.h>
#include "Wire.h"

#include <ESP32Servo.h>
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

#include "Chrono.h"
Chrono timer0;
Chrono timer1;
Chrono timer2;
Chrono timer3;
Chrono timer4;
Chrono timer5;
Chrono timer6;

#include "FastLED.h"
#define LED_DATA_PIN       33
#define LED_NUM      150
#define LED_TYPE   WS2812B
#define LED_COLOR_ORDER   GRB
#define LED_VOLTS          3
#define LED_MAX_MA       200
CRGBArray<LED_NUM> leds;

#define PIN_TFT_DC 14
#define PIN_TFT_CS DF_GFX_CS

#define PIN_LED_1 17
#define PIN_LED_2 16
#define PIN_LED_3 4

#define PIN_SERVO1 27
#define PIN_SERVO2 25
#define PIN_SERVO3 32
#define PIN_SERVO4 12

#include "LittleFS.h"

#include <U8g2lib.h>
U8G2_SSD1306_64X32_1F_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, SCL, SDA);

#include <Arduino_GFX_Library.h>
/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_ESP32SPI(PIN_TFT_DC, PIN_TFT_CS);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 2 /* RST */, 0 /* rotation */, true /* IPS */);

int posX=0;
int posY=0;
int scaleX=2;
int scaleY=2;

void setup() {
  Serial.begin(9600);
  Serial.println("booting...");
/*
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
*/
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);

  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  digitalWrite(PIN_LED_1, LOW);
  digitalWrite(PIN_LED_2, LOW);
  digitalWrite(PIN_LED_3, LOW);

//  FastLED.setMaxPowerInVoltsAndMilliamps(LED_VOLTS, LED_MAX_MA);
  FastLED.addLeds<LED_TYPE,LED_DATA_PIN,LED_COLOR_ORDER>(leds, LED_NUM)
    .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(10);

  Wire.begin();

  u8g2.begin();
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_profont11_tf);	// choose a suitable font
  u8g2.drawStr(0, 7, "Hello");	 // write something to the internal memory
  u8g2.drawStr(0, 14, "World...");
  u8g2.sendBuffer();

  gfx->begin();
  gfx->fillScreen(WHITE);
  gfx->setCursor(20, 20);
  gfx->setTextColor(BLACK);
  gfx->setTextSize(3);
  gfx->println("BOOTING");
  delay(1000);

  posX=random(0, gfx->width());
  posY=random(0, gfx->height());

  pinMode(PIN_SERVO1, OUTPUT);
  pinMode(PIN_SERVO2, OUTPUT);
  pinMode(PIN_SERVO3, OUTPUT);
  pinMode(PIN_SERVO4, OUTPUT);

  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);
  servo3.attach(PIN_SERVO3);
  servo4.attach(PIN_SERVO4);

  timer0.start();
  timer1.start();
  timer2.start();
  timer3.start();
  timer4.start();
  timer5.start();
  timer6.start();

  Serial.println("booted.");
}

void tickOledGlitch();
void tickServo1();
void tickOledMini();
void tickLedSeq();
void tickLed1();
void tickDebugLed();

void loop() {
  tickDebugLed();
  tickOledGlitch();
  tickServo1();
  tickOledMini();
  tickLedSeq();
  tickLed1();
}

void tickDebugLed()
{
  if (timer0.elapsed() < 500) return;

  timer0.restart();
  digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
}

int led1Brightness = 0;
int led2Brightness = 0;
int led3Brightness = 0;
void tickLed1()
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

  analogWrite(PIN_LED_1, led1Brightness);
  analogWrite(PIN_LED_2, led2Brightness);
  analogWrite(PIN_LED_3, led3Brightness);
}


int led=0;
void tickLedSeq()
{
  if (timer5.elapsed() < 500) {
    return;
  }
  timer5.restart();
 
  leds[led] = CRGB::White;
  led++;

  if (led > LED_NUM) {
    led = 0;
    FastLED.clear();
  }
  FastLED.show();
}

void tickOledMini()
{
  if (timer4.elapsed() < 100) {
    return;
  }
  timer4.restart();

  u8g2.clearBuffer();					// clear the internal memory
  char buf[16];
  sprintf(buf, "%lu", millis() / 1000);
  u8g2.drawStr(0, 7, buf);
  u8g2.sendBuffer();
}

void tickOledGlitch()
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


int servoState=0;
void tickServo1()
{
  if (timer3.elapsed() < 1000) {
    return;
  }

  timer3.restart();

  if (servoState == 1) {
    servo1.write(0);
    servo2.write(0);
    servo3.write(0);
    servo4.write(0);
  } else if (servoState == 2) {
    servo1.write(180);
  } else if (servoState == 3) {
    servo2.write(180);
  } else if (servoState == 4) {
    servo3.write(180);
  } else if (servoState == 5) {
    servo4.write(180);
    servoState = 0;
  }
  servoState++;
}
