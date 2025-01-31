# About

ESP32S3 module that acts as a webserver and keyboard.
Whenever someone sends an HTTP request to the server it'll emulate an F24 keypress.
Used to wake up a laptop which doesn't respond to Wake-on-LAN and out of reach by other wireless means (e.g. like a 2.4 GHz wireless mouse or similar).

- Written for [LILYGO T-Dongle-S3](https://www.lilygo.cc/products/t-dongle-s3)
- Use PlatformIO plugin VS Code to build
- Copy `wifi_setup.h.sample` to `wifi_setup.h` and configure SSID and password in the header file before building
- To upload plug the device into a USB port *while holding the button on it*
- To run the code replug it without holding the button

## TODO

- Implement simple logging onto the TFT (need to handle overflowing text with println, e.g. calculate how many lines with in certain orientation and keep a few line buffers)
- Handle different REST requests to do certain stuff (enabled/disable TFT using POST and DELETE, etc.)