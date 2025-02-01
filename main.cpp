#if ARDUINO_USB_MODE
#warning This sketch should be used when USB is in OTG mode
void setup()
{
}
void loop()
{
}
#else

#include <functional>

#include <Arduino.h>
#include <ESPmDNS.h>
#include <FastLED.h>
#include <USB.h>
#include <USBHIDKeyboard.h>
#include <WebServer.h>
#include <WiFi.h>

#include "pin_config.h"
#include "wifi_setup.h"

#define WAIT_HUGE 5000
#define WAIT_NORMAL 1000
#define WAIT_SMALL 500
#define WAIT_TINY 100
#define NUM_LEDS 1
#define BAUD_RATE 9600
#define HTTP_PORT 80
#define DOMAIN_NAME "wake-on-esp32s3"

CRGB leds[NUM_LEDS];
USBHIDKeyboard Keyboard;
WebServer server(HTTP_PORT);

void withLed(CRGB::HTMLColorCode color, std::function<void()> fn)
{
    leds[0] = color;
    FastLED.show();
    fn();
    delay(WAIT_SMALL);
    leds[0] = CRGB::Black;
    FastLED.show();
}

void withSerial(std::function<void()> fn)
{
    Serial.begin(BAUD_RATE);
    fn();
    Serial.end();
}

template <typename T> void xPrint(const T &s)
{
    Serial.print(s);
}

template <typename T> void xPrintln(const T &s)
{
    Serial.println(s);
}

void setup()
{
    delay(WAIT_HUGE);
    withSerial([]() {
        xPrint("starting at ");
        xPrintln(WiFi.macAddress());
    });
    server.on("/", HTTP_GET, []() {
        withLed(CRGB::Blue, []() {
            withSerial([]() {
                xPrint("wake up from "); 
                xPrintln(server.client().remoteIP());
            });
            Keyboard.begin();
            Keyboard.press(KEY_F24);
            Keyboard.release(KEY_F24);
            Keyboard.end();
        });
        server.send(200);
    });
    USB.begin();
    FastLED.clear();
    FastLED.addLeds<APA102, LED_DI_PIN, LED_CI_PIN, BGR>(leds, NUM_LEDS);
    withSerial([]() {
        xPrint("connecting to ");
        xPrintln(WIFI_SSID);
    });
    WiFi.disconnect(true);
    delay(WAIT_NORMAL);
    WiFi.onEvent([](WiFiEvent_t event) {
        switch (event)
        {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            withLed(CRGB::Green, []() {
                withSerial([]() {
                    xPrint("connected at ");
                    xPrintln(WiFi.localIP());
                    server.begin();
                    if (MDNS.begin(DOMAIN_NAME))
                    {
                        xPrintln("set up mDNS");
                    }
                    else
                    {
                        xPrintln("couldn't set up mDNS");
                    }
                });
                MDNS.addService("http", "tcp", HTTP_PORT);
            });
            delay(WAIT_NORMAL);
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            MDNS.end();
            withLed(CRGB::Red, []() {
                withSerial([]() {
                    xPrint("disconnected, reconnecting to ");
                    xPrintln(WIFI_SSID);
                });
                WiFi.disconnect(true);
                delay(WAIT_NORMAL);
                WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
            });
            delay(WAIT_NORMAL);
            break;
        default:
            break;
        }
    });
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop()
{
    server.handleClient();
    delay(WAIT_TINY);
}
#endif