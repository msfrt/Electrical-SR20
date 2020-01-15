// Dave Yonkers, 2020

#include <EasyTimer.h>
#include <PWMControl.h>
#include <ReadADC.h>
#include <StateCAN.h>
#include <FlexCAN_T4.h>

// this file contains all of the control tables for fans and water pump
#include "Tables.hpp"

int fanl_pin = 1; // update!
int fanl_min_pwm = 0;
int fanl_max_pwm = 255;
int fanl_ss_dur = 5000; // duration of soft start in millis
int fanl_update_freq = 10;
int fanl_pwm_freq_norm = 40; // normal pwm frequency in Hz
int fanl_pwm_freq_ss = 420; // soft start pwm frequency in Hz
PWMDevice fan_left(fanl_pin, fanl_table_rows, fanl_table_cols, fanl_min_pwm, fanl_max_pwm,
                   fanl_ss_dur, fanl_update_freq, fanl_pwm_freq_norm, fanl_pwm_freq_ss);

void setup() {
  
  int *fanl_table_ptr = fan_left_table[0]; // create a temp ptr to populate PWM device
  fan_left.fill_table(fanl_table_ptr); // populate the PWMDevice table

}

void loop() {
  // put your main code here, to run repeatedly:

}
