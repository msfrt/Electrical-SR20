#include <Adafruit_NeoPixel.h>
#include <StateCAN.h>
#include <FlexCAN_T4.h>
#include <EasyTimer.h>

#include "rainbow_pixels.hpp"


// NeoPixel parameters & initialization
const int pixel_pin = 9;  // teensy pin #
const int pixel_cnt = 1;  // number of LEDs
      int pixel_brightness_percent = 5;  // 0 - 100%

Adafruit_NeoPixel pixel = Adafruit_NeoPixel(pixel_cnt, pixel_pin, NEO_GRB + NEO_KHZ800);



void setup() {

  // begin NeoPixel
  pixel.setBrightness(map(pixel_brightness_percent, 0, 100, 0, 255));
  pixel.begin();
  pixel.show();

}



void loop() {
  // put your main code here, to run repeatedly:

  rainbow_pixels(pixel);
}
