#ifndef SIGNAL_SAMPLER_H
#define SIGNAL_SAMPLER_H

#include "Arduino.h"

// teensy voltage in mV10
const int teensy_voltage_mV10 = 33000;

// initialize global variable for easier conversions.
int read_resolution_max;

// sets the read resultion for sensors, and returns the max value associated with the
// number of bits possible. (default for arduino is 10bits)
int set_resolution(int bits=10);

// the class for any given sensor
class Sensor{
  private:
    String name_ = "";
    int pin_;
    int value_;
    int analog_value_;
    int analog_running_total_;
    int analog_read_count_;
    int analog_min_;
    int analog_max_;
    int analog_avg_;
    double sensor_min_;
    double sensor_max_;
    double sensor_avg_;
    const bool temp_sensor_;

    // sensor-specific stuffs
    int zero_volt_mV10_ = 0;
    double mV10_per_sensor_unit_ = 0.00; // ex. 30 for 3mV/degree C (type:double to avoid integer math data conversion)
    int z1_ = 1200; // z1 & z2 are for the voltage divider (units are ohms)
    int z2_ = 2200; // check the wikipedia page for a diagram en.wikipedia.org/wiki/Voltage_divider
  public:

    // converts all of the analog values to meaningful values
    void maths(int conversions=3);

};


#endif
