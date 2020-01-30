
#include <Adafruit_NeoPixel.h>

// include the SPI library:
#include <SPI.h>

const int ledSig = 2;
const int adc1_cs = 3;
const int brakelightSig = 4;
const int dataSig = 5;
const int wpSig = 6;
const int rightFanSig = 7;
const int leftFanSig = 8;
const int eepromCS = 9;
const int adc2_cs = 10;
const int mosi = 11;
const int miso = 12;
const int clk = 13;
const int drsSig = 20;
const int boardTemp = 21;

const int ledBrightness = 100;

unsigned long timer = 0;

// switch to only write to the EEPROM one time
bool sent = false;

// set the address to be read/write to
uint16_t address = 4;

// set the 1 byte data to be written
uint8_t data = 100;

// number of bytes in a page (constant for the 25LC128)
const uint8_t pageSize = 64;

// number of pages in the 25LC128
const uint8_t numPages = 256;

Adafruit_NeoPixel led = Adafruit_NeoPixel(1, ledSig, NEO_GRB + NEO_KHZ800);

int buff[8];

void setup() {
  // put your setup code here, to run once:

  pinMode(ledSig, OUTPUT);
  pinMode(adc1_cs, OUTPUT);
  pinMode(brakelightSig, OUTPUT);
  pinMode(dataSig, OUTPUT);
  pinMode(wpSig, OUTPUT);
  pinMode(rightFanSig, OUTPUT);
  pinMode(leftFanSig, OUTPUT);
  pinMode(eepromCS, OUTPUT);
  pinMode(adc2_cs, OUTPUT);
  pinMode(mosi, OUTPUT);
  pinMode(miso, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(drsSig, OUTPUT);
  pinMode(boardTemp, OUTPUT);

  led.setBrightness(ledBrightness);
  led.begin();
  led.show();

    // set the chip select pin high
  digitalWrite(adc1_cs, HIGH);
  digitalWrite(adc2_cs, HIGH);

  // write the CS pin high to disable the eeprom chip
  digitalWrite(eepromCS, HIGH);

  digitalWrite(rightFanSig, HIGH);
  digitalWrite(leftFanSig, HIGH);
  digitalWrite(wpSig, HIGH);
  //digitalWrite(dataSig, HIGH);

  // initialize serial communication
  Serial.begin(112500);

  delay(5000);

  // initialize SPI:
  SPI.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

//  led.setPixelColor(0, 0, 30, 0);
//  led.show();
//  digitalWrite(drsSig, HIGH);
//  delay(200);
//  led.setPixelColor(0, 0, 0, 0);
//  led.show();
//  digitalWrite(drsSig, LOW);
//  delay(200);

  // call anaRead() every 5uS which is 200000 Hz or 200000 Sps, the max rate for the ADC128S022
  if ((micros() - timer) > 100000){

//    for(auto i = 1; i <= 8; i++){
//      buff[i-1] = anaRead(adc1_cs,i);
//      //delay(20);
//      
//      }
  anaRead(adc2_cs,1);

//  for(auto i = 0; i < 8; i++){
//    
//    float voltage;
//    voltage = (float(buff[i]) / 4095) * 5;
//    Serial.print("Input ");
//    Serial.print(i);
//    Serial.print(": ");
//    Serial.print(buff[i]);
//    Serial.print("  which is: ");
//    Serial.print(voltage, 4);
//    Serial.println(" Volts");
//  }
 

//    anaRead(adc2_cs,1);
//    anaRead(adc2_cs,2);
//    anaRead(adc2_cs,3);
//    anaRead(adc2_cs,4);
//    anaRead(adc2_cs,5);
//    anaRead(adc2_cs,6);
//    anaRead(adc2_cs,7);
//    anaRead(adc2_cs,8);
    timer = micros();

    Serial.println("-------------------");
  }

//
//  // slow down the read rate for testing
//  delay(1000);
//
//     // only write the data to the EEPROM module once
//   if(!sent){
//
//   // write the data
//   eepromWriteByte(eepromCS, address, data);
//
//   // toggle sent to prevent the eeprom from being written to
//   sent = true;
//  }
//
//  // repeatly read the data that was written
//  else{
//   Serial.println(eepromRead(eepromCS, address));
//  }

}

int anaRead(int adcCS, uint16_t channelNo) {

  //--------------------------------------------------------------------------------------------------------------------
  // anaRead takes in a ADC's chip select pin number and the ADC channel number to be read, reads the value of the
  // input, and then returns the read value which is a 12 bit number (0 - 4095)  porportional to the voltage at the
  // selected input of the ADC (0V - 5V)
  //
  // Inputs:
  //     adcCS: pin number on the teensy which is connected to the ADC's chip select pin
  //     channelNo: input number of the ADC which will be read (0-7)
  //
  // Returns:
  //     readValue: the value returned by the ADC which will be a 12 bit number between 0 and 4095 which corresponds to
  //     the voltage at the selected input of the ADC (0V - 5V)
  //--------------------------------------------------------------------------------------------------------------------


  uint16_t readValue = 0;
  uint16_t readValue1 = 0;
  uint16_t readValue2 = 0;
  uint16_t readValue3 = 0;
  uint16_t readValue4 = 0;
  uint16_t readValue5 = 0;
  uint16_t readValue6 = 0;
  uint16_t readValue7 = 0;

  // Start the SPI communication
  // SPISettings(clk frequency, bit order, SPI Mode (google arduino SPI modes for details))
  SPI.beginTransaction(SPISettings(800000, MSBFIRST, SPI_MODE0));

  // take the CS pin low to enable the chip:
  digitalWrite(adcCS,LOW);

  //--------------------------------------------------------------------------------------------------------------------
  // In order to tell the ADC which input to read you must send it a binary value corresponding to the channel.
  // channelNo should be a value between 0 and 7 corresponding to the input you want to read. The binary representation
  // of this value must be bit shifted so that it is in the correct position during the SPI communication sequence.
  // The SPI sequence lasts 16 clock cycles so both the sent value and recieved value are 16 bits, one for each clock cycle.
  //
  // Example: If you want to read the 2nd channel of the ADC the function call would be adaRead(adcSC, 2). 2 in a 16 bit
  // binary number is "0000000000000010" (MSB first). The the position of the channelNo must be in the following position
  // "00XXX00000000000", (X represents the position) as defined by the ADC's datasheet. To achieve this, the value
  // inputted into this function is bit shifted by 11 to the left. The SPI.transfer16( )sends this value and returns
  // the value sent back by the ADC.
  //--------------------------------------------------------------------------------------------------------------------


  // send the channelNo for the ADC to read
  readValue = SPI.transfer16(channelNo << 11);
  readValue1 = SPI.transfer16(channelNo+1 << 11);
  readValue2 = SPI.transfer16(channelNo+2 << 11);
  readValue3 = SPI.transfer16(channelNo+3 << 11);
  readValue4 = SPI.transfer16(channelNo+4 << 11);
  readValue5 = SPI.transfer16(channelNo+5 << 11);
  readValue6 = SPI.transfer16(channelNo+6 << 11);
  readValue7 = SPI.transfer16(channelNo+7 << 11);

  Serial.println(readValue);
  Serial.println(readValue1);
  Serial.println(readValue2);
  Serial.println(readValue3);
  Serial.println(readValue4);
  Serial.println(readValue5);
  Serial.println(readValue6);
  Serial.println(readValue7);

  //--------------------------------------------------------------------------------------------------------------------
  // the following is for debugging the bitshifting process
  // // bit shift the inputted channelNo
  // uint16_t channelNoShifted = channelNo << 11;
  //
  // // read out the binary representation of the channelNo interger
  // for (int i=0; i<16; i++){
  //   if (i == 16){
  //     Serial.println(bitRead(channelNoShifted, i));
  //   }
  //   else{
  //     Serial.print(bitRead(channelNoShifted, i));
  //   }
  // }
  //--------------------------------------------------------------------------------------------------------------------
  
  // take the CS pin high to de-select the chip:
  digitalWrite(adcCS,HIGH);
  
  //--------------------------------------------------------------------------------------------------------------------
  // the following is for debugging the returned value from the ADC
//  float voltage = (float(readValue) / 4095) * 5;
//  Serial.print("Input ");
//  Serial.print(channelNo);
//  Serial.print(": ");
//  Serial.print(readValue);
//  Serial.print("  which is: ");
//  Serial.print(voltage, 4);
//  Serial.println(" Volts");
  //--------------------------------------------------------------------------------------------------------------------

  // end the SPI communication
  SPI.endTransaction();

  // return value read by ADC
  return readValue;
}

void eepromWriteByte(int eepromCS, uint16_t address, uint8_t data){
  //--------------------------------------------------------------------------------------------------------------------
  // This function is an example of the process of writing a byte of data to the EEPROM module. In the datasheet there
  // are two methods for writing data to the EEPROM: a single byte or multiple bytes that can be up to a page long(read
  // the datasheet). This is for the method of writing a single byte. A function still needs to be developed in order to
  // write a page, this can be used to write up to 64 bytes. Page size is specific to each EEPROM module.
  //
  // Inputs:
  //   eepromCS: pin number on the teensy which is connected to the EEPROMS's chip select pin
  //   address: address for the data to be written to (should be between 0 and 16384 for the 25LC128)
  //   data: the data to be written (8 bits)
  //
  // Returns:
  //   none
  //--------------------------------------------------------------------------------------------------------------------


  // Start the SPI communication
  // SPISettings(clk frequency, bit order, SPI Mode (google arduino SPI modes for details))
  SPI.beginTransaction(SPISettings(3200000, MSBFIRST, SPI_MODE0));


  //--------------------------------------------------------------------------------------------------------------------
  // The following three commands are needed to write data to the EEPROM module. As specified in the datasheet
  // (which you should read), you must first set the write enable latch in the EEPROM module. This is done by
  // sending the "WREN" instruction which is "00000110". After you send the "WREN" command to complete the setting of the
  // write enable latch.
  //--------------------------------------------------------------------------------------------------------------------


  // take the CS pin low to enable the chip
  digitalWrite(eepromCS, LOW);
  // send the "WREN" command
  SPI.transfer(0b00000110);
  // take the CS high to disable the chip and set the write enable latch
  digitalWrite(eepromCS, HIGH);



  // The following 5 commands are for writing the data to the specified address

  // take the CS pin low to enable the chip
  digitalWrite(eepromCS, LOW);
  // send the "WRITE" command
  SPI.transfer(0b00000010);
  // send the address for which the data will be written to
  SPI.transfer16(address);
  // send the data
  SPI.transfer(data);
  // take the CS high to disable the chip (the data wont be written to the module until this is completed)
  digitalWrite(eepromCS, HIGH);

  // end the SPI communication
  SPI.endTransaction();
}

int eepromRead(int eepromCS, uint16_t address){

  //--------------------------------------------------------------------------------------------------------------------
  // This function is for reading data that is stored in the EEPROM module. The function is designed to read a single
  // address of memory. As specified by the datasheet, the 25LC128 is allows you to read data sequentially by keeping the
  // CS pin low. After calling SPI.transfer() to get the first byte of data, you can continue to call the SPI.transfer()
  // and the EEPROM will return the byte of data in the next address automatically. Again, read the datasheet.
  //
  // Inputs:
  //   eepromCS: pin number on the teensy which is connected to the EEPROMS's chip select pin
  //   address: address for the data to be written to (should be between 0 and 16384 for the 25LC128)
  //
  // Returns:
  //   data: the byte of data that was returned by the EEPROM module
  //--------------------------------------------------------------------------------------------------------------------


  // initialize the variable to store the recieved data
  uint8_t data;

  // Start the SPI communication
  // SPISettings(clk frequency, bit order, SPI Mode (google arduino SPI modes for details))
  SPI.beginTransaction(SPISettings(3200000, MSBFIRST, SPI_MODE0));

  // take the CS pin low to enable the chip
  digitalWrite(eepromCS, LOW);

  // send the "READ" command
  SPI.transfer(0b00000011);
  // send the address for which the data will be written to
  SPI.transfer16(address);
  // send the data (the 0 is needed so you can call SPI.transfer() so it will return the data sent by the EEPROM)
  data = SPI.transfer(0);

  // take the CS high to disable the chip (the data wont be written to the module until this is completed)
  digitalWrite(eepromCS, HIGH);

  // end the SPI communication
  SPI.endTransaction();

  return data;
}
