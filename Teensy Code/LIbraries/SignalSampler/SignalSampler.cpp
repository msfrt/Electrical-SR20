#include "Arduino.h"
#include "SignalSampler.h"

int set_resolution(int bits){
  analogReadResolution(bits);
  read_resolution_max = pow(2, bits) - 1;
  return read_resolution_max;
}


double Sensor::maths(){
  this->analog_avg_ = analog_running_total_ / analog_read_count_;

  // convert the analog inputs into the teeny voltage (mV*10)
  this->sensor_avg_ = this->sensor_avg_ * (teensy_voltage_mV10 / read_resolution_max);
}
