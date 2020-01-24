#ifndef ONBOARD_DIAGNOSTICS_HPP
#define ONBOARD_DIAGNOSTICS_HPP

#include <EasyTimer.h>
#include "sigs_inside.hpp"

EasyTimer OBD_user_override_timout_check_timer(2); // 2Hz
EasyTimer OBD_M400_timout_check_timer(2); // 2Hz

// this is where you should put your diagnistic checks. Each check should be enclosed in their own timer.
void obd(){

  // user override time-outs
  if (OBD_user_override_timout_check_timer.isup()){
    USER_wpOverride.timeout_check();
    USER_fanLeftOverride.timeout_check();
    USER_fanRightOverride.timeout_check();
    USER_brakeLightOverride.timeout_check();
  }

  // M400 timeouts
  if (OBD_M400_timout_check_timer.isup()){
    M400_rpm.timeout_check();
    M400_engineTemp.timeout_check();
    M400_batteryVoltage.timeout_check();
  }

}

#endif
