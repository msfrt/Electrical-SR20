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

// NeoPixel parameters
const int pixels_top_pin = 3; // teensy pin #
const int pixels_left_pin= 2;
const int pixels_right_pin = 4;
const int pixels_top_cnt = 16; // number of LEDs
const int pixels_left_cnt = 4;
const int pixels_right_cnt = 4;
      int pixel_brightness_percent = 5; // 0 - 100; 100 is blinding...

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

// state_racing_bitmap
#include "sr_bitmap.hpp"

// info screen struct and functions
#include "info_screen.hpp"

// debugging timer
EasyTimer debug(50);

// used for dynamically changing clock speed :-)))
// #if defined(__IMXRT1062__)
// extern "C" uint32_t set_arm_clock(uint32_t frequency);
// #endif

InfoScreen info_test(display_right, M400_batteryVoltage, M400_engineTemp, M400_oilTemp, M400_groundSpeed);

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

  // clear the screens
  display_left.fillScreen(ILI9341_BLACK);
  display_right.fillScreen(ILI9341_BLACK);

  M400_rpm = 11800;
  M400_gear = 2;

  int gap_y_px = 12;
  int gap_x_px = 1;

  display_left.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  display_left.setTextWrap(false);
  display_left.setFont(LiberationMono_40_Bold);
  display_left.setCursor(gap_x_px, gap_y_px);
  display_left.println("OIL: 87.4");
  display_left.setCursor(gap_x_px, DISPLAY_HEIGHT / 4 + gap_y_px);
  display_left.println("WTR: 69.4");
  display_left.setCursor(gap_x_px, (DISPLAY_HEIGHT / 4) * 2 + gap_y_px);
  display_left.println("RPM:  9.4");
  display_left.setCursor(gap_x_px, (DISPLAY_HEIGHT / 4) * 3 + gap_y_px);
  display_left.println("OILT:  69");

  display_left.drawFastHLine(0, (DISPLAY_HEIGHT / 4) * 0, DISPLAY_WIDTH, ILI9341_GREEN);
  display_left.drawFastHLine(0, (DISPLAY_HEIGHT / 4) * 1, DISPLAY_WIDTH, ILI9341_GREEN);
  display_left.drawFastHLine(0, (DISPLAY_HEIGHT / 4) * 2, DISPLAY_WIDTH, ILI9341_GREEN);
  display_left.drawFastHLine(0, (DISPLAY_HEIGHT / 4) * 3, DISPLAY_WIDTH, ILI9341_GREEN);
  display_left.drawFastHLine(0, (DISPLAY_HEIGHT / 4) * 4 - 1, DISPLAY_WIDTH, ILI9341_GREEN);


  // display lana del rey
  //display_left.writeRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, (uint16_t*)lana1);
  //display_right.writeRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, (uint16_t*)lana2);

  // info screen struct testing
  M400_batteryVoltage = 12.692;
  M400_engineTemp = 45.7832;
  M400_oilTemp = 98.2354;
  M400_groundSpeed = 10.34;

  info_test.set_labels("BAT:", "ENGT:", "OILT:", "SPD:");
  info_test.set_numdigits(5, 4, 4, 5);
  info_test.set_precisions(4, 4, 4, 4);
  info_test.print_labels();
  info_test.print_lines();
  info_test.update_signals();

  M400_batteryVoltage = 14.2345;
  M400_engineTemp = 98.34;
  M400_oilTemp = 12.26305;
  M400_groundSpeed = 11.234;

  Serial.println("line1");
  info_test.update_signals();
  Serial.println("line2");
}



void loop() {

  info_test.update_signals();

  if (millis() < 4000){

  } else if (millis() < 10000){
    M400_batteryVoltage = 12.69;
    M400_engineTemp = 105;
    info_test.sig2_warning = true;
    M400_oilTemp = 65.2;
    M400_groundSpeed = 35.54;
  }


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
    M400_engineTemp = M400_engineTemp.value();
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
