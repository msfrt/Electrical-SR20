
#include <Adafruit_NeoPixel.h>

const int ledSig = 2;
const int adc1_cs = 3;
const int brakelightSig = 4;
const int dataSig = 5;
const int wpSig = 6;
const int rightFanSig = 7;
const int leftFanSig = 8;
const int eeprom_cs = 9;
const int adc2_cs = 10;
const int mosi = 11;
const int miso = 12;
const int clk = 13;
const int drsSig = 20;
const int boardTemp = 21;

const int ledBrightness = 100;

Adafruit_NeoPixel led = Adafruit_NeoPixel(1, ledSig, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:

  pinMode(ledSig, OUTPUT);
  pinMode(adc1_cs, OUTPUT);
  pinMode(brakelightSig, OUTPUT);
  pinMode(dataSig, OUTPUT);
  pinMode(wpSig, OUTPUT);
  pinMode(rightFanSig, OUTPUT);
  pinMode(leftFanSig, OUTPUT);
  pinMode(eeprom_cs, OUTPUT);
  pinMode(adc2_cs, OUTPUT);
  pinMode(mosi, OUTPUT);
  pinMode(miso, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(drsSig, OUTPUT);
  pinMode(boardTemp, OUTPUT);

  led.setBrightness(ledBrightness);
  led.begin();
  led.show();
  led.setPixelColor(1, 0, 30, 0);

}

void loop() {
  // put your main code here, to run repeatedly:

}
