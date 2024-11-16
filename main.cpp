#if ARDUINO_USB_MODE
#warning This sketch should be used when USB is in OTG mode
void setup()
{
}
void loop()
{
}
#else

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

void setup()
{
    delay(WAIT_HUGE);
    server.on("/", HTTP_POST, []() {
        leds[0] = CRGB::Blue;
        FastLED.show();
        Serial.begin(BAUD_RATE);
        Serial.println("got message");
        Serial.end();
        server.send(200);
        Keyboard.begin();
        Keyboard.press(KEY_F24);
        Keyboard.release(KEY_F24);
        Keyboard.end();
        delay(WAIT_SMALL);
        leds[0] = CRGB::Black;
        FastLED.show();
    });
    USB.begin();
    FastLED.clear();
    FastLED.addLeds<APA102, LED_DI_PIN, LED_CI_PIN, BGR>(leds, NUM_LEDS);
    Serial.begin(BAUD_RATE);
    Serial.print("connecting to ");
    Serial.println(WIFI_SSID);
    Serial.end();
    WiFi.disconnect(true);
    delay(WAIT_NORMAL);
    WiFi.onEvent([](WiFiEvent_t event) {
        switch (event)
        {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            leds[0] = CRGB::Green;
            FastLED.show();
            Serial.begin(BAUD_RATE);
            Serial.print("connected at ");
            Serial.println(WiFi.localIP());
            Serial.end();
            server.begin();
            if (MDNS.begin(DOMAIN_NAME))
            {
                Serial.begin(BAUD_RATE);
                Serial.println("set up mDNS");
                Serial.end();
            }
            else
            {
                Serial.begin(BAUD_RATE);
                Serial.println("couldn't set up mDNS");
                Serial.end();
            }
            MDNS.addService("http", "tcp", HTTP_PORT);
            delay(WAIT_SMALL);
            leds[0] = CRGB::Black;
            FastLED.show();
            delay(WAIT_NORMAL);
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            MDNS.end();
            leds[0] = CRGB::Red;
            FastLED.show();
            Serial.begin(BAUD_RATE);
            Serial.print("disconnected, reconnecting to ");
            Serial.println(WIFI_SSID);
            Serial.end();
            WiFi.disconnect(true);
            delay(WAIT_NORMAL);
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
            delay(WAIT_SMALL);
            leds[0] = CRGB::Black;
            FastLED.show();
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