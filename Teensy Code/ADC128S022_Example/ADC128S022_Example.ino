#include <SPI.h>  // include the SPI library:

const int slaveSelectPin = 9;

uint16_t a0 = 0; 
uint16_t a1 = 0;
uint16_t r0 = 0;
uint16_t r1 = 7;

void setup() {
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  Serial.begin(9600);
  SPI.begin(); 
}

void loop() {
  delay(100);
  ADCRead();
}

int ADCRead() {

  SPI.beginTransaction(SPISettings(7000000, MSBFIRST, SPI_MODE0));
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin,LOW);
  //  send in the address and value via SPI:
  a0 = SPI.transfer16(r0 << 11);
 // a1 = SPI.transfer16(r1 >> 3);
  r1 = r0 << 11;
  Serial.print("A0: ");
  Serial.println(a0);
 // Serial.print("A1: ");
  //Serial.println(a1);
  for (int i=0; i<16; i++){
    if (i == 16){
      Serial.println(bitRead(r1, i));
    }
    else{
      Serial.print(bitRead(r1, i));
    }
  }

  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin,HIGH); 
  SPI.endTransaction();
}
