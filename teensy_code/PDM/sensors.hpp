#include <ReadADC.h>
#include <EasyTimer.h>
#include "board_temp.hpp"

// Sensor definitions

#define ADC1_CS 3
#define ADC2_CS 10

// ADC1
ADCSensor keepalive_volt_sens(ADC1_CS, 1, 0, 313);
ADCSensor       pdm_volt_sens(ADC1_CS, 2, 0, 313);
ADCSensor      data_volt_sens(ADC1_CS, 3, 0, 313);
ADCSensor      main_volt_sens(ADC1_CS, 4, 0, 313);
ADCSensor      fanl_volt_sens(ADC1_CS, 5, 0, 313);
ADCSensor      fanr_volt_sens(ADC1_CS, 6, 0, 313);
ADCSensor        wp_volt_sens(ADC1_CS, 7, 0, 313);
ADCSensor      fuel_volt_sens(ADC1_CS, 8, 0, 313);
// ADC2
ADCSensor    fuel_current_sens(ADC2_CS, 2, 2500, 20);
ADCSensor      wp_current_sens(ADC2_CS, 3, 2500, 20);
ADCSensor    fanr_current_sens(ADC2_CS, 4, 2500, 20);
ADCSensor    fanl_current_sens(ADC2_CS, 5, 2500, 20);
ADCSensor     pdm_current_sens(ADC2_CS, 6, 2500, 20);
ADCSensor brakelight_volt_sens(ADC2_CS, 7,    0, 313);
ADCSensor    starter_volt_sens(ADC2_CS, 8,    0, 313);
// stupid board temp
BoardTemp pdm_board_temp_sens(21, GLO_read_resolution_bits, 22.2222, 187);


// this function uses the local timers to determine when to call the ADCSensor sample function
void sample_ADCs(){

  // static timer definitions (keeps them in the local scope & never deleted)
  static EasyTimer sample_timer_1(10000); // 10,000Hz
  static EasyTimer sample_timer_2(5000); // 5,000Hz

  if (sample_timer_1.isup()){

          keepalive_volt_sens.sample();
          pdm_volt_sens.sample();
          data_volt_sens.sample();
          main_volt_sens.sample();
         fanl_volt_sens.sample();
         fanr_volt_sens.sample();
           wp_volt_sens.sample();
         fuel_volt_sens.sample();
       pdm_current_sens.sample();
      fanl_current_sens.sample();
      fanr_current_sens.sample();
        wp_current_sens.sample();
      fuel_current_sens.sample();
  }

  if (sample_timer_2.isup()){
    brakelight_volt_sens.sample();
       starter_volt_sens.sample();
  }
}


// this function must be called in the setup() portion of the main file.
// It initializes the pins for comunication
void initialize_ADCs(){
  // ADC1
   keepalive_volt_sens.begin();
         pdm_volt_sens.begin();
        data_volt_sens.begin();
        main_volt_sens.begin();
        fanl_volt_sens.begin();
        fanr_volt_sens.begin();
          wp_volt_sens.begin();
        fuel_volt_sens.begin();
  // ADC2
     fuel_current_sens.begin();
       wp_current_sens.begin();
     fanr_current_sens.begin();
     fanl_current_sens.begin();
      pdm_current_sens.begin();
  brakelight_volt_sens.begin();
     starter_volt_sens.begin();
}
