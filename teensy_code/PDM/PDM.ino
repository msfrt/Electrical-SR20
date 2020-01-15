// Dave Yonkers, 2020

#include <EasyTimer.h>
#include <PWMControl.h>
#include <ReadADC.h>
#include <StateCAN.h>
#include <FlexCAN_T4.h>

// this file contains all of the control tables for fans and water pump
#include "Tables.hpp"

// left fan definition & partial initialization
int fanl_pin = 1; // update!
int fanl_min_pwm = 0;
int fanl_max_pwm = 255;
int fanl_ss_dur = 5000; // duration of soft start in millis
int fanl_update_freq = 10;
int fanl_pwm_freq_norm = 40; // normal pwm frequency in Hz
int fanl_pwm_freq_ss = 420; // soft start pwm frequency in Hz
PWMDevice fan_left(fanl_pin, fanl_table_rows, fanl_table_cols, fanl_min_pwm, fanl_max_pwm,
                   fanl_ss_dur, fanl_update_freq, fanl_pwm_freq_norm, fanl_pwm_freq_ss);

// right fan definition & partial initialization
int fanr_pin = 1; // update!
int fanr_min_pwm = 0;
int fanr_max_pwm = 255;
int fanr_ss_dur = 5000; // duration of soft start in millis
int fanr_update_freq = 10;
int fanr_pwm_freq_norm = 40; // normal pwm frequency in Hz
int fanr_pwm_freq_ss = 420; // soft start pwm frequency in Hz
PWMDevice fan_right(fanr_pin, fanr_table_rows, fanr_table_cols, fanr_min_pwm, fanr_max_pwm,
                   fanr_ss_dur, fanr_update_freq, fanr_pwm_freq_norm, fanr_pwm_freq_ss);

// water pump definition & partial initialization
int wp_pin = 1; // update!
int wp_min_pwm = 0;
int wp_max_pwm = 255;
int wp_ss_dur = 5000; // duration of soft start in millis
int wp_update_freq = 10;
int wp_pwm_freq_norm = 40; // normal pwm frequency in Hz
int wp_pwm_freq_ss = 420; // soft start pwm frequency in Hz
PWMDevice water_pump(wp_pin, wp_table_rows, wp_table_cols, wp_min_pwm, wp_max_pwm,
                     wp_ss_dur, wp_update_freq, wp_pwm_freq_norm, wp_pwm_freq_ss);

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
