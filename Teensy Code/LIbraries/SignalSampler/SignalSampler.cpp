#include "Arduino.h"
#include "SignalSampler.h"

int set_resolution(int bits){
  analogReadResolution(bits);
  read_resolution_max = pow(2, bits) - 1;
  return read_resolution_max;
}


void Sensor::maths(int conversions){
  switch (conversions) {
    case 3: // transform all three: min, max, average
      // convert the analog inputs into the teeny voltage (mV*10)
      this->analog_min_ = this->analog_min_ * (teensy_voltage_mV10 / read_resolution_max);

      // this jumble of arithmetic turns the voltage into a sensor value
      this->sensor_min_ = ( (this->analog_min_ - this->zero_volt_mV10_) / this->mV10_per_sensor_unit_ )
                          * this->inverse_factor_;
    case 2: // m
      this->analog_max_ = this->analog_max_ * (teensy_voltage_mV10 / read_resolution_max);
      this->sensor_max_ = ( (this->analog_max_ - this->zero_volt_mV10_) / this->mV10_per_sensor_unit_ )
                          * this->inverse_factor_;
    case 1:
      this->analog_avg_ = analog_running_total_ / analog_read_count_;
      this->analog_avg_ = this->analog_avg_ * (teensy_voltage_mV10 / read_resolution_max);
      this->sensor_avg_ = ( (this->analog_avg_ - this->zero_volt_mV10_) / this->mV10_per_sensor_unit_ )
                          * this->inverse_factor_;
  }
}
