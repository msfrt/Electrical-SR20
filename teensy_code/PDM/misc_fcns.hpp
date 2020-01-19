#ifndef MISC_FUNCTIONS_HPP
#define MISC_FUNCTIONS_HPP

// minimum voltage for the engine to be in "cranking" mode
const int cranking_starter_volt_threshold = 5;
// max rpm for the engine to still be in "cranking" mode
const int cranking_rpm_threshold = 2000;
// engine running min rpm threshold
const int engine_on_rpm_threshold = 900;
// engine cooldown state duration in milliseconds
const int engine_cooldown_duration = 2500;


// this function takes a reference to the engine state variable and returns a reference to the same variable.
// the engine state will be updated if determined necessary.
int &determine_engine_state(int &engine_state){
  static unsigned long cooldown_until_time;
  static unsigned long current_time;

  current_time = millis(); // update the time

  // determine if engine is cranking
  if (temp_volt >= cranking_starter_volt_threshold &&
      M400_rpm.value() <= cranking_rpm_threshold){
    engine_state = 1; // cranking

  // determine if engine is on
  } else if (M400_rpm.value() >= engine_on_rpm_threshold){
    engine_state = 2; // on

  // determine if cooldown or off
  } else {
    // was the engine just running? If it was and we made it to the else statement, trigger cooldown mode.
    // note: this can only be triggered if the engine was determined previously to be on.
    if (engine_state == 2){
      cooldown_until_time = current_time + engine_cooldown_duration;
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


#endif
