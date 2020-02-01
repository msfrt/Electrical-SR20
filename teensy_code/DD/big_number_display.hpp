#ifndef BIG_BIG_NUMBERS_ON_THE_SMALL_SMALL_SCREEN
#define BIG_BIG_NUMBERS_ON_THE_SMALL_SMALL_SCREEN

struct NumberDisplay{

  String label;
  ILI9341_t3n &screen;
  StateSignal &signal;
  int last_val = 9999;
  int current_val = 0;
  unsigned long startup_until;
  unsigned long startup_message_millis = 2000;

  // initializer
  NumberDisplay(ILI9341_t3n &display, StateSignal &sig, String lab) : label(lab), screen(display), signal(sig) {};

  // begin the display and write startup message
  void begin();

  // check if the display needs to be updated, and update if necessary
  bool update(bool override = false);

};

void NumberDisplay::begin(){
  // clear the screen and draw two borders
  screen.fillScreen(ILI9341_BLACK);
  screen.drawFastHLine(0,                  0, DISPLAY_WIDTH, ILI9341_GREEN);
  screen.drawFastHLine(0, DISPLAY_HEIGHT - 1, DISPLAY_WIDTH, ILI9341_GREEN);

  this->startup_until = millis() + startup_message_millis;

  screen.setFont(LiberationMono_72_Bold_Italic); // each char is 60 px wide
  screen.setCursor((DISPLAY_WIDTH - (this->label.length() * 60)) / 2 - 5, (DISPLAY_HEIGHT - 72) / 2);
  screen.print(this->label);
}


bool NumberDisplay::update(bool override){

  this->current_val = static_cast<int>(this->signal.value());

  if (this->) {
    /* code */
  }
}

#endif
