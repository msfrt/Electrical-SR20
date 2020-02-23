#include <ReadADC.h>
#include <EasyTimer.h>

// Sensor definitions

// ADCSensor my_sensor(int chip_select, int adc_channel, int sensor_zero_mV, int mV_per_sensor_unit);

// ADC1
ADCSensor keepalive_volt_sens(3, 0, 0, 313);
ADCSensor       pdm_volt_sens(3, 1, 0, 313);
ADCSensor      data_volt_sens(3, 2, 0, 313);
ADCSensor      main_volt_sens(3, 3, 0, 313);
ADCSensor      fanl_volt_sens(3, 4, 0, 313);
ADCSensor      fanr_volt_sens(3, 5, 0, 313);
ADCSensor        wp_volt_sens(3, 6, 0, 313);
ADCSensor      fuel_volt_sens(3, 7, 0, 313);
// ADC2
ADCSensor    fuel_current_sens(10, 1, 2500, 40);
ADCSensor      wp_current_sens(10, 2, 2500, 40);
ADCSensor    fanr_current_sens(10, 3, 2500, 40);
ADCSensor    fanl_current_sens(10, 4, 2500, 40);
ADCSensor     pdm_current_sens(10, 5, 2500, 20);
ADCSensor brakelight_volt_sens(10, 6,    0, 313);
ADCSensor    starter_volt_sens(10, 7,    0, 313);


// this function uses the local timers to determine when to call the ADCSensor sample function
void sample_ADCs(){

  // static timer definitions (keeps them in the local scope & never deleted)
  static EasyTimer sample_timer_1(2000); // 2,000Hz
  static EasyTimer sample_timer_2(1000); // 1,000Hz

  if (sample_timer_1.isup()){
          pdm_volt_sens.sample_n(5);
         fanl_volt_sens.sample_n(5);
         fanr_volt_sens.sample_n(5);
           wp_volt_sens.sample_n(5);
         main_volt_sens.sample_n(5);
         data_volt_sens.sample_n(5);
         fuel_volt_sens.sample_n(5);
    keepalive_volt_sens.sample_n(5);
       pdm_current_sens.sample_n(5);
      fanl_current_sens.sample_n(5);
      fanr_current_sens.sample_n(5);
        wp_current_sens.sample_n(5);
      fuel_current_sens.sample_n(5);
  }

  if (sample_timer_2.isup()){
    brakelight_volt_sens.sample_n(5);
       starter_volt_sens.sample_n(5);
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
