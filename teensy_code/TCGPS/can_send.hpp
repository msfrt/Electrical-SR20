#ifndef CAN_SEND_DRIVERDISPLAY_HPP
#define CAN_SEND_DRIVERDISPLAY_HPP

#include <FlexCAN_T4.h>
#include <EasyTimer.h>
#include <BoardTemp.h>
#include "sigs_inside.hpp"


static CAN_message_t msg;
extern BoardTempDiode board_temp;


void send_TCGPS_10(){
  // the following line initializes a counter that is specific to PDM_10, but is static, so it's only created once.
  static StateCounter ctr;

  // update message ID and length
  msg.id = 160;
  msg.len = 8;

  // since the last calculation event, this records the sensor data into the actual signals. The calculations are
  // done automatically. why don't we simply just throw the sensors into the can message buffer, you ask? Well, the
  // signals are global and are used elsewhere for other important PDM functions.
  TCGPS_boardTemp = board_temp.value();
  TCGPS_teensyTemp = tempmonGetTemp();


  // load up the message buffer
  msg.buf[0] = ctr.value();
  msg.buf[1] = 0;
  msg.buf[2] = TCGPS_boardTemp.can_value();
  msg.buf[3] = TCGPS_boardTemp.can_value() >> 8;
  msg.buf[4] = TCGPS_teensyTemp.can_value();
  msg.buf[5] = TCGPS_teensyTemp.can_value() >> 8;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  // send the message
  cbus1.write(msg);
}


// lap trigger message, no fancy signals needed.
void send_TCGPS_11(){

  msg.id = 161;
  msg.len = 8;

  msg.buf[0] = 100;
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  // send the message
  cbus1.write(msg);
}



void send_can1(){
  // hey, there's nothing in here! (for now, at least)
}

void send_can2(){

  static EasyTimer TCGPS_10_timer(10); // 10Hz
  if (TCGPS_10_timer.isup()){
    send_TCGPS_10();
  }

}

#endif
