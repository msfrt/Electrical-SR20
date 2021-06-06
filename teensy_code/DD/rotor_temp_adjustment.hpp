#ifndef ROTOR_TEMP_ADJUSTMENT_HPP
#define ROTOR_TEMP_ADJUSTMENT_HPP

#include <StateCAN.h>
#include "sigs_inside.hpp"

// read the docstring below for an explanation
const float FL_ROTOR_DISC_AREA_RATIO = 0.5; 
const float FR_ROTOR_DISC_AREA_RATIO = 0.5;
const float RL_ROTOR_DISC_AREA_RATIO = 0.5;
const float RR_ROTOR_DISC_AREA_RATIO = 0.5;

/*
 * Calculate the actual brake rotor temp when given the measured temperature and the ratio of rotor material to
 * total rotor area.
 * 
 * The calculation is:
 * measured_temp_kelvin = [ (1 - A)(Ta)^4 + (A)(Td)^4 ]^(1/4)
 * Ta -> ambient temperature in Kelvin
 * Td -> brake disc temperature in Kelvin
 * 
 * @param StateSignal measured_temp - the measured temperature in C of the brake rotors from the IR sensor
 * @param StateSignal ambient_temp - the ambient air temperature in C (could be from the TMAP sensor)
 * @param StateSignal calculated_temp - the signal that the calculated temp in C will be assigned to.
 * @param float rotor_area_ratio - the ratio of actual steel rotor material in relation to total measured area.
 *                                 For example, if 20% of the rotor is slotted for cooling, 80% of the rotor's
 *                                 measured area will be steel. There, the input to the function would be 0.80.
 * @returns float - the calculated temp
 */
float calculate_adjusted_rotor_temp(StateSignal &measured_temp, StateSignal &ambient_temp, StateSignal &calculated_temp, float rotor_area_ratio){

  static const int K = 273; // add K to temperature measurements in C to get temperature in Kelvin

  // NOTE: I took out the ^4 because it would overflow. Even ^2 would overflow. This solution is not as accurate,
  // but is probably better than nothing
  double temp = (((double)measured_temp.value() + K) - (1.0 - rotor_area_ratio) * ((double)ambient_temp.value() + K)) / rotor_area_ratio - K;

  calculated_temp = temp;
  return temp;

}


#endif