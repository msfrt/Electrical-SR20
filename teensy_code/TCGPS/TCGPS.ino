#include <Adafruit_NeoPixel.h>
#include <StateCAN.h>
#include <FlexCAN_T4.h>
#include <EasyTimer.h>
#include <BoardTemp.h>

// can bus decleration
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> cbus1;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> cbus2;

#include "sigs_inside.hpp"

#include "rainbow_pixels.hpp"

#include "can_send.hpp"

// board temp setup
#define READ_RESOLUTION_BITS 12
const int board_temp_pin = 19;
BoardTempDiode board_temp(board_temp_pin, READ_RESOLUTION_BITS, 26.2, 580);
EasyTimer board_temp_sample_timer(50);


// NeoPixel parameters & initialization
const int pixel_pin = 9;  // teensy pin #
const int pixel_cnt = 1;  // number of LEDs
const int pixel_brightness_percent = 5;  // 0 - 100%
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(pixel_cnt, pixel_pin, NEO_GRB + NEO_KHZ800);


EasyTimer debug_timer(10);


void setup() {

  // initilize CAN busses
  cbus1.begin();
  cbus1.setBaudRate(1000000);
  cbus2.begin();
  cbus2.setBaudRate(1000000);

  analogReadResolution(READ_RESOLUTION_BITS);
  board_temp.begin();

  Serial.begin(115200);

  // begin NeoPixel
  pixel.setBrightness(map(pixel_brightness_percent, 0, 100, 0, 255));
  pixel.begin();
  pixel.show();

}



void loop() {
  // put your main code here, to run repeatedly:

  if (board_temp_sample_timer.isup())
    board_temp.sample();


  if (debug_timer.isup()){
  }

  // turn the neopixels rainbow colors :-)
  rainbow_pixels(pixel);

  // send can messages
  send_can1();
  send_can2();
}
