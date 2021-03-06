// Dave Yonkers, 2020

#include <Adafruit_NeoPixel.h>
#include <StateCAN.h>
#include <FlexCAN_T4.h>
#include <EasyTimer.h>
#include <BoardTemp.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "ILI9341_t3n.h"
#define SPI0_DISP1

#define READ_RESOLUTION_BITS 12

// can bus decleration
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> cbus1;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> cbus2;

//BoardTemp(int pin, int read_bits, int temp_cal, int mv_cal);
BoardTempDiode board_temp(21, READ_RESOLUTION_BITS, 26.2, 598);
EasyTimer board_temp_sample_timer(100);


// fonts :)
#include "font_LiberationMonoBold.h"
#include "font_LiberationMonoBoldItalic.h"

// photos :) - converted with http://www.rinkydinkelectronics.com/t_imageconverter565.php
#include "titans1.c"
#include "titans2.c"

// NeoPixel parameters
const int pixels_top_pin = 3; // teensy pin #
const int pixels_left_pin= 2;
const int pixels_right_pin = 4;
const int pixels_top_cnt = 16; // number of LEDs
const int pixels_left_cnt = 4;
const int pixels_right_cnt = 4;
      int pixel_brightness_percent = 5; // 0 - 100; 100 is blinding... 4 is the minimum for all LED bar colors to work
const int pixel_brightness_nighttime = 1;
const int pixel_brightness_daytime = 55;

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

#define DISPLAY_HEIGHT 240
#define DISPLAY_WIDTH 320

// used for light-sensor dimming
const int display_brightness_percent_nighttime = 75;
const int display_brightness_percent_daytime = 100;


ILI9341_t3n display_left = ILI9341_t3n(TFTL_CS, TFTL_DC, TFTL_RST);
ILI9341_t3n display_right = ILI9341_t3n(TFTR_CS, TFTR_DC, TFTR_RST);

GFXcanvas1 left_canvas(DISPLAY_WIDTH, DISPLAY_HEIGHT); // 128x32 pixel canvas
GFXcanvas1 right_canvas(DISPLAY_WIDTH, DISPLAY_HEIGHT); // 128x32 pixel canvas

// pins for the steering wheel buttons
const int button1_pin = 14;
const int button2_pin = 15;
unsigned long button1_time = 0;
unsigned long button2_time = 0 ;
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

unsigned long warning_lights_timeout_dur = 5000; // 15s

// signal definitions
#include "sigs_inside.hpp" // eventually move signal and can message definitions to shared folder, then link using the full file path

// CAN message definitions
#include "can_read.hpp"
#include "can_send.hpp"

// bitmaps - generated here: http://javl.github.io/image2cpp/
#include "sr_bitmap.hpp"
#include "big_numbers.hpp"

// info screen struct and functions
#include "info_screen.hpp"
// big number display struct and functions
#include "big_number_display.hpp"

// includes warning messages display
#include "user_message_display.hpp"

// lap timer screen
#include "lap_timer.hpp"

// brightness sensor reading and mapping function
#include "light_sensor.hpp"
EasyTimer light_sensor_sample_timer(100);
const int light_sensor_pin = 20;


char rpm_form[] = "%04.1f";
char oilp_form[] = "%03.1f";
char oilt_form[] = "%03.0f";
char engt_form[] = "%04.1f";
InfoScreen engine_vitals_right_screen(display_right, M400_rpm, M400_oilPressure, M400_oilTemp, M400_engineTemp,
                                        /* label */  "RPM:",   "OILP:",          "OILT:",      "ENG:",
                            /* string formatting */  rpm_form, oilp_form ,       oilt_form,    engt_form );

char speed_form[] = "%04.1f";
char battv_form[] = "%04.1f";
char bias_form[] = "%02.0f%%"; // the extra two %'s are not a typo!
char fanl_form[] = "%03.0f";
InfoScreen auxilary_info_left_screen(display_left, M400_groundSpeed, PDM_pdmVoltAvg, ATCCF_brakeBias, PDM_fanLeftPWM,
                                      /* label */  "SPD:",           "BAT:",         "BIAS:",         "FANS:",
                          /* string formatting */  speed_form,       battv_form,     bias_form,       fanl_form);


char fl_form[] = "%05.1f";
char fr_form[] = "%05.1f";
char rl_form[] = "%05.1f";
char rr_form[] = "%05.1f";
InfoScreen brake_temp_info_left_screen(display_left, ATCCF_rotorTempFL, ATCCF_rotorTempFR, ATCCR_rotorTempRL, ATCCR_rotorTempRR,
                                      /* label */    "FL:",             "FR:",             "RL:",             "RR:",
                          /* string formatting */    fl_form,           fr_form,           rl_form,           rr_form);


NumberDisplay gear_display_left(display_left, M400_gear, "GEAR");
NumberDisplay tc_display_left(display_left, C50_tcSet, "TC"); 

// lap-time array declaration
float prev_lap_times[4]; // arrays to hold the last 4 lap time details
float prev_lap_times_diff[4];
int prev_lap_numbers[4];
// lap-time display initialization
LapTimeDisplay lap_time_display_left(display_left, prev_lap_numbers, prev_lap_times, "LAP-T", false);
LapTimeDisplay lap_time_display_right(display_right, prev_lap_numbers, prev_lap_times_diff, "LAP-D", true);

// obd_message is a 9-byte char array defined in the can_read file
UserMessageDisplay warning_message_display(display_left, obd_message, "MESSAGE:", ILI9341_WHITE);

EasyTimer info_screen_update_timer(10); // rate at which the screens will check their variables for updates

EasyTimer debug(50); // debugging timer

// used for dynamically changing clock speed :-)))
// #if defined(__IMXRT1062__)
// extern "C" uint32_t set_arm_clock(uint32_t frequency);
// #endif


void setup() {

  // dynamically change clock speed
  // #if defined(__IMXRT1062__)
  //   set_arm_clock(45000000);
  //   Serial.print("F_CPU_ACTUAL=");
  //   Serial.println(F_CPU_ACTUAL);
  // #endif

  // set analog read resolution
  analogReadResolution(READ_RESOLUTION_BITS);

  // initilize CAN busses
  cbus1.begin();
  cbus1.setBaudRate(1000000);
  cbus2.begin();
  cbus2.setBaudRate(1000000);
  set_mailboxes();

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
  led_startup(pixels_top, pixels_left, pixels_right, 1);

  // scale rpm on the engine vitals screen down by 1000
  engine_vitals_right_screen.inv_factor_sig1 = 1000;

  // starts the screens at the main info screens
  auxilary_info_left_screen.begin();
  engine_vitals_right_screen.begin();


  // initilize board temp
  board_temp.begin();

  // fuck kyle busch
  //display_left.writeRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, (uint16_t*)fuck_kyle_busch);
  //display_right.writeRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, (uint16_t*)fuck_kyle_busch);

}




void loop() {

  // read CAN buses
  read_can1();
  read_can2();

  // determine the gear
  determine_gear(M400_rpm, M400_groundSpeed, M400_gear);


  // determine display and pixel brightness
  if (light_sensor_sample_timer.isup()){
    static int brightness_factor;
    brightness_factor = brightness_sensor(light_sensor_pin, READ_RESOLUTION_BITS);

    // if there was an update to the brightness
    if (brightness_factor > -1){

      pixel_brightness_percent = map(brightness_factor, 0, 100, pixel_brightness_nighttime, pixel_brightness_daytime);
      display_left_brightness_percent = map(brightness_factor, 0, 100, display_brightness_percent_nighttime, display_brightness_percent_daytime);
      display_right_brightness_percent = map(brightness_factor, 0, 100, display_brightness_percent_nighttime, display_brightness_percent_daytime);

      Serial.println(display_left_brightness_percent);
      Serial.println(pixel_brightness_percent);
      Serial.println(map(display_left_brightness_percent, 0, 100, 0, 255));
      Serial.println(map(pixel_brightness_percent, 0, 100, 0, 255));

      // update brightness
      analogWrite(TFTL_BL, map(display_left_brightness_percent, 0, 100, 0, 255));
      analogWrite(TFTR_BL, map(display_right_brightness_percent, 0, 100, 0, 255));
      pixels_top.setBrightness(map(pixel_brightness_percent, 0, 100, 0, 255));
      pixels_left.setBrightness(map(pixel_brightness_percent, 0, 100, 0, 255));
      pixels_right.setBrightness(map(pixel_brightness_percent, 0, 100, 0, 255));
    }
  }

  // check the laptrigger for a timeout
  TCGPS_laptrigger.timeout_check();

  // board temp sampling shenanigans
  if (board_temp_sample_timer.isup())
    board_temp.sample();


  // buttons and mode initialization ----------------------------------------

  // if button 1 was pressed changed the led mode
  if (check_button(button2_pin, button2_time)){
    if (++led_mode > 2){ // upper bound
      led_mode = 1;
    }
  }

  // if button 2 was pressed change the screen mode and run the required initializations
  if (check_button(button1_pin, button1_time)){

    // check to see if there's a message displayed. If so, simply turn it off. Otherwise, increment the screen
    if (warning_message_display.show()){
      warning_message_display.show(false);

    // increment the screen
    } else {
      if (++screen_mode > 6){ // upper bound
        screen_mode = 1;
      }
      screen_mode_begins(screen_mode, true);
    }

  }


  // driver warning signals ---------------------------------------------

  // if the signal is valid, there is probably a message that is waiting for the driver.
  if (USER_driverSignal.timeout_check() || PDM_driverDisplayLEDs.timeout_check()){

    // flash red lights to stop and shut off the car
    if (static_cast<int>(PDM_driverDisplayLEDs.value()) == 3 ||
        static_cast<int>(USER_driverSignal.value()) == 3){
      led_mode = 12;

    // flash yellow lights to stop the car
    } else if (static_cast<int>(PDM_driverDisplayLEDs.value()) == 2 ||
               static_cast<int>(USER_driverSignal.value()) == 2){
      led_mode = 11;

    // flash white lights to tell the driver to come in
    } else if (static_cast<int>(PDM_driverDisplayLEDs.value()) == 1 ||
               static_cast<int>(USER_driverSignal.value()) == 1){
      led_mode = 10;

    // go back to default LED mode
    } else if (static_cast<int>(PDM_driverDisplayLEDs.value()) == 0 ||
               static_cast<int>(USER_driverSignal.value()) == 0){
      led_mode = 1;

    }
  }


  // LED updates -------------------------------------------------------

  static unsigned int last_normal_led_time = millis();

  if (led_mode == 1){
    rpm_bar(pixels_top, M400_rpm, M400_gear);
    static float max_power_reduction_value = 25;
    engine_cut_bar(pixels_left,  M400_tcPowerReduction, max_power_reduction_value, true);
    engine_cut_bar(pixels_right, M400_tcPowerReduction, max_power_reduction_value, true);

    // this will prolly be changed
    //lockup_indicator(pixels_left, 0, M400_groundSpeedLeft, MM5_Ax, ATCCF_brakePressureF, ATCCF_brakePressureR);
    //lockup_indicator(pixels_left, 3, M400_driveSpeedLeft, MM5_Ax, ATCCF_brakePressureF, ATCCF_brakePressureR);
    //lockup_indicator(pixels_right, 0, M400_groundSpeedRight, MM5_Ax, ATCCF_brakePressureF, ATCCF_brakePressureR);
    //lockup_indicator(pixels_right, 3, M400_driveSpeedRight, MM5_Ax, ATCCF_brakePressureF, ATCCF_brakePressureR);

    // these two are unused
    pixels_left.setPixelColor(0, 0, 0, 0);
    pixels_right.setPixelColor(0, 0, 0, 0);

    pixels_top.show();
    pixels_left.show();
    pixels_right.show();

    last_normal_led_time = millis();

  } else if (led_mode == 2){
    party_bar(pixels_top, pixels_left, pixels_right);
    last_normal_led_time = millis();

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


  // test for warning LED timeout
  if ((led_mode == 10 || led_mode == 11 || led_mode == 12) &&
      (millis() > (last_normal_led_time + warning_lights_timeout_dur) )){
    led_mode = 1;
  }



  // display updates --------------------------------------------------

  if (info_screen_update_timer.isup()){
    static bool lap_timer_on = false; // used to determine if we need to run initilizations after lap screen turns off

    if (lap_timer_screen(display_left, display_right, TCGPS_laptrigger, prev_lap_times, prev_lap_times_diff, prev_lap_numbers)
        || warning_message_display.show()){
      lap_timer_on = true;

    // Mode 1 - aux and engine info
    } else if (screen_mode == 1){ // add && !user_message

        // run the initializations again
        if (lap_timer_on == true){
          lap_timer_on = false;
          screen_mode_begins(screen_mode, false); // skip startup screens
        } else {
          auxilary_info_left_screen.update();
          engine_vitals_right_screen.update();
        }

    // Mode 2 - brake temp and engine info
    } else if (screen_mode == 2){ // add && !user_message

      // run the initializations again
      if (lap_timer_on == true){
        lap_timer_on = false;
        screen_mode_begins(screen_mode, false); // skip startup screens
      } else {
        brake_temp_info_left_screen.update();
        engine_vitals_right_screen.update();
      }


    // Mode 2 - gear display and engine info 
    } else if (screen_mode == 3){

      // run the initializations again
      if (lap_timer_on == true){
        lap_timer_on = false;
        screen_mode_begins(screen_mode, false);
      } else {
        gear_display_left.update();
        engine_vitals_right_screen.update();
      }

    // Mode 3 - tc display and engine info
    } else if (screen_mode == 4){

      // run the initializations again
      if (lap_timer_on == true){
        lap_timer_on = false;
        screen_mode_begins(screen_mode, false);
      } else {
        tc_display_left.update();
        engine_vitals_right_screen.update();
      }

    // Mode 4 - lap times
    } else if (screen_mode == 5){

      // run the initializations again
      if (lap_timer_on == true){
        lap_timer_on = false;
        screen_mode_begins(screen_mode, false);
      } else {
        lap_time_display_left.update();
        lap_time_display_right.update();
      }

    }

    // Modes that simply display images are not seen here, as they are only written once in screen_mode_begins

  }


  // send can messages
  send_can1();
  send_can2();

}




// returns true if a button was pressed. Parameters are the button pin and a reference to the last time
// that the button was pressed.
bool check_button(const int &pin, unsigned long &time){

  // single press
  if ((digitalRead(pin) == LOW) && (millis() - time >= button_delay)){
    time = millis();
    return true;
  }

  return false;
}


// relocated function that's called when the screen mode changes. This runs the required initializations for
// every screen mode, except for lap trigger, which is timed by itself.
void screen_mode_begins(int &screen_mode, bool startup_screen){
  // auxilarry info screen and engine vitals screen
  if (screen_mode == 1){
    auxilary_info_left_screen.begin();
    engine_vitals_right_screen.begin();

  } else if (screen_mode == 2){
    brake_temp_info_left_screen.begin();
    engine_vitals_right_screen.begin();

  // gear screen and carry-over of engine vitals
  } else if (screen_mode == 3){
    gear_display_left.begin(startup_screen);
    engine_vitals_right_screen.begin();


  } else if (screen_mode == 4){
    tc_display_left.begin(startup_screen);
    engine_vitals_right_screen.begin();

  } else if (screen_mode == 5) {
    lap_time_display_left.begin(startup_screen);
    lap_time_display_right.begin(startup_screen);

  } else if (screen_mode == 6) {
    // display titans
    display_left.writeRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, (uint16_t*)titans1);
    display_right.writeRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, (uint16_t*)titans2);

  } else if (screen_mode == 7) {
    // display titans
    display_left.writeRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, (uint16_t*)titans1);
    display_right.writeRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, (uint16_t*)titans2);
  }
}


void set_mailboxes(){

  // to view mailbox status, you can use the member function mailboxStatus(). Don't put it in a fast loop, though,
  // because you may actually affect how the chips moves things around

  cbus1.setMaxMB(64);
  cbus1.setMB(MB4,RX,STD);  // first 24 mailboxes as rx, 8 rx extended. this is pretty overkill, but hey, here they are
  cbus1.setMB(MB5,RX,STD);  // this leaves the last 32 mailboxes for tx
  cbus1.setMB(MB6,RX,STD);
  cbus1.setMB(MB7,RX,STD);
  cbus1.setMB(MB8,RX,STD);
  cbus1.setMB(MB9,RX,STD);
  cbus1.setMB(MB10,RX,STD);
  cbus1.setMB(MB11,RX,STD);
  cbus1.setMB(MB12,RX,STD);
  cbus1.setMB(MB13,RX,STD);
  cbus1.setMB(MB14,RX,STD);
  cbus1.setMB(MB15,RX,STD);
  cbus1.setMB(MB16,RX,STD);
  cbus1.setMB(MB17,RX,STD);
  cbus1.setMB(MB18,RX,STD);
  cbus1.setMB(MB19,RX,STD);
  cbus1.setMB(MB20,RX,STD);
  cbus1.setMB(MB21,RX,STD);
  cbus1.setMB(MB22,RX,STD);
  cbus1.setMB(MB23,RX,STD);
  cbus1.setMB(MB24,RX,EXT);
  cbus1.setMB(MB25,RX,EXT);
  cbus1.setMB(MB26,RX,EXT);
  cbus1.setMB(MB27,RX,EXT);
  cbus1.setMB(MB28,RX,EXT);
  cbus1.setMB(MB29,RX,EXT);
  cbus1.setMB(MB30,RX,EXT);
  cbus1.setMB(MB31,RX,EXT);

  cbus2.setMaxMB(64);
  cbus2.setMB(MB4,RX,STD);
  cbus2.setMB(MB5,RX,STD);
  cbus2.setMB(MB6,RX,STD);
  cbus2.setMB(MB7,RX,STD);
  cbus2.setMB(MB8,RX,STD);
  cbus2.setMB(MB9,RX,STD);
  cbus2.setMB(MB10,RX,STD);
  cbus2.setMB(MB11,RX,STD);
  cbus2.setMB(MB12,RX,STD);
  cbus2.setMB(MB13,RX,STD);
  cbus2.setMB(MB14,RX,STD);
  cbus2.setMB(MB15,RX,STD);
  cbus2.setMB(MB16,RX,STD);
  cbus2.setMB(MB17,RX,STD);
  cbus2.setMB(MB18,RX,STD);
  cbus2.setMB(MB19,RX,STD);
  cbus2.setMB(MB20,RX,STD);
  cbus2.setMB(MB21,RX,STD);
  cbus2.setMB(MB22,RX,STD);
  cbus2.setMB(MB23,RX,STD);
  cbus2.setMB(MB24,RX,EXT);
  cbus2.setMB(MB25,RX,EXT);
  cbus2.setMB(MB26,RX,EXT);
  cbus2.setMB(MB27,RX,EXT);
  cbus2.setMB(MB28,RX,EXT);
  cbus2.setMB(MB29,RX,EXT);
  cbus2.setMB(MB30,RX,EXT);
  cbus2.setMB(MB31,RX,EXT);
}


// temporarily used to determine what gear we are in for the driver display shift lights
int determine_gear(StateSignal &rpm, StateSignal &speed, StateSignal &gear){

  // set the gear signal to non-valid
  gear.set_validity(false);

  // if rpm is decently high and the car is moving, we'll say that we're at least in gear 1
  if ((rpm.value() >= 1500) && (speed.value() >= 5)){
    gear.set_secondary_value(1);
    return 1;
  } else {
    gear.set_secondary_value(0);
    return 0;
  }

}


// there is a bug in this function somewhere. it turns to a party when driving sometimes
bool determine_if_party_time(StateSignal &rpm, int &led_mode){
  static unsigned long party_time_timeout = 300000; // 300k millisconds = 5minutes
  static unsigned long normal_until_time = millis() + party_time_timeout;
  static bool party_time = false;

  // if the rpm is greater than 0, really, then they were at least cranking or somthing
  if (rpm.value() > 10){
    normal_until_time = millis() + party_time_timeout;
    party_time = false;

  } else if (millis() > normal_until_time){
    if (party_time == false){
      party_time = true;
      led_mode = 2;
    }
  }

  return party_time;

}



