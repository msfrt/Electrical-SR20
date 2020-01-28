#include <StateCAN.h>
#include <Adafruit_NeoPixel.h>
#include <EasyTimer.h>

const int downshift12_rpm = 9362;
const int downshift23_rpm = 9703;
const int downshift34_rpm = 9783;
const int downshift45_rpm = 9855;
const int upshift12_rpm = 13224;
const int upshift23_rpm = 11900;
const int upshift34_rpm = 12000;
const int upshift45_rpm = 12000;

const int rpm_bar_min_bound = 5000;
const int rpm_bar_max_bound = 14000;

// returns a value 0-255 for the certain LED and subtracts the PWM output from the total
int led_pwm(int &total_pwm){
  static int temp_store;

  // LED should be completely on
  if (total_pwm >= 255){
    total_pwm -= 255;
    return 255;

  // LED should be completely off
  } else if (total_pwm <= 0){
    total_pwm -= 255;
    return 0;

  // LED should be dimmed
  } else {
    temp_store = total_pwm;
    total_pwm = 0;
    return temp_store;
  }
}

// sets the rpm bar to the desired output. Returns a boolean true if the driver should shift
bool rpm_bar(Adafruit_NeoPixel &top, const int &numtop, StateSignal &rpm, StateSignal &gear){

  static const int idle_rpm = 2000;
  static const int rev_limit_rpm = 14000;

  static int bar_percentage;
  static int bar_pwms;
  static int bar_mode;
  static EasyTimer rpm_flash_timer(20);
  static bool rpm_flash_on;
  static const int max_bar_pwm_posns = numtop * 256;
  static int pwm_current_led;

  // determine if downshift mode
  if ((rpm.value() <= downshift12_rpm && (gear.value() > 1.9 && gear.value() < 2.1)) || // currently gear 2
      (rpm.value() <= downshift23_rpm && (gear.value() > 2.9 && gear.value() < 3.1)) || // 3
      (rpm.value() <= downshift34_rpm && (gear.value() > 3.9 && gear.value() < 4.1)) || // 4
      (rpm.value() <= downshift45_rpm && (gear.value() > 4.9 && gear.value() < 5.1))){  // 5
    bar_mode = 1;

  // determine if upshift mode
  } else if ((rpm.value() >= upshift12_rpm && (gear.value() > 0.9 && gear.value() < 1.1)) ||
             (rpm.value() >= upshift23_rpm && (gear.value() > 1.9 && gear.value() < 2.1)) ||
             (rpm.value() >= upshift34_rpm && (gear.value() > 2.9 && gear.value() < 3.1)) ||
             (rpm.value() >= upshift45_rpm && (gear.value() > 3.9 && gear.value() < 4.1))){
    bar_mode = 2;

  // otherwise we are in rpmbar mode
  } else {
    bar_mode = 0;
  }

  // downshift
  if (bar_mode == 1){
    // set first third to be purple
    for (int i = 0; i <= numtop / 3; ++i){
      top.setPixelColor(i, 100, 0, 255);
    }
    for (int i = numtop / 3; i <= numtop; ++i){
      top.setPixelColor(i, 0, 0, 0);
    }

  // upshift
  } else if (bar_mode == 2){
    if (rpm_flash_timer.isup()){

      // set the LEDs off
      for (int i = 0; i <= numtop; ++i){
        top.setPixelColor(i, 0, 0, 0);
      }

      // lights were on, turn them off
      if (rpm_flash_on){
        rpm_flash_on = false;
      // turn the last half of lights on
      } else {
        rpm_flash_on = true;
        for (int i = numtop / 2; i <= numPixelsTop; ++i){
          top.setPixelColor(i, 255, 0, 0);
        }
      }
    }


  // else, show rpm bar mode
  } else {

    // determine bar percentage * 10 (variable depending on gear). It's out of 1000 for more precision (map is int math)
    if (gear.value() < 1){ // neutral
      bar_percentage = map(rpm.value(), idle_rpm, rev_limit_rpm, 0, 1000);
    } else if (gear.value() > 0.9 && gear.value() < 1.1){ // gear 1
      bar_percentage = map(rpm.value(), idle_rpm, upshift12_rpm, 0, 1000);
    } else if (gear.value() > 1.9 && gear.value() < 2.1){ // gear 2
      bar_percentage = map(rpm.value(), downshift12_rpm, upshift23_rpm, 0, 1000);
    } else if (gear.value() > 2.9 && gear.value() < 3.1){ // gear 3
      bar_percentage = map(rpm.value(), downshift23_rpm, upshift34_rpm, 0, 1000);
    } else if (gear.value() > 3.9 && gear.value() < 4.1){ // gear 4
      bar_percentage = map(rpm.value(), downshift34_rpm, upshift45_rpm, 0, 1000);
    } else { // gear 5
      bar_percentage = map(rpm.value(), downshift45_rpm, rev_limit_rpm, 0, 1000);
    }

    // turn them all off so we can set the ones that we want to be on
    for (int i = 0; i <= numtop; i++){
      top.setPixelColor(i, 0, 0, 0);
    }

    // the total pwms that we need to set in the bar
    bar_pwms = (bar_percentage * max_bar_pwm_posns) / 1000;

    // set the LEDs to be on
    for (int i = 0; i <= numtop; i++){
      pwm_current_led = led_pwm(bar_pwms);
      // the divisors take care of the gradients
      top.setPixelColor(i, pwm_current_led / (numtop - i), pwm_current_led / ((i + 0.01) * 2), 0);
    }

  }


  // finally, send out the values that we just set
  top.show();

  // if the driver should be shifting
  if (bar_mode == 2){
    return true;
  } else {
    return false;
  }
}
