#ifndef SENSORS_ATCC_HPP
#define SENSORS_ATCC_HPP

#include <ReadADC.h>

// ADC CS teensy pins
#define ADC1_CS 10
#define ADC2_CS 9
#define ADC3_CS 8

ADCSensor sus_travel_fl_sens(ADC3_CS, 3, 0, 1000); // will need corrected values
ADCSensor sus_travel_fr_sens(ADC3_CS, 0, 0, 1000); // (sensor is not chosen yet  ??? nice.)

ADCSensor brake_temp_fr_sens(ADC1_CS, 4, 500, 5);
ADCSensor brake_temp_fl_sens(ADC1_CS, 7, 500, 5);

ADCSensor brake_pressure_front_sens(ADC2_CS, 1, 0, 0);
ADCSensor brake_pressure_rear_sens(ADC2_CS, 2, 0, 0);

ADCSensor swa_sens(ADC2_CS, 4, 0, 0);

ADCSensor tire_temp_fr_inner_sens(ADC1_CS, 5, 0, 0);
ADCSensor tire_temp_fr_middle_sens(ADC1_CS, 6, 0, 0);
ADCSensor tire_temp_fr_outer_sens(ADC2_CS, 0, 0, 0);
ADCSensor tire_temp_fl_inner_sens(ADC2_CS, 3, 0, 0);
ADCSensor tire_temp_fl_middle_sens(ADC2_CS, 5, 0, 0);
ADCSensor tire_temp_fl_outer_sens(ADC2_CS, 6, 0, 0);

ADCSensor coolant_temp_mid_sens(ADC3_CS, 5, 0, 1); // should return a voltage that will be passed into a bosch temp fcn


void sample_ADCs_F(){

  // 1,000 Hz
  static EasyTimer sample_timer_1(1000); // static variable prevents object from deletion when out of scope
  if (sample_timer_1.isup()){
    sus_travel_fl_sens.sample();
    sus_travel_fr_sens.sample();
  }
}


void initialize_ADCs_F(){
  sus_travel_fl_sens.begin();
  sus_travel_fr_sens.begin();
}

#endif
