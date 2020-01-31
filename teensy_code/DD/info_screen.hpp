#ifndef INFO_SCREEN_HPP
#define INFO_SCREEN_HPP

#include "arduino.h"
#include <StateCAN.h>
#include "ILI9341_t3n.h"
#include "Adafruit_GFX.h"
#include "font_LiberationMonoBold.h"
#include "font_LiberationMonoBoldItalic.h"

struct InfoScreen {

  ILI9341_t3n &screen;

  int x_buff_px = 1;
  int y_buff_px = 12;
  int char_width_px = 35; // pixel width of one character in the font

  // default color definitions
  uint16_t line_color = ILI9341_GREEN;
  uint16_t text_background_default = ILI9341_BLACK;
  uint16_t text_background_warning = ILI9341_BLACK;
  uint16_t text_color_default = ILI9341_WHITE;
  uint16_t text_color_warning = ILI9341_RED;

  // these should be set externally if you want the color of the number to be the warning color
  bool sig1_warning = false;
  bool sig2_warning = false;
  bool sig3_warning = false;
  bool sig4_warning = false;

  // the signals to display
  StateSignal &sig1;
  StateSignal &sig2;
  StateSignal &sig3;
  StateSignal &sig4;

  // the number of digits (including '.') that there is space to display
  int num_digits_sig1 = 4;
  int num_digits_sig2 = 4;
  int num_digits_sig3 = 4;
  int num_digits_sig4 = 4;

  // the number of digits past the decimal point to display
  int dec_prec_sig1 = 0;
  int dec_prec_sig2 = 0;
  int dec_prec_sig3 = 0;
  int dec_prec_sig4 = 0;

  String label1;
  String label2;
  String label3;
  String label4;

  float last_val_sig1;
  float last_val_sig2;
  float last_val_sig3;
  float last_val_sig4;

  InfoScreen() = delete;
  InfoScreen(ILI9341_t3n &scr, StateSignal &s1, StateSignal &s2, StateSignal &s3, StateSignal &s4) :
  screen(scr), sig1(s1), sig2(s2), sig3(s3), sig4(s4) {};

  // set variables for all of the signals. This should be done sometime after you initialize the info screen.
  void set_labels(String lab1, String lab2, String lab3, String lab4); // lables including ':'
  void set_numdigits(int d1, int d2, int d3, int d4); // number of digits alotted for the number-portion
  void set_precisions(int p1, int p2, int p3, int p4); // decimal precision. When in doubt, add more decimals

  // prints the divider lines
  void print_lines();

  // prints the signal labels
  void print_labels();

  // update signal values individually (you probably don't wanna call this one)
  bool update_sig1(bool override = false);
  bool update_sig2(bool override = false);
  bool update_sig3(bool override = false);
  bool update_sig4(bool override = false);

  // update the dignals if necessary
  bool update_signals();

};


void InfoScreen::set_labels(String lab1, String lab2, String lab3, String lab4){
  this->label1 = lab1;
  this->label2 = lab2;
  this->label3 = lab3;
  this->label4 = lab4;
}

void InfoScreen::set_numdigits(int d1, int d2, int d3, int d4){
  this->num_digits_sig1 = d1;
  this->num_digits_sig2 = d2;
  this->num_digits_sig3 = d3;
  this->num_digits_sig4 = d4;
}

void InfoScreen::set_precisions(int p1, int p2, int p3, int p4){
  this->dec_prec_sig1 = p1;
  this->dec_prec_sig2 = p2;
  this->dec_prec_sig3 = p2;
  this->dec_prec_sig4 = p2;
}


// prints the seperator lines between signals
void InfoScreen::print_lines(){
  screen.drawFastHLine(0, (DISPLAY_HEIGHT / 4) * 0,     DISPLAY_WIDTH, this->line_color);
  screen.drawFastHLine(0, (DISPLAY_HEIGHT / 4) * 1,     DISPLAY_WIDTH, this->line_color);
  screen.drawFastHLine(0, (DISPLAY_HEIGHT / 4) * 2,     DISPLAY_WIDTH, this->line_color);
  screen.drawFastHLine(0, (DISPLAY_HEIGHT / 4) * 3,     DISPLAY_WIDTH, this->line_color);
  screen.drawFastHLine(0, (DISPLAY_HEIGHT / 4) * 4 - 1, DISPLAY_WIDTH, this->line_color);
}

void InfoScreen::print_labels(){
  screen.setTextColor(this->text_color_default, this->text_background_default);
  screen.setTextWrap(false);
  screen.setFont(LiberationMono_40_Bold);

  // signal 1
  screen.setCursor(this->x_buff_px, this->y_buff_px);
  screen.println(this->label1);
  // signal 2
  screen.setCursor(this->x_buff_px, (DISPLAY_HEIGHT / 4) * 1 + this->y_buff_px);
  screen.println(this->label2);
  // signal 3
  screen.setCursor(this->x_buff_px, (DISPLAY_HEIGHT / 4) * 2 + this->y_buff_px);
  screen.println(this->label3);
  // signal 4
  screen.setCursor(this->x_buff_px, (DISPLAY_HEIGHT / 4) * 3 + this->y_buff_px);
  screen.println(this->label4);
}


// runs each update function and returns true if at l
bool InfoScreen::update_signals(){
  static bool b1;
  static bool b2;
  static bool b3;
  static bool b4;

  screen.setTextColor(this->text_color_default);
  screen.setTextWrap(false);

  b1 = this->update_sig1();
  b2 = this->update_sig2();
  b3 = this->update_sig3();
  b4 = this->update_sig4();

  if (b1 || b2 || b3 || b4)
    return true;
  else
    return false;
}


bool InfoScreen::update_sig1(bool override){
  // ensure that we only write to the screen if the value has changed
  if (override || (this->last_val_sig1 != sig1.value())){
    this->last_val_sig1 = sig1.value();

    int x_loc = DISPLAY_WIDTH - (this->num_digits_sig1 * this->char_width_px);
    int y_loc = this->y_buff_px;

    // move to the place where we've alotted the space to update the
    screen.setCursor(x_loc, y_loc);

    // determine the color of the text to be printed
    if (!this->sig1_warning){
      screen.setTextColor(this->text_color_default); // all gucci
    } else {
      screen.setTextColor(this->text_color_warning); // there is a warning present
    }

    // overwrite the old pixels on the screen with the background color
    if (!this->sig1_warning){
      screen.fillRect(x_loc, 1, DISPLAY_WIDTH - x_loc, (DISPLAY_HEIGHT - 4) / 4, this->text_background_default);
    } else {
      screen.fillRect(x_loc, 1, DISPLAY_WIDTH - x_loc, (DISPLAY_HEIGHT - 4) / 4, this->text_background_warning);
    }

    // write the text
    screen.print(this->last_val_sig1, this->dec_prec_sig1);

    return true;
  }
  return false;
}


bool InfoScreen::update_sig2(bool override){
  if (override || (this->last_val_sig2 != sig2.value())){
    this->last_val_sig2 = sig2.value();

    int x_loc = DISPLAY_WIDTH - (this->num_digits_sig2 * this->char_width_px);
    int y_loc = (DISPLAY_HEIGHT / 4) * 1 + this->y_buff_px;

    screen.setCursor(x_loc, y_loc);

    if (!this->sig2_warning){
      screen.setTextColor(this->text_color_default);
    } else {
      screen.setTextColor(this->text_color_warning);
    }

    if (!this->sig2_warning){
      screen.fillRect(x_loc, (DISPLAY_HEIGHT / 4) + 1, DISPLAY_WIDTH - x_loc,
                     (DISPLAY_HEIGHT - 4) / 4, this->text_background_default);
    } else {
      screen.fillRect(x_loc, (DISPLAY_HEIGHT / 4) + 1, DISPLAY_WIDTH - x_loc,
                     (DISPLAY_HEIGHT - 4) / 4, this->text_background_warning);
    }

    screen.print(this->last_val_sig2, this->dec_prec_sig2);

    return true;
  }
  return false;
}


bool InfoScreen::update_sig3(bool override){
  if (override || (this->last_val_sig3 != sig3.value())){
    this->last_val_sig3 = sig3.value();

    int x_loc = DISPLAY_WIDTH - (this->num_digits_sig3 * this->char_width_px);
    int y_loc = (DISPLAY_HEIGHT / 4) * 2 + this->y_buff_px;

    screen.setCursor(x_loc, y_loc);

    if (!this->sig3_warning){
      screen.setTextColor(this->text_color_default);
    } else {
      screen.setTextColor(this->text_color_warning);
    }

    if (!this->sig3_warning){
      screen.fillRect(x_loc, (DISPLAY_HEIGHT / 4) * 2 + 1, DISPLAY_WIDTH - x_loc,
                     (DISPLAY_HEIGHT - 4) / 4, this->text_background_default);
    } else {
      screen.fillRect(x_loc, (DISPLAY_HEIGHT / 4) * 2 + 1, DISPLAY_WIDTH - x_loc,
                     (DISPLAY_HEIGHT - 4) / 4, this->text_background_warning);
    }

    screen.print(this->last_val_sig3, this->dec_prec_sig3);

    return true;
  }
  return false;
}



bool InfoScreen::update_sig4(bool override){
  if (override || (this->last_val_sig4 != sig4.value())){
    this->last_val_sig4 = sig4.value();

    int x_loc = DISPLAY_WIDTH - (this->num_digits_sig4 * this->char_width_px);
    int y_loc = (DISPLAY_HEIGHT / 4) * 3 + this->y_buff_px;

    screen.setCursor(x_loc, y_loc);

    if (!this->sig4_warning){
      screen.setTextColor(this->text_color_default);
    } else {
      screen.setTextColor(this->text_color_warning);
    }

    if (!this->sig4_warning){
      screen.fillRect(x_loc, (DISPLAY_HEIGHT / 4) * 3 + 1, DISPLAY_WIDTH - x_loc,
                     (DISPLAY_HEIGHT - 4) / 4 - 1, this->text_background_default);
    } else {
      screen.fillRect(x_loc, (DISPLAY_HEIGHT / 4) * 3 + 1, DISPLAY_WIDTH - x_loc,
                     (DISPLAY_HEIGHT - 4) / 4 - 1, this->text_background_warning);
    }

    screen.print(this->last_val_sig4, this->dec_prec_sig4);

    return true;
  }
  return false;
}

#endif
