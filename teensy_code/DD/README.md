# Driver Display
#### Dave Yonkers, 2020

## Dependancies

There are several dependancies for the driver display code. External libraries for the Neopixels and the Adafruit GFX should
be included in the Teensyduino install, but you will need to install the optimized ILI9341 library. Here are the links:
* [ILI9341_t3n](https://github.com/KurtE/ILI9341_t3n)
* [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
* [Adafruit Neopixel](https://github.com/adafruit/Adafruit_NeoPixel)
* [FlexCAN T4](https://github.com/tonton81/FlexCAN_T4)

You will also need to install pretty much every library in Dave's 
[SR-Libraries](https://github.com/msfrt/SR-Libraries) repository.

## Creating a new InfoScreen

The info screen is the OG driver display screen. Each screen can display four StateSignals.
