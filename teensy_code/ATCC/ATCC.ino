// written by Dave Yonkers, 2020

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

  //initialize the CAN Bus and set its baud rate to 1Mb
  cbus2.begin();
  cbus2.setBaudRate(1000000);

  // initialize SPI communication
  SPI.begin();

  switch (ATCCMS) {
    case 0:
      initialize_ADCs_F();
      break;
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  //
  // send_can_front(); // this will go somewhere in here.
}
