#include <Arduino.h>
#include "Wire.h"

#include <ESP32Servo.h>
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

#include "FastLED.h"
#define LED_DATA_PIN       33
#define LED_NUM      150
#define LED_TYPE   WS2812B
#define LED_COLOR_ORDER   GRB
#define LED_VOLTS          3
#define LED_MAX_MA       200
CRGB leds[LED_NUM];

// Circular TFT display
#define PIN_TFT_DC 14
#define PIN_TFT_CS DF_GFX_CS

// PWM LED Pins
#define PIN_LED_1 17
#define PIN_LED_2 16
#define PIN_LED_3 4

// PWM Servo Pins
#define PIN_SERVO1 27
#define PIN_SERVO2 25
#define PIN_SERVO3 32
#define PIN_SERVO4 12

// Circular Display Graphics
#include <U8g2lib.h>
U8G2_SSD1306_64X32_1F_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, SCL, SDA);

#include <Arduino_GFX_Library.h>
/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_ESP32SPI(PIN_TFT_DC, PIN_TFT_CS);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 2 /* RST */, 0 /* rotation */, true /* IPS */);

#include "routines/tests.h"
TestRoutines testRoutines(
  BUILTIN_LED,
  PIN_LED_1,
  PIN_LED_2,
  PIN_LED_3,
  &u8g2,
  gfx,
  &servo1,
  &servo2,
  &servo3,
  &servo4,
  leds,
  LED_NUM
);

// Filesystem
#include "LittleFS.h"

// Webserver
#include "ESPAsyncWebServer.h"
AsyncWebServer webServer(80);

// HTTP APIs
#include "web/eyeball-api.h"
EyeballApi eyeballApi;
#include "web/fin-api.h"
FinApi finApi;
#include "web/head-sides-api.h"
HeadSidesApi headSidesApi;
#include "web/lens-lights-api.h"
LensLightsApi lensLightsApi;
#include "web/mini-display-api.h"
MiniDisplayApi miniDisplayApi;
#include "web/misc-api.h"
MiscApi miscApi;
#include "web/routines-api.h"
RoutinesApi routinesApi;
#include "web/index-page.h"
IndexPage indexPage;
#include "web/not-found-handler.h"
NotFoundHandler notFoundHandler;

// Wifi DNS server for Captive Portal
#include <DNSServer.h>
DNSServer dnsServer;

// Wifi Services
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "services/wifi-service.h"
#include "web/captive-request-handler.h"
WifiService wifiService(&dnsServer);
CaptiveRequestHandler captiveRequestHandler;

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

  pinMode(PIN_SERVO1, OUTPUT);
  pinMode(PIN_SERVO2, OUTPUT);
  pinMode(PIN_SERVO3, OUTPUT);
  pinMode(PIN_SERVO4, OUTPUT);

  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);
  servo3.attach(PIN_SERVO3);
  servo4.attach(PIN_SERVO4);

  testRoutines.begin();

  // Captive Portal
  webServer.addHandler(&captiveRequestHandler).setFilter(ON_AP_FILTER);//only when requested from AP

  // Attach HTTP Handlers
  eyeballApi.attach(&webServer);;
  finApi.attach(&webServer);;
  headSidesApi.attach(&webServer);;
  lensLightsApi.attach(&webServer);;
  miniDisplayApi.attach(&webServer);;
  miscApi.attach(&webServer);;
  routinesApi.attach(&webServer);;
  indexPage.attach(&webServer);
  notFoundHandler.attach(&webServer);

  // Wifi + DNS
  wifiService.begin();

  // Webserver
  webServer.begin();

  Serial.println("booted.");
}

void loop() {
  dnsServer.processNextRequest();
  testRoutines.tickDebugLed();
  testRoutines.tickOledGlitch();
//  testRoutines.tickServo1();
  testRoutines.tickOledMini();
  testRoutines.tickLedSeq();
  testRoutines.tickLed1();
}