#include <StateCAN.h>
#include <ReadADC.h>
#include <EasyTimer.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_SPITFT.h>
//#include <Adafruit_SPITFT_Macros.h>
//#include <gfxfont.h>
#include <Adafruit_ST7735.h>
//#include <Adafruit_ST7789.h>
//#include <Adafruit_ST77xx.h>

Adafruit_NeoPixel ledTop (16, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledLeft (4, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledRight (4, 3, NEO_GRB + NEO_KHZ800);

EasyTimer ledTimer(1);

int ledCount = 0;

void setup() {
  ledTop.begin();
  ledLeft.begin();
  ledRight.begin();

}

void loop() {
  ledTest();

}

void ledTest(){
  ledTop.clear();
  ledLeft.clear();
  ledRight.clear();

  if(ledTimer.isup()){
    ledTop.setPixelColor(ledCount, 0, 150 ,0);
    ledLeft.setPixelColor((ledCount % 4), 0, 150 ,0);
    ledRight.setPixelColor((ledCount % 4), 0, 150 ,0);
  }

  if(ledCount < 15){
    ledCount++;
  }
  else{
    ledCount = 0;
  }

  ledTop.show();
  ledLeft.show();
  ledRight.show();
}
