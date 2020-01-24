#ifndef CAN_SEND_ATCC_HPP
#define CAN_SEND_ATCC_HPP

#include <FlexCAN_T4.h>

// bus and message_t definition
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> cbus2;
static CAN_message_t msg;

// message definitions below

void send_ATCCF_10(){
  // static definition - only defined once (like a global variable, but is local to this function only)
  static StateCounter ctr;

  // update per message
  msg.id = 410;
  msg.len = 8;

  // automatically does calcs
  ATCCF_suspensionTravelFL = sus_travel_fl_sens.avg();
  ATCCF_suspensionTravelFR = sus_travel_fr_sens.avg();

  // load up the message buffer
  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = ATCCF_suspensionTravelFL.can_value();
  msg.buf[5] = ATCCF_suspensionTravelFL.can_value() >> 8;
  msg.buf[6] = ATCCF_suspensionTravelFR.can_value();
  msg.buf[7] = ATCCF_suspensionTravelFR.can_value() >> 8;

  // send it!
  cbus2.write(msg);

}

void send_can_front(){

  // make for each message
  static EasyTimer ATCCF_10_timer(200); // 200Hz
  if (ATCCF_10_timer.isup()){
    send_ATCCF_10();
  }


}

#endif
