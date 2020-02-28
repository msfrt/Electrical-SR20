#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <StateCAN.h>
#include <FlexCAN_T4.h>
#include <EasyTimer.h>
#include <BoardTemp.h>
#include <EepromHelper.h>
#include <XBee.h>

// can bus decleration
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> cbus1;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> cbus2;

#include "sigs_inside.hpp"

#include "rainbow_pixels.hpp"

#include "can_send.hpp"

// board temp setup
#define READ_RESOLUTION_BITS 12
const int board_temp_pin = 19;
BoardTempDiode board_temp(board_temp_pin, READ_RESOLUTION_BITS, 26.2, 580);
EasyTimer board_temp_sample_timer(50);


// NeoPixel parameters & initialization
const int pixel_pin = 9;  // teensy pin #
const int pixel_cnt = 1;  // number of LEDs
const int pixel_brightness_percent = 100;  // 0 - 100%
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(pixel_cnt, pixel_pin, NEO_GRB + NEO_KHZ800);

// EEPROM
const int eeprom_cs_pin = 10;
EEPROM_25LC128 eeprom(eeprom_cs_pin);


// GPS messages
// GPS communications datasheet: https://cdn-shop.adafruit.com/datasheets/PMTK_A11.pdf
char set_gps_fast_serial_cmd[] = {0x24, 0x50, 0x4D, 0x54, 0x4B, 0x32, 0x35, 0x31, 0x2C, 0x31, 0x31, 0x35, 0x32, 0x30, 0x30, 0x2A, 0x31, 0x46, 0x0D, 0x0A};
char set_gps_fast_update_cmd[] = {0x24, 0x50, 0x4D, 0x54, 0x4B, 0x32, 0x32, 0x30, 0x2C, 0x31, 0x30, 0x30, 0x2A, 0x32, 0x46, 0x0D, 0x0A};


EasyTimer debug_timer(1);

// EEPROM VALUES
EEPROM_Value<int> board_hours(0x0020);
EEPROM_Value<int> board_minutes(0x0024);


// XBee setup and parameters ---------------------

//increase buffer size for serial data coming in from the C50 (unit: bytes)
#define SERIAL4_RX_BUFFER_SIZE 1024

// used to hold the data that will be sent to the XBee. This must be less than 256, and you may get droped frames or
// errors when sending too close to the 246 limit
unsigned char xbee_payload[224] = {0};

// xbee object
XBee xbee = XBee();

// XBee CTS pin (when XBee can not recieve any more data, this is HIGH)
const int xbee_cts_pin = 18;

// address of the recieving modules. Currently, this is the address that all XBees will read
XBeeAddress64 addr64 = XBeeAddress64(0x00000000,0x0000FFFF);

// Used to create a variable to hold the payload (in explicit mode)
ZBExplicitTxRequest telemetry_tx_rq = ZBExplicitTxRequest(addr64, xbee_payload, sizeof(xbee_payload));

// incoming messages from the Xbee
ZBExplicitRxResponse xbee_rx = ZBExplicitRxResponse();

// for safety, only trigger a lap if this string was recieved:
char laptrigger_rx_key[] = "SR20";

// laptrigger support functions
#include "lap_trigger.hpp"

// will continue to send laptrigger signals for this duration after a signal is recieved
unsigned long trigger_send_duration_ms = 200;
unsigned long trigger_send_until;


void setup() {

  Serial.begin(115200);
  Serial3.begin(115200); // TX & RX for XBee
  Serial4.begin(115200); // RX from C50

  // Attach XBee to serial 3
  xbee.setSerial(Serial3);
  pinMode(xbee_cts_pin, INPUT);

  // initilize CAN busses
  cbus1.begin();
  cbus1.setBaudRate(1000000);
  cbus2.begin();
  cbus2.setBaudRate(1000000);

  // initialize SPI bus
  SPI.begin();

  // initialize eeprom
  eeprom.begin();


  analogReadResolution(READ_RESOLUTION_BITS);
  board_temp.begin();

  // begin NeoPixel
  pixel.setBrightness(map(pixel_brightness_percent, 0, 100, 0, 255));
  pixel.begin();
  pixel.show();




}




void loop() {
  // put your main code here, to run repeatedly:

  if (board_temp_sample_timer.isup())
    board_temp.sample();


  // turn the neopixels rainbow colors :-)
  if (!laptrigger_sucess_pixel(pixel)){
    if (digitalRead(xbee_cts_pin) == HIGH){
      pixel.setPixelColor(0, 255, 0, 0);
      pixel.show();
    } else {
      rainbow_pixels(pixel);
    }
  }

  // send can messages
  send_can1();
  send_can2();

  // attempt to read a lap trigger
  if (laptrigger_read()){
    TCGPS_laptrigger = 100;
    send_TCGPS_11(); // a.k.a. the lap trigger message

    trigger_send_until = millis() + trigger_send_duration_ms;

  } else if (millis() <= trigger_send_until){
    // do nothing

  } else {
    TCGPS_laptrigger = 0;
  }

  // send telemetry shenanigans
  telemetry_send();


  // board time tracking (counts cumulative time the board has been running since 4:02PM February 22, 2020)
  timer(board_hours, board_minutes);

  if (debug_timer.isup()){
  }

}





template <class T1, class T2>
void timer(EEPROM_Value<T1> &hours, EEPROM_Value<T2> &minutes){
  static unsigned int last_minute_millis = 0;

  if (millis() > last_minute_millis + 60000){  // 60,000ms = 60s

    // minutes AND hours have changed
    if (eeprom.read(minutes) >= 59){
      hours = eeprom.read(hours) + 1;
      minutes = 0;
      eeprom.write(hours);
      eeprom.write(minutes);

    // just minutes have changed
    } else {
      minutes = eeprom.read(minutes) + 1;
      eeprom.write(minutes);
    }

    // update the last minute timer
    last_minute_millis = millis();
  }
}

EasyTimer sendtimer(10);


// reads in data from the C50 serial input, packages it for the XBee, and then transmits to the XBee module
bool telemetry_send(){
  static int current_byte = 0;

  // if there is data from the C50 and the Xbee is able to recieve data
  if (Serial4.available() && digitalRead(xbee_cts_pin) == LOW){

    Serial.print("Bytes available for reading: "); Serial.println(Serial4.available());
    Serial.print("packing data: ");
    // fill up the transmit array with C50 data
    // for (uint8_t byte = 0; byte < sizeof(xbee_payload); byte++){
    //   xbee_payload[byte] = Serial4.read();
    //   Serial.print(xbee_payload[byte], HEX); Serial.print(" ");
    // }
    while (Serial4.available() && current_byte < sizeof(xbee_payload) - 1){
      xbee_payload[current_byte] = Serial4.read();
      Serial.print(xbee_payload[current_byte], HEX); Serial.print(" ");
      current_byte++;
    }

    if (current_byte >= sizeof(xbee_payload)){
      Serial.println("\n");

      // create a transmission request with the new payload
      ZBExplicitTxRequest zbTx = ZBExplicitTxRequest(addr64, xbee_payload, sizeof(xbee_payload));

      // send the message
      xbee.send(zbTx);
      //xbee.send(telemetry_tx_rq);

      current_byte = 0;

      // xbee.send returns void, so we'll assume that it send okay lololol
      return true;
    }

  }

  return false;

}





// ron's telem code --------------------------------------------------------

// //Read from C50 and send to XBee
// if((Serial1.available() > 0) && (digitalRead(CTS) == LOW)){
//   // Fill bits up to 224 (Position 223)
//   if(counter <223){ // n-1 bytes of Payload[] array
//     payload[counter] = Serial1.read();
//     counter++;
//   }
//   else{ //when counter is 223
//     payload[counter] = Serial1.read();
//
//     // Create new Transmission Request with newly finished payload
//     ZBExplicitTxRequest zbTx = ZBExplicitTxRequest(addr64,payload,sizeof(payload));
//
//     // Send Transmition
//     xbee.send(zbTx);
//     counter = 0;
//   }
// }
