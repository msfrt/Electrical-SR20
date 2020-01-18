// Dave Yonkers, 2020

#ifndef TABLES_HPP
#define TABLES_HPP

#include <PWMControl.h>
#include <StateCAN.h>

// signal definitions
#include "sigs_inside.hpp"

// rows: temp in degrees C
// cols: battery voltage V
const int fanl_table_rows = 12;
const int fanl_table_cols = 14;
const int fanl_table_row_scalar = 10;
const int fanl_table_col_scalar = 10000;
int fan_left_table[fanl_table_rows][fanl_table_cols] =
{
  {    0, 80000, 90000, 100000, 105000, 110000, 119000, 120000, 130000, 137000, 138000, 139000, 142000, 145000},
  {    0,     0,     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
  {  700,     0,     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
  {  850,     0,     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     25},
  {  855,    15,    15,     15,     15,     15,     15,     15,     15,     15,     30,     30,     30,     30},
  {  920,    15,    15,     15,     15,     15,     15,     15,     15,     15,     50,     50,     50,     50},
  {  921,    15,    15,     15,     15,     25,     25,     25,     25,     25,     65,     65,     65,     65},
  {  950,    75,    75,     75,     75,     75,     75,     75,     75,     75,     75,     75,     75,     75},
  {  951,   100,   100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100},
  { 1000,   100,   100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100},
  { 1001,   100,   100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100},
  { 1500,   100,   100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100},
};

// rows: temp in degrees celcius * 10
// cols: battery voltage in mV * 10
const int fanr_table_rows = 12;
const int fanr_table_cols = 14;
const int fanr_table_row_scalar = 10;
const int fanr_table_col_scalar = 10000;
int fan_right_table[fanr_table_rows][fanr_table_cols] =
{
  {    0, 80000, 90000, 100000, 105000, 110000, 119000, 120000, 130000, 137000, 138000, 139000, 142000, 145000},
  {    0,     0,     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
  {  700,     0,     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
  {  850,     0,     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     25},
  {  851,    15,    15,     15,     15,     15,     15,     15,     15,     15,     30,     30,     30,     30},
  {  920,    15,    15,     15,     15,     15,     15,     15,     15,     15,     50,     50,     50,     50},
  {  921,    15,    15,     15,     15,     25,     25,     25,     25,     25,     65,     65,     65,     65},
  {  950,    75,    75,     75,     75,     75,     75,     75,     75,     75,     75,     75,     75,     75},
  {  951,   100,   100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100},
  { 1000,   100,   100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100},
  { 1001,   100,   100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100},
  { 1500,   100,   100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    100}
};

// rows: temp in degrees celcius
// cols: RPM
const int wp_table_rows = 12;
const int wp_table_cols = 8;
const int wp_table_row_scalar = 10;
const int wp_table_col_scalar = 1;
int wp_table[wp_table_rows][wp_table_cols] =
{
  {   00,   0,  10,  20, 500, 510,  5000, 15000},
  {  100,   0,   0,   0,   0,  20,    20,    20},
  {  200,   0,   0,   0,   0,  20,    20,    20},
  {  400,   0,   0,   0,   0,  35,    35,    35},
  {  500,   0,   0,   0,   0,  35,    35,    35},
  {  600,   0,   0,   0,   0,  35,    35,    35},
  {  699,   0,   0,   0,   0,  35,    35,    35},
  {  700,   0,   0,   0,   0,  40,    40,    40},
  {  845,   0,   0,   0,   0,  50,    50,    50},
  {  846,  25,  25,   0,   0,  60,    60,    60},
  { 1000, 100, 100,   0,   0, 100,   100,   100},
  { 1500, 100, 100,   0,   0, 100,   100,   100},
};




// left fan definition & partial initialization
int fanl_pin = 8;
int fanl_min_pwm = 0;
int fanl_max_pwm = 255;
int fanl_ss_dur = 5000; // duration of soft start in millis
int fanl_update_freq = 10;
int fanl_pwm_freq_norm = 40; // normal pwm frequency in Hz
int fanl_pwm_freq_ss = 420; // soft start pwm frequency in Hz
PWMDevice fan_left(fanl_pin, fanl_table_rows, fanl_table_cols,
                   fanl_table_row_scalar, fanl_table_col_scalar,
                   M400_engineTemp, M400_batteryVoltage, USER_fanLeftOverride, fanl_min_pwm, fanl_max_pwm,
                   fanl_ss_dur, fanl_update_freq, fanl_pwm_freq_norm, fanl_pwm_freq_ss);

// right fan definition & partial initialization
int fanr_pin = 7;
int fanr_min_pwm = 0;
int fanr_max_pwm = 255;
int fanr_ss_dur = 5000; // duration of soft start in millis
int fanr_update_freq = 10;
int fanr_pwm_freq_norm = 40; // normal pwm frequency in Hz
int fanr_pwm_freq_ss = 420; // soft start pwm frequency in Hz
PWMDevice fan_right(fanr_pin, fanr_table_rows, fanr_table_cols,
                    fanr_table_row_scalar, fanr_table_col_scalar,
                    M400_engineTemp, M400_batteryVoltage, USER_fanRightOverride, fanr_min_pwm, fanr_max_pwm,
                    fanr_ss_dur, fanr_update_freq, fanr_pwm_freq_norm, fanr_pwm_freq_ss);

// water pump definition & partial initialization
int wp_pin = 6;
int wp_min_pwm = 0;
int wp_max_pwm = 255;
int wp_ss_dur = 5000; // duration of soft start in millis
int wp_update_freq = 10;
int wp_pwm_freq_norm = 40; // normal pwm frequency in Hz
int wp_pwm_freq_ss = 420; // soft start pwm frequency in Hz
PWMDevice water_pump(wp_pin, wp_table_rows, wp_table_cols,
                     wp_table_row_scalar, wp_table_col_scalar,
                     M400_engineTemp, M400_rpm, USER_wpOverride, wp_min_pwm, wp_max_pwm,
                     wp_ss_dur, wp_update_freq, wp_pwm_freq_norm, wp_pwm_freq_ss);


#endif
