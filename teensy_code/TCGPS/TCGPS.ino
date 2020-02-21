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

#include "lap_trigger.hpp"

// board temp setup
#define READ_RESOLUTION_BITS 12
const int board_temp_pin = 19;
BoardTempDiode board_temp(board_temp_pin, READ_RESOLUTION_BITS, 26.2, 580);
EasyTimer board_temp_sample_timer(50);


// NeoPixel parameters & initialization
const int pixel_pin = 9;  // teensy pin #
const int pixel_cnt = 1;  // number of LEDs
const int pixel_brightness_percent = 100;  // 0 - 100%
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(pixel_cnt, pixel_pin, NEO_GRB + NEO_KHZ800);

// EEPROM CS pin
const int eeprom_cs_pin = 10;


// GPS messages
// GPS communications datasheet: https://cdn-shop.adafruit.com/datasheets/PMTK_A11.pdf
char set_gps_fast_serial_cmd[] = {0x24, 0x50, 0x4D, 0x54, 0x4B, 0x32, 0x35, 0x31, 0x2C, 0x31, 0x31, 0x35, 0x32, 0x30, 0x30, 0x2A, 0x31, 0x46, 0x0D, 0x0A};
char set_gps_fast_update_cmd[] = {0x24, 0x50, 0x4D, 0x54, 0x4B, 0x32, 0x32, 0x30, 0x2C, 0x31, 0x30, 0x30, 0x2A, 0x32, 0x46, 0x0D, 0x0A};


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

  // EEPROM disabled
  pinMode(eeprom_cs_pin, OUTPUT);
  digitalWrite(eeprom_cs_pin, HIGH);

  // begin NeoPixel
  pixel.setBrightness(map(pixel_brightness_percent, 0, 100, 0, 255));
  pixel.begin();
  pixel.show();


  // GPS initialization
  pixel.setPixelColor(0, 255, 0, 0); pixel.show(); // pixel red

  Serial2.begin(9600);
  delay(2000);

  pixel.setPixelColor(0, 255, 255, 0); pixel.show(); // pixel yeller
  Serial2.write(set_gps_fast_serial_cmd);
  delay(500);
  Serial1.end();

  delay(1000);
  Serial2.begin(115200);
  delay(500);
  Serial2.write(set_gps_fast_update_cmd);

  pixel.setPixelColor(0, 0, 255, 0); pixel.show(); // pixel green

  delay(500);
  Serial2.end();
  // END - GPS initialization

}



void loop() {
  // put your main code here, to run repeatedly:

  if (board_temp_sample_timer.isup())
    board_temp.sample();


  if (debug_timer.isup()){
  }

  // turn the neopixels rainbow colors :-)
  if (!laptrigger_sucess_pixel(pixel)){
    rainbow_pixels(pixel);
  }

  // send can messages
  send_can1();
  send_can2();


}
