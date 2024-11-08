#if ARDUINO_USB_MODE
#warning This sketch should be used when USB is in OTG mode
void setup() {}
void loop() {}
#else

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <FastLED.h>
#include <USB.h>
#include <USBHIDKeyboard.h>

#include "pin_config.h"
#include "wifi_setup.h"

#define NUM_LEDS 1

CRGB leds[NUM_LEDS];
WebServer server(80);
USBHIDKeyboard Keyboard;

void setup()
{
  delay(5000);
  Serial.begin(9600);
  USB.begin();
  FastLED.clear();
  FastLED.addLeds<APA102, LED_DI_PIN, LED_CI_PIN, BGR>(leds, NUM_LEDS);
  Serial.print("connecting to ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("connected at ");
  Serial.println(WiFi.localIP());
  Serial.end();
  server.on("/", []() {
    server.send(200);
    leds[0] = CRGB::Green;
    FastLED.show();
    Keyboard.begin();
    Keyboard.press(KEY_F24);
    Keyboard.release(KEY_F24);
    Keyboard.end();
    delay(500);
    leds[0] = CRGB::Black;
    FastLED.show();
  });
  server.begin();
}

void loop()
{
  server.handleClient();
  delay(100);
}
#endif