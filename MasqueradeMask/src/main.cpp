#include <Arduino.h>
#include "Wire.h"

#include "Servo.h"
Servo servo;

#include "Chrono.h"
Chrono timer1;
Chrono timer2;
Chrono timer3;
Chrono timer4;

#include "LittleFS.h"

#include <U8g2lib.h>
U8G2_SSD1306_64X32_1F_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, SCL, SDA);

#include "PCF8575.h"
PCF8575 pcf8575(0x20);

#include <Arduino_GFX_Library.h>
/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_ESP8266SPI(0, DF_GFX_CS);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 2 /* RST */, 0 /* rotation */, true /* IPS */);

int posX=0;
int posY=0;
int scaleX=2;
int scaleY=2;

void setup() {
  Serial.begin(9600);
  Serial.println("booting...");

  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);

  Wire.begin();

  u8g2.begin();
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_profont11_tf);	// choose a suitable font
  u8g2.drawStr(0, 7, "Hello");	 // write something to the internal memory
  u8g2.drawStr(0, 14, "World...");
  u8g2.sendBuffer();

  pcf8575.begin();
	pcf8575.pinMode(1, OUTPUT);
	pcf8575.pinMode(3, OUTPUT);
	pcf8575.pinMode(4, OUTPUT);

  gfx->begin();
  gfx->fillScreen(WHITE);
  gfx->setCursor(20, 20);
  gfx->setTextColor(BLACK);
  gfx->setTextSize(3);
  gfx->println("BOOTING");
  delay(1000);

  posX=random(0, gfx->width());
  posY=random(0, gfx->height());

  servo.attach(16);

  timer1.start();
  timer2.start();
  timer3.start();
  timer4.start();

  Serial.println("booted.");
}

void tickOledGlitch();
void tickServo1();
void tickLedStrip();
void tickOledMini();

void loop() {

  tickOledGlitch();
  tickServo1();
  tickLedStrip();
  tickOledMini();
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

int ledState = false;
void tickLedStrip()
{
  if (timer1.elapsed() < 500) {
    return;
  }

  Serial.println("blink");
  timer1.restart();
  ledState = !ledState;
  pcf8575.digitalWrite(1, ledState ? HIGH : LOW);
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
  servoState++;
  servoState = (servoState > 3) ? 0 : servoState;

  if (servoState == 0) {
    servo.write(0);
  } else if (servoState == 1) {
    servo.write(60);
  } else if (servoState == 2) {
    servo.write(120);
  } else if (servoState == 3) {
    servo.write(180);
  }
}
