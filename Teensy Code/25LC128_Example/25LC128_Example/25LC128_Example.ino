//----------------------------------------------------------------------------------------------------------------------
//  Written by:     Nicholas Kopec
//  Purpose:        Example functions for communication with the 25LC128 EEPROM module
//  Description:    25LC128_Example.ino
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//  TODOS:
//    - add functions for reading and writing pages/multiple bytes
//    - needs to be changed into a header file format before being used in other code
//----------------------------------------------------------------------------------------------------------------------

// include the SPI library:
#include <SPI.h>

// define the chip select pin for the ADC
const int eepromCS = 10;

//----------------------------------------------------------------------------------------------------------------------
// EEPROM memory has a limitied number of write cycles per address before it physically degrades. Therefore you must
// therefore you must limit the amount of times that you write to each address. It is good practice to only rewrite
// to the memory if the variable is changed. In this code the teensy only writes to the EEPROM once per power cycle so
// the functionality could be tested
//----------------------------------------------------------------------------------------------------------------------

// switch to only write to the EEPROM one time
bool sent = false;

//----------------------------------------------------------------------------------------------------------------------
// The 25LC128 has a storage structure of 16384 x 8bit organization. This means that there are 16384 address locations
// that each hold 8bits. In order to read/write to the EEPROM module you must send the address of the location you are
// interested in. This will be a number between 0 and 16384 stored with a 16bit interger where the 2 MSBs are dont
// cares.
//----------------------------------------------------------------------------------------------------------------------

// set the address to be read/write to
uint16_t address = 4;

// set the 1 byte data to be written
uint8_t data = 255;

void setup() {

  // set the chip select pin as an output:
  pinMode(eepromCS, OUTPUT);

  // write the CS pin high to disable the eeprom chip
  digitalWrite(eepromCS, HIGH);

  // initialize serial communication
  Serial.begin(9600);

  // initialize SPI:
  SPI.begin();

}

void loop() {

  // slow down the read rate for testing
  delay(1000);

  // only write the data to the EEPROM module once
  if(!sent){

  // write the data
  eepromWriteByte(eepromCS, address, data);

  // toggle sent to prevent the eeprom from being written to
  sent = true;
 }

 // repeatly read the data that was written
 else{
  Serial.println(eepromRead(eepromCS, address));
 }

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
