#include <circular_buffer.h>
#include <FlexCAN_T4.h>
#include <imxrt_flexcan.h>




//static CAN_message_t msg,rxmsg;

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

void setup() {
  // initialize serial communication
  //Serial.begin(112500);



  Can0.begin();
  Can0.setBaudRate(1000000);

}

void loop() {

        CAN_message_t msg;
        delay(100);
        analogWrite(13, HIGH);
        delay(100);
        analogWrite(13, LOW);
        msg.buf[0] = 1;
        msg.buf[1] = 9;
        msg.buf[2] = 0;
        msg.buf[3] = 10;
        msg.buf[4] = 50;
        msg.buf[5] = 0;
        msg.buf[6] = 99;
        msg.buf[7] = 0;

        msg.len = 8;  //CAN message length unit: Byte (8 bits)
        msg.id = 11; //CAN ID

        Serial.println(Can0.write(msg));


}
