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


// software engineer: ron hodge
bool laptrigger_read(){
  xbee.readPacket();

  // if there is a message available to read
  if(xbee.getResponse().isAvailable()){

    // if the ID is correct
    if(xbee.getResponse().getApiId() == ZB_EXPLICIT_RX_RESPONSE){

      // fill the response object
      xbee.getResponse().getZBExplicitRxResponse(xbee_rx);

      // create an array to store the incoming data
      char incoming_msg[xbee_rx.getDataLength()+1] = {0};

      // read each byte and save it into the msg array
      for(int i=0; i < (xbee_rx.getDataLength() - 1); i++){
        incoming_msg[i] = xbee_rx.getData(i);
      }

      //Serial.println(incoming_msg);

      // if they were the same (if there is 0 difference)
      if (strcmp(incoming_msg, laptrigger_rx_key) == 0){
        laptrigger_sucess_pixel(pixel, true); // trigger the initial LED signal
        return true;
      }
    }
  }

  return false;
}

#endif
