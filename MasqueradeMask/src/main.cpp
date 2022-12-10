#include <Arduino.h>

// Peripherals
#include "Wire.h"
#include "LittleFS.h"

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
#include <ESP32Servo.h>
#define SERVO_COUNT 4
Servo servo[SERVO_COUNT];
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

// Component Abstractions -------------------------------------
#include "part/fins.h"
Fins fins(servo, SERVO_COUNT);

#include "part/led-strip.h"
LedStrip ledStrip(leds, LED_NUM);

#include "part/led-series.h"
LedSeries stripLeft(&ledStrip, 10, 20);
LedSeries stripRight(&ledStrip, 20, 30);

#include "part/led-single.h"
LedSingle lensLed1(&ledStrip, 1);
LedSingle lensLed2(&ledStrip, 2);
LedSingle lensLed3(&ledStrip, 3);

#include "part/pwm-led.h"
PwmLed batteryGlow(PIN_LED_1);
PwmLed eyeGlow(PIN_LED_2);
PwmLed finsGlow(PIN_LED_3);

#include "part/display-eye.h"
DisplayEye displayEye(gfx);

#include "part/display-mini.h"
DisplayMini displayMini(&u8g2);

// Routines ---------------------------------------------------
#include "routines/routines.h"
#define ROUTINE_COUNT 2
Routine* routines[ROUTINE_COUNT];
RoutineController routineController(routines, ROUTINE_COUNT);

#include "routines/tests.h"
TestRoutines testRoutine(
  &lensLed1,
  &lensLed2,
  &lensLed3,
  &batteryGlow,
  &eyeGlow,
  &finsGlow,
  &stripLeft,
  &stripRight,
  &fins,
  &displayMini,
  &displayEye
);

#include "routines/routine-two.h"
RoutineTwo routineTwo(&displayMini);

// Webserver --------------------------------------------------
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
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(9600);
  Serial.println("booting...");

  delay(1000);
/*
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
*/

  // Peripherals
  pinMode(PIN_SERVO1, OUTPUT);
  pinMode(PIN_SERVO2, OUTPUT);
  pinMode(PIN_SERVO3, OUTPUT);
  pinMode(PIN_SERVO4, OUTPUT);

  servo[0].attach(PIN_SERVO1);
  servo[1].attach(PIN_SERVO2);
  servo[2].attach(PIN_SERVO3);
  servo[3].attach(PIN_SERVO4);

//  FastLED.setMaxPowerInVoltsAndMilliamps(LED_VOLTS, LED_MAX_MA);
  FastLED.addLeds<LED_TYPE,LED_DATA_PIN,LED_COLOR_ORDER>(leds, LED_NUM)
    .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(10);
  ledStrip.begin();

  // I2C
  Wire.begin();

  // Displays
  u8g2.begin();
  gfx->begin();

  // Components
  fins.begin();
  ledStrip.begin();
  lensLed1.begin();
  lensLed2.begin();
  lensLed3.begin();
  displayEye.begin();
  displayMini.begin();
  stripLeft.begin();
  stripRight.begin();

  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_profont11_tf);	// choose a suitable font
  u8g2.drawStr(0, 7, "Hello");	 // write something to the internal memory
  u8g2.drawStr(0, 14, "World...");
  u8g2.sendBuffer();

  gfx->fillScreen(WHITE);
  gfx->setCursor(20, 20);
  gfx->setTextColor(BLACK);
  gfx->setTextSize(3);
  gfx->println("BOOTING");

  // Routines
  routines[0] = &testRoutine;
  routines[1] = &routineTwo;

  routineController.begin();
  routineController.setRoutine(&testRoutine);

  // Wifi + DNS
  wifiService.begin();

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

  // Webserver
  webServer.begin();

  // Done
  Serial.println("booted.");
  digitalWrite(LED_BUILTIN, LOW);
}

#include "Chrono.h"
Chrono debugLedTimer;

void tickDebugLed()
{
    if (debugLedTimer.elapsed() < 100) {
      return;
    }

    debugLedTimer.restart();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

Chrono timer;

void loop() {
  tickDebugLed();
  dnsServer.processNextRequest();
  routineController.tick();
 
  // Component ticks
  displayMini.tick();
  ledStrip.tick();

  if (timer.elapsed() > 10000) {
    timer.restart();
    timer.stop();

    routineController.setRoutine("test-two");
  }
}
