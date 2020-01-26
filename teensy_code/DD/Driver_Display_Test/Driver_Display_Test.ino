#include <StateCAN.h>
#include <FlexCAN_T4.h>
#include <EasyTimer.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_SPITFT.h>
//#include <Adafruit_SPITFT_Macros.h>
//#include <gfxfont.h>
#include <Adafruit_ST7735.h>
//#include <Adafruit_ST7789.h>
//#include <Adafruit_ST77xx.h>

//NeoPixle Setup
Adafruit_NeoPixel ledTop (16, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledLeft (4, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledRight (4, 3, NEO_GRB + NEO_KHZ800);

EasyTimer ledTimer(1);

//CAN Setup
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> cbus1;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> cbus2;

StateSignal testCAN1(8, true, 1, 0, -127, 127, 0);
StateSignal testCAN2(8, true, 1, 0, -127, 127, 0);

static CAN_message_t msg;

int ledCount = 0;

void setup() {
  ledTop.begin();
  ledLeft.begin();
  ledRight.begin();

}

void loop() {
  ledTest();
  sendCAN1();
  sendCAN2();

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

void sendCAN1(){
  msg.id = 410;
  msg.len = 8;

  testCAN1 = B01101010;

  msg.buf[0] = testCAN1.can_value();
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  cbus1.write(msg);
}

void sendCAN2(){
  msg.id = 410;
  msg.len = 8;

  testCAN2 = B10010101;

  msg.buf[0] = testCAN2.can_value();
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  cbus2.write(msg);
}
