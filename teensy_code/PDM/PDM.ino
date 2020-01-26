// Dave Yonkers, 2020

#include <EasyTimer.h>
#include <PWMControl.h>
#include <ReadADC.h>
#include <StateCAN.h>
#include <FlexCAN_T4.h>
#include <SPI.h>

/* TODOS:
 *  -
*/

// global variable definition
int GLO_engine_state = 0; // engine state (no need to change this variable)
const int GLO_read_resolution_bits = 10; // bits for Teensy-based read resolution

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

// support class for diode-based board temp
#include "board_temp.hpp"

// odds and ends functions
#include "misc_fcns.hpp"

// on-board diagnostics
#include "obd.hpp"

// timer that you can use to print things out for debugging
EasyTimer debug(3);


void setup() {

  // Initialize serial communication
  Serial.begin(112500);

  

  //initialize the CAN Bus and set its baud rate to 1Mb
  cbus1.begin();
  cbus1.setBaudRate(1000000);
  cbus2.begin();
  cbus2.setBaudRate(1000000);

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

  //set EEPROM pin cs pin high
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);

}

void loop() {
  sample_ADCs();

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
  obd();

  // send all of the things
  send_can1();
  send_can2();

  if (debug.isup()){

    Serial.print("PDM Volt: ");
    Serial.println(pdm_volt_sens.avg());

    Serial.print("Left Fan Volt: ");
    Serial.println(fanl_volt_sens.avg());
    
    Serial.print("Right Fan Volt: ");
    Serial.println(fanr_volt_sens.avg());
    
    Serial.print("Water Pump Volt: ");
    Serial.println(wp_volt_sens.avg());
    
    Serial.print("Main Volt: ");
    Serial.println(main_volt_sens.avg());
    
    Serial.print("Data Volt: ");
    Serial.println(data_volt_sens.avg());
    
    Serial.print("Fuel Volt: ");
    Serial.println(fuel_volt_sens.avg());
    
    Serial.print("Keepalive Volt: ");
    Serial.println(keepalive_volt_sens.avg());
    
    Serial.print("PDM Current: ");
    Serial.println(pdm_current_sens.avg());
    
    Serial.print("Left Fan Current: ");
    Serial.println(fanl_current_sens.avg());
    
    Serial.print("Right Fan Current: ");
    Serial.println(fanr_current_sens.avg());
    
    Serial.print("WP Current: ");
    Serial.println(wp_current_sens.avg());

    Serial.print("Fuel Current: ");
    Serial.println(fuel_current_sens.avg());

    Serial.println("------------------------------");
 
  }
}
