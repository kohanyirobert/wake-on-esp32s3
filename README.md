# About

ESP32S3 module that acts as a webserver and keyboard.
Whenever someone sends an HTTP request to the server it'll emulate an F24 keypress.
Used to wake up a laptop which doesn't respond to Wake-on-LAN and out of reach by other wireless means (e.g. like a 2.4 GHz wireless mouse or similar).

- Written for [LILYGO T-Dongle-S3](https://www.lilygo.cc/products/t-dongle-s3)
- Use PlatformIO plugin VS Code to build
- Copy `wifi_setup.h.sample` to `wifi_setup.h` and configure SSID and password in the header file before building
- To upload plug the device into a USB port *while holding the button on it*
- To run the code replug it without holding the button
- To run `pio` executable in VS Code run _Open PlatformIO Core CLI_ from the command palette
- To build and upload project run `pio run --target upload`
- Then run `pio run --target monitor` to see the output on serial

## Android

Discovering the board on Windows and macOS works almost always and `curl wake-on-esp32s3.local` goes through.
Android is a mixed bag. Here are some ways to try and debug what can make the address not to resolve.

I've tested this on a Samsung A52s 5G with Android 12 (latest updates as far as I know).

- From a specific Android version and upwards `.local` addresses should be resolved properly (in theory)
- Private DNS settings can have an effect whether the address resolves or not
- However on my LAN the address wouldn't resolve no matter what my private DNS setting was (off, automatic, or a specific resolver, `dnsforge.de` in my case)
- Yet again, on another LAN the address only resolved when I disabled private DNS
- Checking what kind of DNS server is used to resolve things is kind of difficult, but here is a method
    - Connect to the phone with `adb`
    - Run `dumpsys connectivity | grep -i dns`
    - Give the output to some AI chat model to format and analyze and see what's in there
    - Re-run it after switching private DNS settings
- Using something like Termux to see DNS settings might prove useless, since they might use custom DNS resolver settings
- Although, in theory it should not matter since `.local` resolving should bypass DNS servers (to my understanding)
- Finally, check your browser whether it can resolve `http://wake-on-esp32s3.local`

## TODO

- Implement simple logging onto the TFT (need to handle overflowing text with println, e.g. calculate how many lines with in certain orientation and keep a few line buffers)
- Handle different REST requests to do certain stuff (enabled/disable TFT using POST and DELETE, etc.)