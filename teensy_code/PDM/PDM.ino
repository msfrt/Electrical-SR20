// Dave Yonkers, 2020

#include <EasyTimer.h>
#include <PWMControl.h>
#include <ReadADC.h>
#include <StateCAN.h>
#include <FlexCAN_T4.h>
#include <SPI.h>

// useful sensor sampling definitions can be found here
#include "sensors.hpp"

// this file contains all of the control tables & declarations for fans and water pump
#include "fans.hpp"

// signal definitions are inside
#include "sigs_inside.hpp"

// CAN message definitions are inside
#include "can_msgs.hpp"

// global variable definition
int GLO_engine_state = 0;

void setup() {

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

}

void loop() {
  // put your main code here, to run repeatedly:
  sample_ADCs();

  fan_left.set_pwm(GLO_engine_state);
  fan_right.set_pwm(GLO_engine_state);
  water_pump.set_pwm(GLO_engine_state);
}
