// Dave Yonkers, 2020

#include <EasyTimer.h>
#include <PWMControl.h>
#include <ReadADC.h>
#include <StateCAN.h>
#include <FlexCAN_T4.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <BoardTemp.h>

/* TODOS:
 *  -
*/


// global variable definition
int GLO_engine_state = 0; // engine state (no need to change this variable)
const int GLO_read_resolution_bits = 12; // bits for Teensy-based read resolution

// minimum voltage for the engine to be in "cranking" mode
const int GLO_cranking_starter_volt_threshold = 5;
// max rpm for the engine to still be in "cranking" mode
const int GLO_cranking_rpm_threshold = 2000;
// engine running min rpm threshold
const int GLO_engine_on_rpm_threshold = 900;
// engine cooldown state duration in milliseconds
const int GLO_engine_cooldown_duration = 5000;

const int GLO_brakelight_min_pressure_F = 60; // minimum pressure required to activate the brakelight (PSI)
const int GLO_brakelight_min_pressure_R = 60;
const int GLO_brakelight_teensy_pin = 4;

const int GLO_data_circuit_teensy_pin = 5;

const int GLO_NeoPixel_teensy_pin = 2;
      int GLO_NeoPixel_brightness_percent = 10; // 0 - 100 %

Adafruit_NeoPixel GLO_obd_neopixel(1, GLO_NeoPixel_teensy_pin, NEO_GRB + NEO_KHZ800);

//BoardTemp(int pin, int read_bits, int temp_cal, int mv_cal);
BoardTempDiode board_temp(21, GLO_read_resolution_bits, 28.1, 594);
EasyTimer board_temp_sample_timer(50);

// useful sensor sampling definitions can be found here
#include "sensors.hpp"

// this file contains all of the control tables & declarations for fans and water pump
#include "fans.hpp"

// signal definitions are inside
#include "sigs_inside.hpp"

// CAN message definitions are inside
#include "can_send.hpp"

// CAN read functions are inside
#include "can_read.hpp"

// odds and ends functions
#include "misc_fcns.hpp"

// on-board diagnostics
#include "obd.hpp"

// timer that you can use to print things out for debugging
EasyTimer debug(3);


void setup() { //high 18 low 26

  analogReadResolution(GLO_read_resolution_bits);

  // begin OBD Neopixel
  GLO_obd_neopixel.begin();
  GLO_obd_neopixel.setBrightness(map(GLO_NeoPixel_brightness_percent, 0, 100, 0, 255));
  GLO_obd_neopixel.setPixelColor(0, 255, 0, 0); // red
  GLO_obd_neopixel.show();

  // Initialize serial communication
  Serial.begin(112500);

  //initialize the CAN Bus and set its baud rate to 1Mb
  cbus1.begin();
  cbus1.setBaudRate(1000000);
  cbus2.begin();
  cbus2.setBaudRate(1000000);
  cbus2.setMB(MB17,TX);

  // populate left fan table
  int *fanl_table_ptr = fan_left_table[0]; // create a temp ptr to populate PWM device
  fan_left.fill_table(fanl_table_ptr); // populate the PWMDevice table

  // populate right fan table
  int *fanr_table_ptr = fan_right_table[0];
  fan_right.fill_table(fanr_table_ptr);

  // populate water pump table
  int *wp_table_ptr = wp_table[0];
  water_pump.fill_table(wp_table_ptr);

  // initialize the ADC sensors
  initialize_ADCs();

  // initialize SPI communication
  SPI.begin();

  // initialize the data circuit pin
  pinMode(GLO_data_circuit_teensy_pin, OUTPUT);
  // turn the data circuit on
  digitalWrite(GLO_data_circuit_teensy_pin, HIGH);

  // EEPROM PINS
  pinMode(9, INPUT);
  digitalWrite(9, HIGH);

  GLO_obd_neopixel.setPixelColor(0, 0, 255, 0); // green
  GLO_obd_neopixel.show();

  // board temp initialization
  board_temp.begin();

}

void loop() {

  // sensor sampling
  sample_ADCs();
  if (board_temp_sample_timer.isup()){
    board_temp.sample();
  }

  // read both can buses
  read_can1();
  read_can2();

  // run the brakelight
  brakelight();

  // determine engine state and control PWM outputs
  determine_engine_state(GLO_engine_state);
  fan_left.set_pwm(GLO_engine_state);
  fan_right.set_pwm(GLO_engine_state);
  water_pump.set_pwm(GLO_engine_state);

  // continously run OBD (individual timers are included)
  obd_main();

  // send all of the things
  send_can1();
  send_can2();

  if (debug.isup()){

    cbus2.mailboxStatus();
  }
}
