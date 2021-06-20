 #include <EasyTimer.h>
#include <PWMControl.h>
#include <ReadADC.h>
#include <StateCAN.h>
#include <FlexCAN_T4.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>

// ATCC Module Select - 0 front, 1 back
const int ATCCMS = 1;

// sensor definitions
#include "sensors.hpp"

// signal definitions
#include "sigs_inside.hpp"

// can message definitions
#include "can_send.hpp"

// brake bias calculation for front ATCC
#include "bias_calc.hpp"

// rainbow RGB
#include "rainbow_pixels.hpp"

const int GLO_NeoPixel_teensy_pin = 0;
      int GLO_NeoPixel_brightness_percent = 100; // 0 - 100 %
Adafruit_NeoPixel GLO_neopixel(1, GLO_NeoPixel_teensy_pin, NEO_GRB + NEO_KHZ800);


void setup() {

  // begin Neopixel
  GLO_neopixel.begin();
  GLO_neopixel.setBrightness(map(GLO_NeoPixel_brightness_percent, 0, 100, 0, 255));
  GLO_neopixel.setPixelColor(0, 255, 0, 0); // red
  GLO_neopixel.show();

  // Initialize serial communication
  Serial.begin(112500);

  // initialize SPI communication
  SPI.begin();

  //initialize the CAN Bus and set its baud rate to 1Mb
  cbus2.begin();
  cbus2.setBaudRate(1000000);

  //initialize ADCs
  initialize_ADCs();

  // turn neopixel green
  GLO_neopixel.setPixelColor(0, 0, 255, 0); // green
  GLO_neopixel.show();


}


void loop() {

  msu_pixels(GLO_neopixel);

  switch (ATCCMS)
  {
    case 0:
      sample_ADCs_F();
      send_can_F();
      calculate_brake_bias(ATCCF_brakePressureF, ATCCF_brakePressureR, ATCCF_brakeBias);
      break;
    case 1:
      sample_ADCs_R();
      send_can_R();
      break;
  }



}
