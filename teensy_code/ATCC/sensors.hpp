#ifndef SENSORS_ATCC_HPP
#define SENSORS_ATCC_HPP

#include <ReadADC.h>

// ADC CS teensy pins
#define ADC1_CS 10
#define ADC2_CS 9
#define ADC3_CS 8

// ADCs
ADCChip adc1(ADC1_CS);
ADCChip adc2(ADC2_CS);
ADCChip adc3(ADC3_CS);

// front sensors
ADCSensor sus_travel_fl_sens(0, 0, 1000); // will need corrected values
ADCSensor sus_travel_fr_sens(2, 0, 1000); // (sensor is not chosen yet  ??? nice.)

ADCSensor brake_temp_fr_sens(10, 500, 5);
ADCSensor brake_temp_fl_sens(4, 500, 5);

ADCSensor brake_pressure_front_sens(1, 500, 2);
ADCSensor brake_pressure_rear_sens(3, 500, 2);

ADCSensor swa_sens(7, 0, 0); // can't do yet

ADCSensor tire_temp_fr_inner_sens(5, 400, 30);
ADCSensor tire_temp_fr_middle_sens(6, 400, 30);
ADCSensor tire_temp_fr_outer_sens(0, 400, 30);
ADCSensor tire_temp_fl_inner_sens(0, 400, 30);
ADCSensor tire_temp_fl_middle_sens(4, 400, 30);
ADCSensor tire_temp_fl_outer_sens(6, 400, 30);

ADCSensor coolant_temp_mid_sens(4, 0, 1000); // leave as is. returns voltage in V

// rear sensors
ADCSensor sus_travel_rr_sens(2, 0, 1000);
ADCSensor sus_travel_rl_sens(0, 0, 1000);

ADCSensor coolant_temp_outlet_sens(4, 0, 1000);
ADCSensor coolant_temp_inlet_sens(6, 0, 1000);

ADCSensor tire_temp_rl_inner_sens(0, 400, 30);
ADCSensor tire_temp_rl_middle_sens(4, 400, 30);
ADCSensor tire_temp_rl_outer_sens(6, 400, 30);
ADCSensor tire_temp_rr_inner_sens(5, 400, 30);
ADCSensor tire_temp_rr_middle_sens(6, 400, 30);
ADCSensor tire_temp_rr_outer_sens(2, 400, 30);

ADCSensor brake_temp_rr_sens(7, 500, 5);
ADCSensor brake_temp_rl_sens(4, 500, 5);


void initialize_ADCs()
{
  adc1.begin();
  adc2.begin();
  adc3.begin();
}




void sample_ADCs_F()
{

  // 1,000 Hz
  static EasyTimer sample_timer_1(1000); // static variable prevents object from deletion when out of scope
  if (sample_timer_1.isup())
  {
    //adc3
    adc3.sample(sus_travel_fr_sens, sus_travel_fl_sens);
  }

  // 200 Hz
  static EasyTimer sample_timer_2(200);
  if (sample_timer_2.isup())
  {
    //adc2
    adc2.sample(brake_pressure_front_sens, brake_pressure_rear_sens, swa_sens);
  }

  // 100 Hz
  static EasyTimer sample_timer_3(100);
  if (sample_timer_3.isup())
  {
    //adc1
    adc1.sample(brake_temp_fr_sens, tire_temp_fr_inner_sens, tire_temp_fr_middle_sens, brake_temp_fl_sens);

    //adc2
    adc2.sample(tire_temp_fr_outer_sens, tire_temp_fl_inner_sens, tire_temp_fl_middle_sens, tire_temp_fl_outer_sens);

    //adc3
    adc3.sample(coolant_temp_mid_sens);
  }
} // end sample_ADCs_F()



void sample_ADCs_R()
{
  // 1,000 Hz
  static EasyTimer sample_timer_1(1000);
  if (sample_timer_1.isup())
  {
    //adc3
    adc3.sample(sus_travel_rr_sens, sus_travel_rl_sens);
  }

  // 100 Hz
  static EasyTimer sample_timer_2(100);
  if (sample_timer_2.isup())
  {
    //adc1
    adc1.sample(brake_temp_rr_sens, tire_temp_rr_inner_sens, tire_temp_rr_middle_sens, brake_temp_rl_sens);

    //adc2
    adc2.sample(tire_temp_rr_outer_sens, tire_temp_rl_inner_sens, tire_temp_rl_middle_sens, tire_temp_rl_outer_sens);

    //adc3
    adc3.sample(coolant_temp_outlet_sens, coolant_temp_inlet_sens);


  }
}




#endif
