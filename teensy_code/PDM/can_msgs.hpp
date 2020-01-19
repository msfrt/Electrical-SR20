#ifndef CAN_MESSAGES_HPP
#define CAN_MESSAGES_HPP

#include <FlexCAN_T4.h>
#include "sigs_inside.hpp"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> cbus1;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> cbus2;

static CAN_message_t msg, rxmsg;

// send message definitions


void send_PDM_10(){
  // the following line initializes a counter that is specific to PDM_10, but is static, so it's only created once.
  static StateCounter ctr;

  // update message ID and length
  msg.id = 260;
  msg.len = 8;

  // since the last calculation event, this records the sensor data into the actual signals. The calculations are
  // done automatically. why don't we simply just throw the sensors into the can message buffer, you ask? Well, the
  // signals are global and are used elsewhere for other important PDM functions.
  PDM_pdmCurrentAvg = pdm_current_sens.avg();
  PDM_pdmCurrentMax = pdm_current_sens.max();
  PDM_pdmCurrentMin = pdm_current_sens.min();

  // load up the message buffer
  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_pdmCurrentAvg.can_value();
  msg.buf[3] = PDM_pdmCurrentAvg.can_value() >> 8;
  msg.buf[4] = PDM_pdmCurrentMax.can_value();
  msg.buf[5] = PDM_pdmCurrentMax.can_value() >> 8;
  msg.buf[6] = PDM_pdmCurrentMin.can_value();
  msg.buf[7] = PDM_pdmCurrentMin.can_value() >> 8;

  // send the message
  cbus2.write(msg);
}



void send_PDM_11(){
  static StateCounter ctr;
  msg.id = 261;
  msg.len = 8;

  PDM_pdmVoltAvg = pdm_volt_sens.avg();
  PDM_pdmVoltMax = pdm_volt_sens.max();
  PDM_pdmVoltMin = pdm_volt_sens.min();

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_pdmVoltAvg.can_value();
  msg.buf[3] = PDM_pdmVoltAvg.can_value() >> 8;
  msg.buf[4] = PDM_pdmVoltMax.can_value();
  msg.buf[5] = PDM_pdmVoltMax.can_value() >> 8;
  msg.buf[6] = PDM_pdmVoltMin.can_value();
  msg.buf[7] = PDM_pdmVoltMin.can_value() >> 8;

  cbus2.write(msg);
}


void send_PDM_12(){
  static StateCounter ctr;
  msg.id = 262;
  msg.len = 8;

  PDM_fanRightCurrentAvg = fanr_current_sens.avg();
  PDM_fanRightCurrentMax = fanr_current_sens.max();
  PDM_fanRightCurrentMin = fanr_current_sens.min();

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_fanRightCurrentAvg.can_value();
  msg.buf[3] = PDM_fanRightCurrentAvg.can_value() >> 8;
  msg.buf[4] = PDM_fanRightCurrentMax.can_value();
  msg.buf[5] = PDM_fanRightCurrentMax.can_value() >> 8;
  msg.buf[6] = PDM_fanRightCurrentMin.can_value();
  msg.buf[7] = PDM_fanRightCurrentMin.can_value() >> 8;

  cbus2.write(msg);
}


void send_PDM_13(){
  static StateCounter ctr;
  msg.id = 263;
  msg.len = 8;

  PDM_fanRightVoltAvg = fanr_volt_sens.avg();
  PDM_fanRightVoltMax = fanr_volt_sens.max();
  PDM_fanRightVoltMin = fanr_volt_sens.min();

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_fanRightVoltAvg.can_value();
  msg.buf[3] = PDM_fanRightVoltAvg.can_value() >> 8;
  msg.buf[4] = PDM_fanRightVoltMax.can_value();
  msg.buf[5] = PDM_fanRightVoltMax.can_value() >> 8;
  msg.buf[6] = PDM_fanRightVoltMin.can_value();
  msg.buf[7] = PDM_fanRightVoltMin.can_value() >> 8;

  cbus2.write(msg);
}


void send_PDM_14(){
  static StateCounter ctr;
  msg.id = 264;
  msg.len = 8;

  PDM_fanLeftCurrentAvg = fanl_current_sens.avg();
  PDM_fanLeftCurrentMax = fanl_current_sens.max();
  PDM_fanLeftCurrentMin = fanl_current_sens.min();

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_fanLeftCurrentAvg.can_value();
  msg.buf[3] = PDM_fanLeftCurrentAvg.can_value() >> 8;
  msg.buf[4] = PDM_fanLeftCurrentMax.can_value();
  msg.buf[5] = PDM_fanLeftCurrentMax.can_value() >> 8;
  msg.buf[6] = PDM_fanLeftCurrentMin.can_value();
  msg.buf[7] = PDM_fanLeftCurrentMin.can_value() >> 8;

  cbus2.write(msg);
}


void send_PDM_15(){
  static StateCounter ctr;
  msg.id = 265;
  msg.len = 8;

  PDM_fanLeftVoltAvg = fanl_volt_sens.avg();
  PDM_fanLeftVoltMax = fanl_volt_sens.max();
  PDM_fanLeftVoltMin = fanl_volt_sens.min();

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_fanLeftVoltAvg.can_value();
  msg.buf[3] = PDM_fanLeftVoltAvg.can_value() >> 8;
  msg.buf[4] = PDM_fanLeftVoltMax.can_value();
  msg.buf[5] = PDM_fanLeftVoltMax.can_value() >> 8;
  msg.buf[6] = PDM_fanLeftVoltMin.can_value();
  msg.buf[7] = PDM_fanLeftVoltMin.can_value() >> 8;

  cbus2.write(msg);
}


void send_PDM_16(){
  static StateCounter ctr;
  msg.id = 266;
  msg.len = 8;

  PDM_wpCurrentAvg = wp_current_sens.avg();
  PDM_wpCurrentMax = wp_current_sens.max();
  PDM_wpCurrentMin = wp_current_sens.min();

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_wpCurrentAvg.can_value();
  msg.buf[3] = PDM_wpCurrentAvg.can_value() >> 8;
  msg.buf[4] = PDM_wpCurrentMax.can_value();
  msg.buf[5] = PDM_wpCurrentMax.can_value() >> 8;
  msg.buf[6] = PDM_wpCurrentMin.can_value();
  msg.buf[7] = PDM_wpCurrentMin.can_value() >> 8;

  cbus2.write(msg);
}


void send_PDM_17(){
  static StateCounter ctr;
  msg.id = 267;
  msg.len = 8;

  PDM_wpVoltAvg = wp_volt_sens.avg();
  PDM_wpVoltMax = wp_volt_sens.max();
  PDM_wpVoltMin = wp_volt_sens.min();

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_wpVoltAvg.can_value();
  msg.buf[3] = PDM_wpVoltAvg.can_value() >> 8;
  msg.buf[4] = PDM_wpVoltMax.can_value();
  msg.buf[5] = PDM_wpVoltMax.can_value() >> 8;
  msg.buf[6] = PDM_wpVoltMin.can_value();
  msg.buf[7] = PDM_wpVoltMin.can_value() >> 8;

  cbus2.write(msg);
}


void send_PDM_18(){
  static StateCounter ctr;
  msg.id = 268;
  msg.len = 8;

  PDM_fuelCurrentAvg = fuel_current_sens.avg();
  PDM_fuelCurrentMax = fuel_current_sens.max();
  PDM_fuelCurrentMin = fuel_current_sens.min();

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_fuelCurrentAvg.can_value();
  msg.buf[3] = PDM_fuelCurrentAvg.can_value() >> 8;
  msg.buf[4] = PDM_fuelCurrentMax.can_value();
  msg.buf[5] = PDM_fuelCurrentMax.can_value() >> 8;
  msg.buf[6] = PDM_fuelCurrentMin.can_value();
  msg.buf[7] = PDM_fuelCurrentMin.can_value() >> 8;

  cbus2.write(msg);
}


void send_PDM_19(){
  static StateCounter ctr;
  msg.id = 269;
  msg.len = 8;

  PDM_fuelVoltAvg = fuel_volt_sens.avg();
  PDM_fuelVoltMax = fuel_volt_sens.max();
  PDM_fuelVoltMin = fuel_volt_sens.min();

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_fuelVoltAvg.can_value();
  msg.buf[3] = PDM_fuelVoltAvg.can_value() >> 8;
  msg.buf[4] = PDM_fuelVoltMax.can_value();
  msg.buf[5] = PDM_fuelVoltMax.can_value() >> 8;
  msg.buf[6] = PDM_fuelVoltMin.can_value();
  msg.buf[7] = PDM_fuelVoltMin.can_value() >> 8;

  cbus2.write(msg);
}


void send_PDM_20(){
  static StateCounter ctr;
  msg.id = 270;
  msg.len = 8;

  PDM_mainVoltAvg = main_volt_sens.avg();
  PDM_mainVoltMax = main_volt_sens.max();
  PDM_mainVoltMin = main_volt_sens.min();

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_mainVoltAvg.can_value();
  msg.buf[3] = PDM_mainVoltAvg.can_value() >> 8;
  msg.buf[4] = PDM_mainVoltMax.can_value();
  msg.buf[5] = PDM_mainVoltMax.can_value() >> 8;
  msg.buf[6] = PDM_mainVoltMin.can_value();
  msg.buf[7] = PDM_mainVoltMin.can_value() >> 8;

  cbus2.write(msg);
}


void send_PDM_21(){
  static StateCounter ctr;
  msg.id = 271;
  msg.len = 8;

  PDM_dataVoltAvg = data_volt_sens.avg();
  PDM_dataVoltMax = data_volt_sens.max();
  PDM_dataVoltMin = data_volt_sens.min();

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_dataVoltAvg.can_value();
  msg.buf[3] = PDM_dataVoltAvg.can_value() >> 8;
  msg.buf[4] = PDM_dataVoltMax.can_value();
  msg.buf[5] = PDM_dataVoltMax.can_value() >> 8;
  msg.buf[6] = PDM_dataVoltMin.can_value();
  msg.buf[7] = PDM_dataVoltMin.can_value() >> 8;

  cbus2.write(msg);
}


void send_PDM_22(){
  static StateCounter ctr;
  msg.id = 272;
  msg.len = 8;

  PDM_keepAliveVoltAvg = keepalive_volt_sens.avg();
  PDM_keepAliveVoltMax = keepalive_volt_sens.max();
  PDM_keepAliveVoltMin = keepalive_volt_sens.min();

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_keepAliveVoltAvg.can_value();
  msg.buf[3] = PDM_keepAliveVoltAvg.can_value() >> 8;
  msg.buf[4] = PDM_keepAliveVoltMax.can_value();
  msg.buf[5] = PDM_keepAliveVoltMax.can_value() >> 8;
  msg.buf[6] = PDM_keepAliveVoltMin.can_value();
  msg.buf[7] = PDM_keepAliveVoltMin.can_value() >> 8;

  cbus2.write(msg);
}


void send_PDM_23(){
  static StateCounter ctr;
  msg.id = 273;
  msg.len = 8;

  PDM_boardTemp = pdm_board_temp_sens.value();
  PDM_brakelightVoltAvg = brakelight_volt_sens.avg();
  PDM_starterRelayVoltAvg = starter_volt_sens.avg();

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_boardTemp.can_value();
  msg.buf[3] = PDM_boardTemp.can_value() >> 8;
  msg.buf[4] = PDM_brakelightVoltAvg.can_value();
  msg.buf[5] = PDM_brakelightVoltAvg.can_value() >> 8;
  msg.buf[6] = PDM_starterRelayVoltAvg.can_value();
  msg.buf[7] = PDM_starterRelayVoltAvg.can_value() >> 8;

  cbus2.write(msg);
}


void send_PDM_24(){
  static StateCounter ctr;
  msg.id = 274;
  msg.len = 8;

  PDM_fanLeftPWM = fan_left.actual();
  PDM_fanRightPWM = fan_right.actual();
  PDM_wpPWM = water_pump.actual();
  PDM_teensyTemp = tempmonGetTemp(); // built-in teensy function

  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = PDM_fanLeftPWM.can_value();
  msg.buf[3] = PDM_fanRightPWM.can_value();
  msg.buf[4] = PDM_wpPWM.can_value();
  msg.buf[5] = 0;
  msg.buf[6] = PDM_teensyTemp.can_value();
  msg.buf[7] = PDM_teensyTemp.can_value() >> 8;

  cbus2.write(msg);
}








// ID 411 on bus 2
void read_ATCCF_11(CAN_message_t &imsg){
  ATCCF_brakePressureF = imsg.buf[2] | imsg.buf[3] << 8;
  ATCCF_brakePressureR = imsg.buf[4] | imsg.buf[5] << 8;
}


// ID 710 on bus 2
void read_USER_10(CAN_message_t &imsg){
  USER_fanLeftOverride    = imsg.buf[0] | imsg.buf[1] << 8;
  USER_fanRightOverride   = imsg.buf[2] | imsg.buf[3] << 8;
  USER_wpOverride         = imsg.buf[4] | imsg.buf[5] << 8;
  USER_brakeLightOverride = imsg.buf[6] | imsg.buf[7] << 8;
}


// ID 100 on bus 1
void read_M400_100(CAN_message_t &imsg){
  // multiplexer first-bit
  switch (imsg.buf[0]) {
    case 4:
      M400_rpm = imsg.buf[4] << 8 | imsg.buf[5];
      break;
  }
}


// ID 101 on bus 1
void read_M400_101(CAN_message_t &imsg){
  // multiplexer first-bit
  switch (imsg.buf[0]) {

    case 2:
      M400_batteryVoltage = imsg.buf[6] << 8 | imsg.buf[7];
      break;

    case 3:
      M400_engineTemp = imsg.buf[6] << 8 | imsg.buf[7];
      break;
  }
}






// function that reads the msg and then directs that data elsewhere
void read_can1(){
  if (cbus1.read(rxmsg)){

    switch (rxmsg.id) {
      case 100:
        read_M400_100(rxmsg);
        break;
      case 101:
        read_M400_101(rxmsg);
        break;
    } // end switch statement

  }
}


// function that reads the msg and then directs that data elsewhere
void read_can2(){
  if (cbus2.read(rxmsg)){

    switch (rxmsg.id) {
      case 411:
        read_ATCCF_11(rxmsg);
        break;
      case 710:
        read_USER_10(rxmsg);
        break;
    } // end switch statement

  }
}

#endif
