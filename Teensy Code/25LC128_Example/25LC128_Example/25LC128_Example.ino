// include the SPI library:
#include <SPI.h>

// define the chip select pin for the ADC
const int eepromCS = 9;

bool sent = false;
uint16_t address = 4;
uint8_t data = 101;

void setup() {
  // set the chip select pin as an output:
  pinMode(eepromCS, OUTPUT);

  // write the CS pin high to disable the eeprom chip
  digitalWrite(eepromCS, HIGH);

  // initialize serial communication
  Serial.begin(9600);

  // initialize SPI:
  SPI.begin();

  delay(10000);
}

void loop() {

  // slow down the read rate for testing
  delay(1000);

 if(!sent){
   eepromWrite(eepromCS, address, data);
 }
 else{
   Serial.println(eepromRead(eepromCS, address));
 }

}

void eepromWrite(int eepromCS, uint16_t address, uint8_t data){

  SPI.beginTransaction(SPISettings(3200000, MSBFIRST, SPI_MODE0));

  digitalWrite(eepromCS, LOW);

  SPI.transfer(0b00000010);
  SPI.transfer16(address);
  SPI.transfer(data);

  digitalWrite(eepromCS, HIGH);


}

int eepromRead(int eepromCS, uint16_t address){

  uint8_t data;

  SPI.beginTransaction(SPISettings(3200000, MSBFIRST, SPI_MODE0));

  digitalWrite(eepromCS, LOW);

  SPI.transfer(0b00000011);
  SPI.transfer16(address);
  data = SPI.transfer(0);

  digitalWrite(eepromCS, HIGH);

  SPI.endTransaction();

  return data;
}
