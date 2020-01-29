// Dave Yonkers, 2020

#include <Adafruit_NeoPixel.h>
#include <StateCAN.h>
#include <EasyTimer.h>

// NeoPixel parameters
const int pixels_top_pin = 3; // teensy pin #
const int pixels_left_pin= 2;
const int pixels_right_pin = 4;
const int pixels_top_cnt = 16; // number of LEDs
const int pixels_left_cnt = 4;
const int pixels_right_cnt = 4;
      int pixel_brightness_percent = 10; // 0 - 100; 100 is blinding...

Adafruit_NeoPixel pixels_top =   Adafruit_NeoPixel(pixels_top_cnt,   pixels_top_pin,   NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_left =  Adafruit_NeoPixel(pixels_left_cnt,  pixels_left_pin,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_right = Adafruit_NeoPixel(pixels_right_cnt, pixels_right_pin, NEO_GRB + NEO_KHZ800);

// pins for the steering wheel buttons
const int button1_pin = 14;
const int button2_pin = 15;
      int button1_value = 0; // use these (button values) to store the return from button check function
      int button2_value = 0;
      int button1_state = 0; // states to keep track of button presses (internal usage to button check function.
      int button2_state = 0; // 0 does not necessarily mean off, and 1 does not necessarily mean on.
unsigned long button1_time=0;
unsigned long button2_time=0;
const unsigned long button_delay = 300; // @GLOBAL_PARAM - milliseconds - used in check_button to avoid double-presses

// modes for the screen and leds
int led_mode = 0;
int screen_mode = 0;

// include externally-written functions
#include "led_startup.hpp"
#include "rpm_bar.hpp"
#include "party_bar.hpp"
#include "lockup_indicator.hpp"

// signal definitions
#include "sigs_inside.hpp"

// debugging timer
EasyTimer debug(50);


void setup() {
  // initialze serial coms
  Serial.begin(115200);

  // init top pixels
  pixels_top.setBrightness(map(pixel_brightness_percent, 0, 100, 0, 255));
  pixels_top.begin();
  pixels_top.show();
  // init right pixels
  pixels_right.setBrightness(map(pixel_brightness_percent, 0, 100, 0, 255));
  pixels_right.begin();
  pixels_right.show();
  // init left pixels
  pixels_left.setBrightness(map(pixel_brightness_percent, 0, 100, 0, 255));
  pixels_left.begin();
  pixels_left.show();

  // initialize buttons
  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(button2_pin, INPUT_PULLUP);

  // fun LED startup sequence. Last parameter is time multiplier. 0 is fastest, 5 is pretty darn slow.
  // if you set it higher than 5, I have respect for your patience
  led_startup(pixels_top, pixels_top_cnt, pixels_left, pixels_left_cnt, pixels_right, pixels_right_cnt, 1);

  M400_rpm = 5000;
  M400_gear = 2;

}


void loop() {

  // if button 1 was double pressed
  button1_value = check_button(button1_pin, button1_state, button1_time);
  if (button1_value == 2){
    led_mode = 69;
    button1_value = 0;
  } else if (button1_value == 1) {
    led_mode = 0;
    button1_value = 0;
  }

  // normal functioning mode
  if (led_mode == 0){
    rpm_bar(pixels_top, pixels_top_cnt, M400_rpm, M400_gear);

    lockup_indicator(pixels_left, 0, M400_groundSpeedLeft, MM5_Ax, ATCCF_brakePressureF, ATCCF_brakePressureR);
    lockup_indicator(pixels_left, 3, M400_driveSpeedLeft, MM5_Ax, ATCCF_brakePressureF, ATCCF_brakePressureR);
    lockup_indicator(pixels_right, 0, M400_groundSpeedRight, MM5_Ax, ATCCF_brakePressureF, ATCCF_brakePressureR);
    lockup_indicator(pixels_right, 3, M400_driveSpeedRight, MM5_Ax, ATCCF_brakePressureF, ATCCF_brakePressureR);

    // these next four LEDs are currently unused, so set them to blank
    pixels_left.setPixelColor(1, 0, 0, 0);
    pixels_left.setPixelColor(2, 0, 0, 0);
    pixels_right.setPixelColor(1, 0, 0, 0);
    pixels_right.setPixelColor(2, 0, 0, 0);

    pixels_top.show();
    pixels_left.show();
    pixels_right.show();

  } else if (led_mode == 69){
    party_bar(pixels_top, pixels_top_cnt, pixels_left, pixels_left_cnt, pixels_right, pixels_right_cnt);
  }

  if (debug.isup()){

    if (M400_rpm.value() < 11800){
      M400_rpm = M400_rpm.value() + 15;
    }
    Serial.println(M400_rpm.value());
  }

}




// takes a button pin and a reference to the state. First, updates state. Returns an int according to the number of
// times the button was pressed (up to 2). returns 0 if nothing.
int check_button(const int &pin, int &state, unsigned long &time){

  // single press
  if ((digitalRead(pin) == LOW) && (millis() - time >= button_delay) && (state < 1)){
    state = 1;
    time = millis();
    return 1;

  // double press
  } else if ((digitalRead(pin) == LOW) && (millis() - time >= (button_delay / 2) && (state < 2))){
    state = 2;
    time = millis();
    return 2;

  // reset the state if timeout
  } else if ((state >= 1) && (millis() - time >= button_delay)){
    state = 0;
  }

  return 0;
}
