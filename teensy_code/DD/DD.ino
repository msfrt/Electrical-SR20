// Dave Yonkers, 2020

#include <Adafruit_NeoPixel.h>
#include <StateCAN.h>
#include <EasyTimer.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "ILI9341_t3n.h"
#define SPI0_DISP1

// fonts :)
#include "font_LiberationMonoBold.h"
#include "font_LiberationMonoBoldItalic.h"

// photos :) - converted with http://www.rinkydinkelectronics.com/t_imageconverter565.php
#include "lana1.c"
#include "lana2.c"
#include "fuck_kyle_busch.c"

// NeoPixel parameters
const int pixels_top_pin = 3; // teensy pin #
const int pixels_left_pin= 2;
const int pixels_right_pin = 4;
const int pixels_top_cnt = 16; // number of LEDs
const int pixels_left_cnt = 4;
const int pixels_right_cnt = 4;
      int pixel_brightness_percent = 10; // 0 - 100; 100 is blinding... 4 is the minimum for all LED bar colors to work

Adafruit_NeoPixel pixels_top =   Adafruit_NeoPixel(pixels_top_cnt,   pixels_top_pin,   NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_left =  Adafruit_NeoPixel(pixels_left_cnt,  pixels_left_pin,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_right = Adafruit_NeoPixel(pixels_right_cnt, pixels_right_pin, NEO_GRB + NEO_KHZ800);

// TFT display paramters
#define TFTL_DC 18
#define TFTL_CS 10
#define TFTL_MOSI 11
#define TFTL_MISO 12
#define TFTL_CLK 13
#define TFTL_RST 19
#define TFTL_BL 6
    int display_left_brightness_percent = 100;


#define TFTR_DC 5
#define TFTR_CS 9
#define TFTR_MOSI 11
#define TFTR_MISO 12
#define TFTR_CLK 13
#define TFTR_RST 17
#define TFTR_BL 7
    int display_right_brightness_percent = 100;

const int DISPLAY_HEIGHT = 240;
const int DISPLAY_WIDTH = 320;


ILI9341_t3n display_left = ILI9341_t3n(TFTL_CS, TFTL_DC, TFTL_RST);
ILI9341_t3n display_right = ILI9341_t3n(TFTR_CS, TFTR_DC, TFTR_RST);

// pins for the steering wheel buttons
const int button1_pin = 14;
const int button2_pin = 15;
unsigned long button1_time = 0;
unsigned long button2_time =0 ;
const unsigned long button_delay = 300; // @GLOBAL_PARAM - milliseconds - used in check_button to avoid double-presses

// modes for the screen and leds
int led_mode = 1;
int screen_mode = 1;

// include externally-written functions
#include "led_startup.hpp"
#include "rpm_bar.hpp"
#include "party_bar.hpp"
#include "warning_lights.hpp"
#include "lockup_indicator.hpp"

// signal definitions
#include "sigs_inside.hpp"

// bitmaps - generated here: http://javl.github.io/image2cpp/
#include "sr_bitmap.hpp"
#include "big_numbers.hpp"

// info screen struct and functions
#include "info_screen.hpp"

// big number display struct and functions
#include "big_number_display.hpp"

// debugging timer
EasyTimer debug(50);

// used for dynamically changing clock speed :-)))
// #if defined(__IMXRT1062__)
// extern "C" uint32_t set_arm_clock(uint32_t frequency);
// #endif

InfoScreen engine_vitals_right_screen(display_right, M400_rpm, M400_oilPressure, M400_engineTemp, M400_batteryVoltage,
                                        /* label */  "RPM:",   "OILP:",          "ENG:",          "BAT:",
                        /* max decimal precision */  4,        4,                4,               4);

InfoScreen auxilary_info_left_screen(display_left, M400_groundSpeed, M400_gear, M400_fuelUsed, PDM_fanLeftPWM,
                                      /* label */  "SPD:",           "GEAR:  ", "FUEL:",         "FAN:",
                      /* max decimal precision */  2,                0,         4,               4);


NumberDisplay gear_display_left(display_left, M400_gear, "GEAR");
NumberDisplay tc_display_left(display_left, M400_gear, "TC"); // change signal when C50 signals are set up

EasyTimer info_screen_update_timer(10); // rate at which the screens will check their variables for updates


void setup() {

  // dynamically change clock speed
  // #if defined(__IMXRT1062__)
  //   set_arm_clock(45000000);
  //   Serial.print("F_CPU_ACTUAL=");
  //   Serial.println(F_CPU_ACTUAL);
  // #endif

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

  // initialize screens
  display_left.begin();
  display_right.begin();
  display_left.setRotation(3);
  display_right.setRotation(1);
  // draw SR logo
  display_left.fillScreen(ILI9341_BLACK);
  display_right.fillScreen(ILI9341_BLACK);
  // set screen brightness
  pinMode(TFTL_BL, OUTPUT);
  pinMode(TFTR_BL, OUTPUT);
  analogWrite(TFTL_BL, map(display_left_brightness_percent, 0, 100, 0, 255));
  analogWrite(TFTR_BL, map(display_right_brightness_percent, 0, 100, 0, 255));
  // draw the OG state racing logo in white
  display_left.drawBitmap(0, 0, state_racing_bitmap, DISPLAY_WIDTH, DISPLAY_HEIGHT, ILI9341_WHITE);
  display_right.drawBitmap(0, 0, state_racing_bitmap, DISPLAY_WIDTH, DISPLAY_HEIGHT, ILI9341_WHITE);


  // initialize buttons
  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(button2_pin, INPUT_PULLUP);

  // fun LED startup sequence. Last parameter is time multiplier. 0 is fastest, 5 is pretty darn slow.
  // if you set it higher than 5, I have respect for your patience
  led_startup(pixels_top, pixels_top_cnt, pixels_left, pixels_left_cnt, pixels_right, pixels_right_cnt, 1);

  // non-zero signals for testing purposes only
  M400_rpm = 420;
  M400_oilPressure = 0;
  M400_engineTemp = 0;
  M400_fuelUsed = 100;
  M400_batteryVoltage = 12.653;
  M400_groundSpeed = 0;
  M400_gear = 5;
  M400_engineTemp = 19.32;
  PDM_fanLeftPWM = 0.1;

  auxilary_info_left_screen.begin();
  engine_vitals_right_screen.inv_factor_sig1 = 1000; // scale rpm down by 1000
  engine_vitals_right_screen.begin();



  // display lana del rey
  //display_left.writeRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, (uint16_t*)lana1);
  //display_right.writeRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, (uint16_t*)lana2);

  // fuck kyle busch
  //display_left.writeRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, (uint16_t*)fuck_kyle_busch);
  //display_right.writeRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, (uint16_t*)fuck_kyle_busch);

}






void loop() {


  // buttons and mode initialization ----------------------------------------

  // if button 1 was pressed changed the led mode
  if (check_button(button2_pin, button2_time)){
    if (++led_mode > 2){ // upper bound
      led_mode = 1;
    }
  }

  // if button 2 was pressed change the screen mode and run the required initilizations
  if (check_button(button1_pin, button1_time)){
    if (++screen_mode > 3){ // upper bound
      screen_mode = 1;
    }

    // auxilarry info screen and engine vitals screen
    if (screen_mode == 1){
      auxilary_info_left_screen.begin();
      engine_vitals_right_screen.begin();


    // gear screen and carry-over of engine vitals
    } else if (screen_mode == 2){
      gear_display_left.begin();


    } else if (screen_mode == 3){
      tc_display_left.begin();

    }
  }


  // LED updates ----------------------------------------

  if (led_mode == 1){
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

  } else if (led_mode == 2){
    party_bar(pixels_top, pixels_top_cnt, pixels_left, pixels_left_cnt, pixels_right, pixels_right_cnt);

  // tell the driver to come in
  } else if (led_mode == 10){
    full_warning_lights(pixels_top, pixels_left, pixels_right, "WHITE");

  // tell the driver to stop
  } else if (led_mode == 11){
    full_warning_lights(pixels_top, pixels_left, pixels_right, "YELLOW");

  // tell the driver to stop and shut off the car
  } else if (led_mode == 12){
    full_warning_lights(pixels_top, pixels_left, pixels_right, "RED");

  }



  // display updates --------------------------------------------------

  if (info_screen_update_timer.isup()){

    // Mode 1 - aux and engine info
    if (screen_mode == 1){
        auxilary_info_left_screen.update_signals();
        engine_vitals_right_screen.update_signals();

    // Mode 2 - gear display and engine info
    } else if (screen_mode == 2){
      gear_display_left.update();
      engine_vitals_right_screen.update_signals();

    } else if (screen_mode == 3){
      tc_display_left.update();
      engine_vitals_right_screen.update_signals();
    }
  }



  if (debug.isup()){
    M400_engineTemp = M400_engineTemp.value();
  }

}




// takes a button pin and a reference to the state. First, updates state. Returns an int according to the number of
// times the button was pressed (up to 2). returns 0 if nothing.
bool check_button(const int &pin, unsigned long &time){

  // single press
  if ((digitalRead(pin) == LOW) && (millis() - time >= button_delay)){
    time = millis();
    return true;
  }

  return false;
}
