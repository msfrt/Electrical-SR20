#ifndef SENSORS_ATCC_HPP
#define SENSORS_ATCC_HPP

#include <ReadADC.h>

// ADC CS teensy pins
#define ADC1_CS 10
#define ADC2_CS 9
#define ADC3_CS 8


// front sensors
ADCSensor sus_travel_fl_sens(ADC3_CS, 3, 0, 1000); // will need corrected values
ADCSensor sus_travel_fr_sens(ADC3_CS, 0, 0, 1000); // (sensor is not chosen yet  ??? nice.)

ADCSensor brake_temp_fr_sens(ADC1_CS, 4, 500, 5);
ADCSensor brake_temp_fl_sens(ADC1_CS, 7, 500, 5);

ADCSensor brake_pressure_front_sens(ADC2_CS, 1, 500, 2);
ADCSensor brake_pressure_rear_sens(ADC2_CS, 2, 500, 2);

ADCSensor swa_sens(ADC2_CS, 4, 0, 0); // can't do yet

ADCSensor tire_temp_fr_inner_sens(ADC1_CS, 5, 400, 30);
ADCSensor tire_temp_fr_middle_sens(ADC1_CS, 6, 400, 30);
ADCSensor tire_temp_fr_outer_sens(ADC2_CS, 0, 400, 30);
ADCSensor tire_temp_fl_inner_sens(ADC2_CS, 3, 400, 30);
ADCSensor tire_temp_fl_middle_sens(ADC2_CS, 5, 400, 30);
ADCSensor tire_temp_fl_outer_sens(ADC2_CS, 6, 400, 30);

ADCSensor coolant_temp_mid_sens(ADC3_CS, 5, 0, 1000); // leave as is. returns voltage in V

// rear sensors
ADCSensor sus_travel_rr_sens(ADC3_CS, 0, 0, 1000);
ADCSensor sus_travel_rl_sens(ADC3_CS, 3, 0, 1000);

ADCSensor coolant_temp_outlet_sens(ADC3_CS, 5, 0, 1000);
ADCSensor coolant_temp_inlet_sens(ADC3_CS, 7, 0, 1000);

ADCSensor tire_temp_rl_inner__sens(ADC2_CS, 3, 400, 30);
ADCSensor tire_temp_rl_middle__sens(ADC2_CS, 5, 400, 30);
ADCSensor tire_temp_rl_outer_sens(ADC2_CS, 6, 400, 30);
ADCSensor tire_temp_rr_inner_sens(ADC1_CS, 5, 400, 30);
ADCSensor tire_temp_rr_middle_sens(ADC1_CS, 6, 400, 30);
ADCSensor tire_temp_rr_outer_sens(ADC2_CS, 0, 400, 30);

ADCSensor brake_temp_rr_sens(ADC1_CS, 4, 500, 5);
ADCSensor brake_temp_rl_sens(ADC1_CS, 7, 500, 5);


void initialize_ADCs_F(){
  sus_travel_fl_sens.begin();
  sus_travel_fr_sens.begin();
  brake_temp_fl_sens.begin();
  brake_temp_fr_sens.begin();
  brake_pressure_front_sens.begin();
  brake_pressure_rear_sens.begin();
  swa_sens.begin();
  tire_temp_fr_inner_sens.begin();
  tire_temp_fr_middle_sens.begin();
  tire_temp_fr_outer_sens.begin();
  tire_temp_fl_inner_sens.begin();
  tire_temp_fl_middle_sens.begin();
  tire_temp_fl_outer_sens.begin();

void initialize_ADCs_R(){
  sus_travel_rr_sens.begin();
  sus_travel_rl_sens.begin();
  coolant_temp_outlet_sens.begin();
  coolant_temp_inlet_sens.begin();
  tire_temp_rl_inner__sens.begin();
  tire_temp_rl_middle__sens.begin();
  tire_temp_rl_outer_sens.begin();
  tire_temp_rr_inner_sens.begin();
  tire_temp_rr_middle_sens.begin();
  tire_temp_rr_outer_sens.begin();
  brake_temp_rr_sens.begin();
  brake_temp_rl_sens.begin();
}


void sample_ADCs_F(){

  // 1,000 Hz
  static EasyTimer sample_timer_1(1000); // static variable prevents object from deletion when out of scope
  if (sample_timer_1.isup()){
    sus_travel_fl_sens.sample();
    sus_travel_fr_sens.sample();
  }

  // 200 Hz
  static EasyTimer sample_timer_2(200);
  if sample_timer_2.isup(){
    // brake pressures
    brake_pressure_front_sens.sample();
    brake_pressure_rear_sens.sample();
    // steering angle
    swa_sens.sample();
  }

  // 100 Hz
  static EasyTimer sample_timer_3(100);
  if sample_timer_3.isup(){
    // rotor temps
    brake_temp_fl_sens.sample();
    brake_temp_fr_sens.sample();
    // tire temps
    tire_temp_fr_inner_sens.sample();
    tire_temp_fr_middle_sens.sample();
    tire_temp_fr_outer_sens.sample();
    tire_temp_fl_inner_sens.sample();
    tire_temp_fl_middle_sens.sample();
    tire_temp_fl_outer_sens.sample();
    // coolant temp
    coolant_temp_mid_sens.sample();
  }
} // end sample_ADCs_F()



void sample_ADCs_R(){
  // 1,000 Hz
  static EasyTimer sample_timer_1(1000);
  if (sample_timer_1.isup()){
    sus_travel_rl_sens.sample();
    sus_travel_rr_sens.sample();
  }

  // 100 Hz
  static EasyTimer sample_timer_2(100);
  if sample_timer_2.isup(){
    // rotor temps
    brake_temp_rl_sens.sample();
    brake_temp_rr_sens.sample();
    // tire temps
    tire_temp_rr_inner_sens.sample();
    tire_temp_rr_middle_sens.sample();
    tire_temp_rr_outer_sens.sample();
    tire_temp_rl_inner_sens.sample();
    tire_temp_rl_middle_sens.sample();
    tire_temp_rl_outer_sens.sample();
    // coolant temp
    coolant_temp_inlet_sens.sample();
    coolant_temp_outlet_sens.sample();
  }
}




#endif
