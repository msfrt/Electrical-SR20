#ifndef LIGHT_SENSOR_DIMMER
#define LIGHT_SENSOR_DIMMER

int brightness_sensor(int &pin, int &analog_read_bits){
  static unsigned int max_read_val = pow(2, analog_read_bits) - 1;
  static unsigned long running_total = 0;
  static unsigned long running_sample_count = 0;
  static int last_calcd_value = 100;

  analog_read_bits

  return last_calcd_value;
}


#endif
