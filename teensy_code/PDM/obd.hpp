#ifndef ONBOARD_DIAGNOSTICS_HPP
#define ONBOARD_DIAGNOSTICS_HPP

#include <EasyTimer.h>
#include <Adafruit_NeoPixel.h>
#include "sigs_inside.hpp"

int OBDFLAG_oil_pressure = 0;

// this is where you should put your diagnistic checks. Each check should be enclosed in their own timer.
void obd_main(){
  static EasyTimer user_override_timout_check_timer(2); // 2Hz
  static EasyTimer M400_timout_check_timer(2); // 2Hz
  static EasyTimer oil_pressure_check_timer(10);  //


  // user override time-outs
  if (user_override_timout_check_timer.isup()){
    USER_wpOverride.timeout_check();
    USER_fanLeftOverride.timeout_check();
    USER_fanRightOverride.timeout_check();
    USER_brakeLightOverride.timeout_check();
  }

  // M400 timeouts
  if (M400_timout_check_timer.isup()){
    M400_rpm.timeout_check();
    M400_engineTemp.timeout_check();
    M400_batteryVoltage.timeout_check();
    M400_oilPressure.timeout_check();
  }


  // oil pressure acceptance check
  if (oil_pressure_check_timer.isup()){
    obd_oil_pressure_acceptence(M400_oilPressure, M400_rpm);
  }


}


bool obd_oil_pressure_acceptence(StateSignal &oil_pressure, StateSignal &rpm){
  static unsigned long last_good_time = 0;
  static bool oil_pressure_good = true;

  if 
}



#endif
