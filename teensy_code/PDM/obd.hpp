#ifndef ONBOARD_DIAGNOSTICS_HPP
#define ONBOARD_DIAGNOSTICS_HPP

#include <EasyTimer.h>
#include <Adafruit_NeoPixel.h>
#include "sigs_inside.hpp"
#include "rainbow_pixels.hpp"

// add low fuel pressure warning

int OBDFLAG_oil_pressure = 0;

// Oil pressure parameters
const int OBDPARAM_oil_pressure_min_rpm = 2500;   // only check for oil issues above this rpm
const int OBDPARAM_oil_pressure_dip_time_ms = 5000;   // time allowed below minimum pressure before raising a flag
const int OBDPARAM_oil_pressure_min_percent_allowed = 70;  // percent of predicted oil pressure before it is determined bad

//

// forward references
bool obd_oil_pressure_acceptence(StateSignal&, StateSignal&);
void obd_leds();

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

  obd_leds();

}


bool obd_oil_pressure_acceptence(StateSignal &oil_pressure, StateSignal &rpm){
  static unsigned long good_until_time = 5000;
  static bool oil_pressure_good = true;
  static float predicted_pressure = 0.0;

  // check if the sensors are valid in the first place.
  if (!oil_pressure.is_valid() || !rpm.is_valid())
    return true;

  // calcluate the predicted pressure
  // Function generated: 02/15/2020 10:28:53
  predicted_pressure = 20.068 * log(rpm.value() - 1776.948) - 116.958;

  // current oil pressure is acceptable
  if ((oil_pressure.value() * 100) / predicted_pressure >= OBDPARAM_oil_pressure_min_percent_allowed){
    oil_pressure_good = true;
    OBDFLAG_oil_pressure = 1;
    good_until_time = millis() + OBDPARAM_oil_pressure_dip_time_ms;


  // current oil pressure is unacceptable
  } else {
    oil_pressure_good = false;


    // raise a flag
    if (millis() > good_until_time && OBDFLAG_oil_pressure == 0){
      OBDFLAG_oil_pressure = 1;

      // send messages to the driver (send 10 times because this is important and can NOT be missed buy DD)
      for (int i = 0; i < 10; i++){
        // PDM 31 - driver display LED
        msg.buf[0] = 3; // LED color code RED
        msg.buf[1] = 0;
        msg.buf[2] = 0;
        msg.buf[3] = 0;
        msg.buf[4] = 0;
        msg.buf[5] = 0;
        msg.buf[6] = 0;
        msg.buf[7] = 0;
        msg.id = 281;
        msg.len = 8;
        cbus2.write(msg);

        // PDM 30 - driver message
        msg.buf[0] = 'O';
        msg.buf[1] = 'I';
        msg.buf[2] = 'L';
        msg.buf[3] = 'P';
        msg.buf[4] = ' ';
        msg.buf[5] = 'L';
        msg.buf[6] = 'O';
        msg.buf[7] = 'W';
        msg.id = 280;
        msg.len = 8;
        cbus2.write(msg);
      }
    }
  }

  return oil_pressure_good;
}

void obd_leds(){
  static bool leds_on = true;

  // if there is a critical flag present, blink red
  if (OBDFLAG_oil_pressure){
    static EasyTimer bad_blink(10);
    if (bad_blink.isup()){
      if (leds_on){
        leds_on = false;
        GLO_obd_neopixel.setPixelColor(0, 0, 0, 0);
        GLO_obd_neopixel.show();
      } else {
        leds_on = true;
        GLO_obd_neopixel.setPixelColor(0, 255, 0, 0);
        GLO_obd_neopixel.show();
      }
    }

  // if pdm determines that the engine is cranking, flash orange really fast like rititititititit (engine noise)
  } else if (GLO_engine_state == 1){
    static EasyTimer cranking_blink(20);
    if (cranking_blink.isup()){
      if (leds_on){
        leds_on = false;
        GLO_obd_neopixel.setPixelColor(0, 0, 0, 0);
        GLO_obd_neopixel.show();
      } else {
        leds_on = true;
        GLO_obd_neopixel.setPixelColor(0, 255, 175, 0);
        GLO_obd_neopixel.show();
      }
    }


  // if the user is overriding a pwm control, blink purple
  } else if (USER_wpOverride.value() >= 1 || USER_fanLeftOverride.value() >= 1 ||
             USER_fanRightOverride.value() >= 1 || USER_brakeLightOverride.value() >= 1){
    static EasyTimer user_override_blink(4);
    if (user_override_blink.isup()){
      if (leds_on){
        leds_on = false;
        GLO_obd_neopixel.setPixelColor(0, 0, 0, 0);
        GLO_obd_neopixel.show();
      } else {
        leds_on = true;
        GLO_obd_neopixel.setPixelColor(0, 255, 0, 255);
        GLO_obd_neopixel.show();
      }
    }

  // no flags set. No reason to flash LEDs. Make them RGB
  } else {
    rainbow_pixels(GLO_obd_neopixel);
  }
}


#endif
