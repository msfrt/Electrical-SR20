

//------------------------------------------------------------------------
// Written by:  Ron Hodge and Sammy Cohen
// Created:     01/21/2020
// Purpose:     Convert Analog Sensor Readings to CAN Messages
// Description: Reads ADC and creates CAN Message
//------------------------------------------------------------------------

#include <SPI.h>
#include <ReadADC.h>
#include <EasyTimer.h>
#include <StateCAN.h>

//Front = 0
//Back = 1
#define ATCC 0

//ADC CS pins
#define ADC1 10
#define ADC2 9
#define ADC3 8

//define message type for CAN
static CAN_message_t msg;
static CAN_message_t rxmsg;


//initialize the CAN send timer
EasyTimer CANTimer1000Hz(1000);
EasyTimer CANTimer500Hz(500);
EasyTimer CANTimer200Hz(200);
EasyTimer CANTimer100Hz(100);
EasyTimer CANTimer50Hz(50);
EasyTimer CANTimer20Hz(20);
EasyTimer CANTimer10Hz(10);
EasyTimer CANTimer1Hz(1);

//initialize the sensor reading timers
EasyTimer SensTimer1000Hz(10000);
EasyTimer SensTimer1000Hz(2000);
EasyTimer SensTimer1000Hz(1000);
EasyTimer SensTimer1000Hz(500);
EasyTimer SensTimer1000Hz(200);
EasyTimer SensTimer1000Hz(100);
EasyTimer SensTimer1000Hz(50);
EasyTimer SensTimer1000Hz(20);
EasyTimer SensTimer1000Hz(10);
EasyTimer SensTimer1000Hz(1);

// initialize LED timer and variable
EasyTimer LEDTimer(); //what Hz shoud this be for the loop Probably should ask dave later this week
bool LED_on = false;

//Create Sensors
ADCSensor sensor1(ADC1,0,0,0);      //Rename when certain of sensors being used
ADCSensor sensor2(ADC1,1,0,0);
ADCSensor sensor3(ADC1,2,0,0);
ADCSensor sensor4(ADC1,3,0,0);
ADCSensor sensor5(ADC1,4,0,0);
ADCSensor sensor6(ADC1,5,0,0);
ADCSensor sensor7(ADC1,6,0,0);
ADCSensor sensor8(ADC1,7,0,0);
ADCSensor sensor9(ADC2,0,0,0);
ADCSensor sensor10(ADC2,1,0,0);
ADCSensor sensor11(ADC2,2,0,0);
ADCSensor sensor12(ADC2,3,0,0);
ADCSensor sensor13(ADC2,4,0,0);
ADCSensor sensor14(ADC2,5,0,0);
ADCSensor sensor15(ADC2,6,0,0);
ADCSensor sensor16(ADC2,7,0,0);
ADCSensor sensor17(ADC3,0,0,0);
ADCSensor sensor18(ADC3,1,0,0);
ADCSensor sensor19(ADC3,2,0,0);
ADCSensor sensor20(ADC3,3,0,0);
ADCSensor sensor21(ADC3,4,0,0);
ADCSensor sensor22(ADC3,5,0,0);
ADCSensor sensor23(ADC3,6,0,0);
ADCSensor sensor24(ADC3,7,0,0);

//need do finde all sensors for front and rear and Declare them?
//ex. Sensor FR_DAMPER_POS;

// teensy voltage in mV * 10
const int teensy_voltage_mV10 = 33000;


void setup() {
  Serial.begin(115200);
  SPI.begin();

  switch(ATCC){ // Not sure if there is a way to set up the ADCSensor Sensors
    //Front ATCC
    case 0:
      sensor1.begin();
      sensor2.begin();
      sensor3.begin();
      sensor4.begin();
      sensor5.begin();
      sensor6.begin();
      sensor7.begin();
      sensor8.begin();
      sensor9.begin();
      sensor10.begin();
      sensor11.begin();
      sensor12.begin();
      //sensor13.begin(); // Placeholders: Will set up later
      //sensor14.begin();
      //sensor15.begin();
      //sensor16.begin();
      //sensor17.begin();
      //sensor18.begin();
      //sensor19.begin();
      //sensor20.begin();
      //sensor20.begin();
      //sensor21.begin();
      //sensor22.begin();
      //sensor23.begin();
      //sensor24.begin();
    break;

    //Rear ATCC
    case 1:
      //sensor1.begin();
      //sensor2.begin();
      //sensor3.begin();
      //sensor4.begin();
      //sensor5.begin();
      //sensor6.begin();
      //sensor7.begin();
      //sensor8.begin();
      //sensor9.begin();
      //sensor10.begin();
      //sensor11.begin();
      //sensor12.begin();
      sensor13.begin();     // Placeholders: Will set up later
      sensor14.begin();
      sensor15.begin();
      sensor16.begin();
      sensor17.begin();
      sensor18.begin();
      sensor19.begin();
      sensor20.begin();
      sensor20.begin();
      sensor21.begin();
      sensor22.begin();
      sensor23.begin();
      sensor24.begin();
    break;

  }

}

void loop() {
  switch(ATCC){
    case 0:


    break;

    case 1:


    break;
  }


}
