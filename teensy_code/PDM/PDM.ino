// Dave Yonkers, 2020

#include <EasyTimer.h>
#include <PWMControl.h>
#include <ReadADC.h>
#include <StateCAN.h>
#include <FlexCAN_T4.h>

// this file contains all of the control tables for fans and water pump
#include "Tables.hpp"

void setup() {

  // left fan table population
  int *fanl_table_ptr = fan_left_table[0]; // create a temp ptr to populate PWM device
  fan_left.fill_table(fanl_table_ptr); // populate the PWMDevice table

  // right fan table population
  int *fanr_table_ptr = fan_right_table[0];
  fan_right.fill_table(fanr_table_ptr);

  // water pump table population
  int *wp_table_ptr = wp_table[0];
  water_pump.fill_table(wp_table_ptr);

}

void loop() {
  // put your main code here, to run repeatedly:

}
