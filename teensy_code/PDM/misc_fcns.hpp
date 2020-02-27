#ifndef MISC_FUNCTIONS_HPP
#define MISC_FUNCTIONS_HPP




template <class T1, class T2>
void engine_timer(EEPROM_Value<T1> &hours, EEPROM_Value<T2> &minutes){
  static unsigned int last_minute_millis = 0;

  // if the engine is not on (state 2), keep setting the last minute to the current time. This ensures that the
  // next minute is only triggered when the engine is actuall on.
  if (GLO_engine_state != 2){
    last_minute_millis = millis();
  }

  if (millis() > last_minute_millis + 60000){  // 60,000ms = 60s

    // minutes AND hours have changed
    if (eeprom.read(minutes) >= 59){
      hours = eeprom.read(hours) + 1;
      minutes = 0;
      eeprom.write(hours);
      eeprom.write(minutes);

    // just minutes have changed
    } else {
      minutes = eeprom.read(minutes) + 1;
      eeprom.write(minutes);
    }

    // update the last minute timer
    last_minute_millis = millis();
  }
}





// this function takes a reference to the engine state variable and returns a reference to the same variable.
// the engine state will be updated if determined necessary.
int &determine_engine_state(int &engine_state){
  static unsigned long cooldown_until_time;
  static unsigned long current_time;

  current_time = millis(); // update the time

  // determine if engine is cranking
  if (starter_volt_sens.last_calc_avg() >= GLO_cranking_starter_volt_threshold &&
      M400_rpm.value() <= GLO_cranking_rpm_threshold){
    engine_state = 1; // cranking

  // determine if engine is on
  } else if (M400_rpm.value() >= GLO_engine_on_rpm_threshold){
    engine_state = 2; // on

  // determine if cooldown or off
  } else {
    // was the engine just running? If it was and we made it to the else statement, trigger cooldown mode.
    // note: this can only be triggered if the engine was determined previously to be on.
    if (engine_state == 2){
      cooldown_until_time = current_time + GLO_engine_cooldown_duration;
    }

    // should we still be in the cooldown? Otherwise the engine is off
    if (current_time <= cooldown_until_time){
      engine_state = 3;
    } else {
      engine_state = 0;
    }
  }


  return engine_state;
}




bool brakelight(){
  static const int max_bl_pwm = pow(2, GLO_read_resolution_bits) - 1;

  // user override present
  if (USER_brakeLightOverride.value() >= 0 && USER_brakeLightOverride.value() <= 100){
    analogWrite(GLO_brakelight_teensy_pin, map(USER_brakeLightOverride.value(), 0, 100, 0, max_bl_pwm));
    return true;

  // brake-pressure exceeds minimum activation pressure
  } else if (ATCCF_brakePressureF.value() >= GLO_brakelight_min_pressure_F ||
      ATCCF_brakePressureR.value() >= GLO_brakelight_min_pressure_R){
    digitalWrite(GLO_brakelight_teensy_pin, HIGH);
    return true;

  // turn that shit off :)
  } else {
    digitalWrite(GLO_brakelight_teensy_pin, LOW);
    return false;
  }
}








#endif
