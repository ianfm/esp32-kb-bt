# esp32-kb-bt

The goal is to build a PS/2 to bluetooth HID converter with a small display for funsies.

I have several very different branches because I have been trying different ways of accessing and compiling useful libraries, e.g. for Bluetooth, SSD1306 display, and graphics

Main: 
Using esp-idf and vscode (separate, not the plugin) to develop SSD1306 driver using esp32 HAL for e.g. i2c.

Adafruit-port: 
Copied Adafruit_SSD1306 and Adafruit_GFX libs from arduino folders and tried to port them to use the ESP32 HAL.

esp-idf-arduino:
For this one I'm trying to install arduino as a component in an ESP-IDF project, per https://github.com/espressif/arduino-esp32/blob/master/docs/esp-idf_component.md and running into some lightly documented errors. It's possible I could get this to work by cloning very specific commits of ESP-IDF and arduino-esp32.
