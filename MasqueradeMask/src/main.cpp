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
#define PIN_LED_1 16
#define PIN_LED_2 4

#include "part/button-touch.h"
#define PIN_BUTTON_RIGHT 12
#define PIN_BUTTON_LEFT 32
ButtonTouch buttonRight(PIN_BUTTON_RIGHT, 20);
ButtonTouch buttonLeft(PIN_BUTTON_LEFT, 20);

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
#include "part/led-strip.h"
LedStrip ledStrip(leds, LED_NUM);

#include "part/led-series.h"
LedSeries stripRight(&ledStrip, 0, 22); // 22
LedSeries stripRightLow(&ledStrip, 22, 12); // 12
LedSeries stripLeftLow(&ledStrip, 34, 12); // 12
LedSeries stripLeft(&ledStrip, 46, 22); // 22

#include "part/led-single.h"
LedSingle lensLed1(&ledStrip, 68);
LedSingle lensLed2(&ledStrip, 69);
LedSingle lensLed3(&ledStrip, 70);

#include "part/pwm-led.h"
PwmLed batteryGlow(PIN_LED_1);
PwmLed eyeGlow(PIN_LED_2);

#include "part/display-eye.h"
DisplayEye displayEye(gfx);

#include "part/display-mini.h"
DisplayMini displayMini(&u8g2);

#include "services/hardware.h"
Hardware hardware;

// Routines ---------------------------------------------------
#include "routines/routines.h"
#define ROUTINE_COUNT 3
Routine* routines[ROUTINE_COUNT];
RoutineController routineController(routines, ROUTINE_COUNT);

#include "routines/tests.h"
TestRoutines testRoutine(
  &lensLed1,
  &lensLed2,
  &lensLed3,
  &batteryGlow,
  &eyeGlow,
  &stripLeft,
  &stripRight,
  &stripLeftLow,
  &stripRightLow,
  &displayMini,
  &displayEye
);

#include "routines/routine-two.h"
RoutineTwo routineTwo(&displayMini);

#include "routines/wom-routine.h"
WomRoutine womRoutine(&displayMini, &stripLeft, &stripRight, &stripLeftLow, &stripRightLow, &lensLed1, &lensLed2, &lensLed3, &batteryGlow, &eyeGlow);

// Webserver --------------------------------------------------
#include "ESPAsyncWebServer.h"
AsyncWebServer webServer(80);

// HTTP APIs
#include "web/eyeball-api.h"
EyeballApi eyeballApi;
#include "web/head-sides-api.h"
HeadSidesApi headSidesApi;
#include "web/lens-lights-api.h"
LensLightsApi lensLightsApi;
#include "web/mini-display-api.h"
MiniDisplayApi miniDisplayApi(&displayMini);
#include "web/misc-api.h"
MiscApi miscApi;
#include "web/routines-api.h"
RoutinesApi routinesApi(&routineController);
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

  // Populate hardware
  hardware.batteryGlow = &batteryGlow;
  hardware.buttonLeft = &buttonLeft;
  hardware.buttonRight = &buttonRight;
  hardware.displayMini = &displayMini;
  hardware.eyeGlow = &eyeGlow;
  hardware.ledStrip = &ledStrip;
  hardware.lensLed1 = &lensLed1;
  hardware.lensLed2 = &lensLed2;
  hardware.lensLed3 = &lensLed3;
  hardware.stripLeft = &stripLeft;
  hardware.stripRight = &stripRight;
  hardware.stripLipLeft = &stripLeftLow;
  hardware.stripLipRight = &stripRightLow;

  // FastLED.setMaxPowerInVoltsAndMilliamps(LED_VOLTS, LED_MAX_MA);
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
  ledStrip.begin();
  lensLed1.begin();
  lensLed2.begin();
  lensLed3.begin();
  displayEye.begin();
  displayMini.begin();
  stripLeft.begin();
  stripRight.begin();

  buttonLeft.begin();
  buttonLeft.setTouchCallback([]() {
    routineController.next();
  });

  buttonRight.begin();
  buttonRight.setTouchCallback([]() {
    Serial.println("Touch B");
  });
  buttonRight.setHoldCallback([]() {
    Serial.println("Touch B: HOLD");
  }, 1000);

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
  routines[2] = &womRoutine;

  routineController.begin();
  routineController.setRoutine(&testRoutine);

  // Wifi + DNS
  wifiService.begin();

  // Captive Portal
  webServer.addHandler(&captiveRequestHandler).setFilter(ON_AP_FILTER);//only when requested from AP

  // Attach HTTP Handlers
  eyeballApi.attach(&webServer);
  headSidesApi.attach(&webServer);
  lensLightsApi.attach(&webServer);
  miniDisplayApi.attach(&webServer);
  miscApi.attach(&webServer);
  routinesApi.attach(&webServer);
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

void loop() {
  tickDebugLed();
  dnsServer.processNextRequest();
  routineController.tick();
 
  // Component ticks
  hardware.tick();
}
