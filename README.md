# esp32-kb-bt

The goal is to fix a nice monoprice keyboard with a broken PS/2 cable by fconnecting it to a microcontroller and sending appropriate HID packets to a Bluetooth client.

I am also adding a display for the experience.



## Branches
I have several very different branches because I have been trying different ways of accessing and compiling useful libraries, e.g. for keyboard, Bluetooth, SSD1306 display, and graphics. Their purposes are explained below, as well as the build tool used for each.

### Master: 
Developing SSD1306 driver using esp32 HAL for i2c.  
**build tool**:  esp-idf.py terminal  
**hardware**:  SSD1306  
**libraries**:  N/A (ESP32 I guess)  



### Adafruit-port: 
Copied Adafruit_SSD1306 and Adafruit_GFX libs from arduino folders into project folder and tried to port them to use the ESP32 HAL instead of arduino core functions.  
**build tool**: esp-idf.py terminal  
**hardware**: SSD1306  
**libraries**: Adafruit_SSD1306, Adafruit_GFX  


### esp-idf-arduino:  
Trying to install arduino as a component in an ESP-IDF project, per https://github.com/espressif/arduino-esp32/blob/master/docs/esp-idf_component.md and running into some lightly documented errors. It's possible I could get this to work by cloning very specific commits of ESP-IDF and arduino-esp32, according to some issue ticket.  
**build tool**: esp-idf.py terminal  
**hardware**: SSD1306  
**libraries**: Adafruit_SSD1306, Adafruit_GFX  


### Arduino-only
Using arduino plugin for VS Code (or arduino IDE) to interface with the keyboard   
**build tool**: Arduino IDE  
**hardware**: Keyboard  
**libraries**: PS2Keyboard by ? from arduino libmgr  
