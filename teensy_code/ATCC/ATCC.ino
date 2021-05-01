 #include <EasyTimer.h>
#include <PWMControl.h>
#include <ReadADC.h>
#include <StateCAN.h>
#include <FlexCAN_T4.h>
#include <SPI.h>

// ATCC Module Select - 0 front, 1 back
const int ATCCMS = 0;

// sensor definitions
#include "sensors.hpp"

// signal definitions
#include "sigs_inside.hpp"

// can message definitions
#include "can_send.hpp"

void setup() {

  // Initialize serial communication
  Serial.begin(112500);

  // initialize SPI communication
  SPI.begin();

  //initialize the CAN Bus and set its baud rate to 1Mb
  cbus2.begin();
  cbus2.setBaudRate(1000000);

  // initialize SPI communication
  SPI.begin();

  //initialize ADCs
  initialize_ADCs();

}


void loop() {
  switch (ATCCMS)
  {
    case 0:
      sample_ADCs_F();
      send_can_F();
      break;
    case 1:
      sample_ADCs_R();
      send_can_R();
      break;
  }
}
