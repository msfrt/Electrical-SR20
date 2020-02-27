#ifndef HOT_HOT_LAPS_INCOMING
#define HOT_HOT_LAPS_INCOMING

#include <Adafruit_NeoPixel.h>
#include <EasyTimer.h>

// trigger this with the on boolean set to true when a lap trigger is recieved. Returns true if the lights
// should be on, false otherwise. Will continue to update the light as long as the it's within the lightup duration
bool laptrigger_sucess_pixel(Adafruit_NeoPixel &pixels, const bool &turn_on = false){
  static unsigned int lightup_duration = 750; // change me!
  static unsigned int lightup_until = 0;
  static bool light_on = false;
  static EasyTimer flash_timer(50);

  // if the function was called to be on, update the flash time
  if (turn_on){
    lightup_until = millis() + lightup_duration;
  }

  // take care of the flashing if need be
  if (millis() <= lightup_until){
    if (flash_timer.isup()){
      if (light_on){
        light_on = false;
        for (int i = 0; i < pixels.numPixels(); i++){
          pixels.setPixelColor(i, 0, 0, 0);
        }

      } else {
        light_on = true;
        for (int i = 0; i < pixels.numPixels(); i++){
          pixels.setPixelColor(i, 255, 255, 255);
        }
      }

      pixels.show();
    } // end timer
    return true;

  } else {
    return false;
  }

}

#endif
