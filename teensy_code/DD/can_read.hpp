#ifndef CAN_READ_DRIVERDISPLAY_HPP
#define CAN_READ_DRIVERDISPLAY_HPP

#include "sigs_inside.hpp"
#include <FlexCAN_T4.h>
#include "user_message_display.hpp"
#include "rotor_temp_adjustment.hpp"

static CAN_message_t rxmsg;
extern UserMessageDisplay warning_message_display;


// ID 161 on bus 1
void read_TCGPS_11(CAN_message_t &imsg){
  TCGPS_laptrigger.set_can_value(imsg.buf[0]);
}

// ID 410 on bus 2
void read_ATCCF_10(CAN_message_t &imsg){
  ATCCF_brakeBias.set_can_value(imsg.buf[2] | imsg.buf[3] << 8);
}

// ID 411 on bus 2
void read_ATCCF_11(CAN_message_t &imsg){
  ATCCF_brakePressureF.set_can_value(imsg.buf[2] | imsg.buf[3] << 8);
  ATCCF_brakePressureR.set_can_value(imsg.buf[4] | imsg.buf[5] << 8);
}

/*
 * Decode a CAN frame for the message ATCCF_12
 * \param imsg A reference to the incoming CAN message frame
 */
void read_ATCCF_12(CAN_message_t &imsg) {

	ATCCF_counterMsg412.set_can_value(((imsg.buf[0] & 0b00001111)));
	ATCCF_coolantTempRadMiddle.set_can_value((imsg.buf[2]) | (imsg.buf[3] << 8));
	ATCCF_rotorTempFL.set_can_value((imsg.buf[4]) | (imsg.buf[5] << 8));
	ATCCF_rotorTempFR.set_can_value((imsg.buf[6]) | (imsg.buf[7] << 8));

  // new rotor temp values, update the adjusted ones
  // calculate_adjusted_rotor_temp(ATCCF_rotorTempFL, M400_inletAirTemp, DD_adjRotorTempFL, FL_ROTOR_DISC_AREA_RATIO);
  // calculate_adjusted_rotor_temp(ATCCF_rotorTempFR, M400_inletAirTemp, DD_adjRotorTempFR, FR_ROTOR_DISC_AREA_RATIO);

  // Serial.println(ATCCF_rotorTempFL.value());
  // Serial.println(M400_inletAirTemp.value());
  // Serial.println(FR_ROTOR_DISC_AREA_RATIO);
  // Serial.print("Calculated: ");
  // Serial.println(DD_adjRotorTempFL.value());
  
  // Serial.println();

}

/*
 * Decode a CAN frame for the message ATCCR_11
 * \param imsg A reference to the incoming CAN message frame
 */
void read_ATCCR_11(CAN_message_t &imsg) {

	ATCCR_counterMsg461.set_can_value(((imsg.buf[0] & 0b00001111)));
	ATCCR_boardTemp.set_can_value((imsg.buf[2]) | (imsg.buf[3] << 8));
	ATCCR_rotorTempRL.set_can_value((imsg.buf[4]) | (imsg.buf[5] << 8));
	ATCCR_rotorTempRR.set_can_value((imsg.buf[6]) | (imsg.buf[7] << 8));

  // new rotor temp values, update the adjusted ones
  //calculate_adjusted_rotor_temp(ATCCR_rotorTempRL, M400_inletAirTemp, DD_adjRotorTempRL, RL_ROTOR_DISC_AREA_RATIO);
  //calculate_adjusted_rotor_temp(ATCCR_rotorTempRR, M400_inletAirTemp, DD_adjRotorTempRR, RR_ROTOR_DISC_AREA_RATIO);

}


// ID 261 on bus 2
void read_PDM_11(CAN_message_t &imsg){
  PDM_pdmVoltAvg.set_can_value(imsg.buf[2] | imsg.buf[3] << 8);
}


// ID 274 on bus 2
void read_PDM_24(CAN_message_t &imsg){
  PDM_fanLeftPWM.set_can_value(imsg.buf[2]);
}


// ID 710 on bus 2
void read_USER_10(CAN_message_t &imsg){
  USER_fanLeftOverride.set_can_value(imsg.buf[0]);
  USER_fanRightOverride.set_can_value(imsg.buf[1]);
  USER_wpOverride.set_can_value(imsg.buf[2]);
  USER_brakeLightOverride.set_can_value(imsg.buf[3]);
}


// ID 711 on bus 2
void read_USER_11(CAN_message_t &imsg){
  USER_driverSignal.set_can_value(imsg.buf[0]);
}

// ID 281 on bus 2
void read_PDM_281(CAN_message_t &imsg){
  PDM_driverDisplayLEDs.set_can_value(imsg.buf[0]);
}


// ID 712 or ID 280 on bus 2 - the 64-bit message
char obd_message[9] = ""; // <= 8 characters!!! One extra char in definition for the null-terminator.
void read_driver_message(CAN_message_t &imsg){
  for (int i = 0; i < 8; i++){
    obd_message[i] = imsg.buf[i];
  }
  obd_message[8] = '\0'; // just for safety

  warning_message_display.begin();
}



// ID 100 on bus 1 - M400 dataset 1
void read_M400_100(CAN_message_t &imsg){
  // multiplexer first-bit
  switch (imsg.buf[0]) {

    case 1:
      M400_groundSpeed.set_can_value(imsg.buf[2] << 8 | imsg.buf[3]);
      break;

    case 2:
      M400_tcPowerReduction.set_can_value(imsg.buf[6] << 8 | imsg.buf[7]);
      break;

    case 4:
      M400_ignCutLevelTotal.set_can_value(imsg.buf[2] << 8 | imsg.buf[3]);
      M400_rpm.set_can_value(imsg.buf[4] << 8 | imsg.buf[5]);
      break;

    case 5:
      M400_gear.set_can_value(imsg.buf[2] << 8 | imsg.buf[3]);
      break;

  }
}


// ID 101 on bus 1 - M400 dataset 2
void read_M400_101(CAN_message_t &imsg){
  // multiplexer first-bit
  switch (imsg.buf[0]) {

    case 2:
      M400_batteryVoltage.set_can_value(imsg.buf[6] << 8 | imsg.buf[7]);
      break;

    case 3:
      M400_engineTemp.set_can_value(imsg.buf[6] << 8 | imsg.buf[7]);
      break;

    case 10:
      M400_fuelPressure.set_can_value(imsg.buf[2] << 8 | imsg.buf[3]);
      break;
    
    case 14:
			// M400_ignEtComp.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			// M400_ignMapComp.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_inletAirTemp.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

    case 15:
      M400_oilPressure.set_can_value(imsg.buf[6] << 8 | imsg.buf[7]);
      break;

    case 16:
      M400_oilTemp.set_can_value(imsg.buf[2] << 8 | imsg.buf[3]);
      break;
  }
}


/*
 * Decode a CAN frame for the message C50_m400Data
 * \param imsg A reference to the incoming CAN message frame
 */
void read_C50_m400Data(CAN_message_t &imsg) {

	// multiplexer signal
	int C50rowCounterM400Data = imsg.buf[0];

	switch (C50rowCounterM400Data) {

		case 0:
			C50_m400ExhaustGasTemp1.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			C50_m400ExhaustGasTemp2.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			C50_m400ExhaustGasTemp3.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 1:
			C50_m400ExhaustGasTemp4.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			C50_mm5YawRate.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			C50_tcSet.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
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
      case 120:
        read_C50_m400Data(rxmsg);
        break;
      case 161:
        read_TCGPS_11(rxmsg);
        break;
    } // end switch statement

  }
}


// function that reads the msg and then directs that data elsewhere
void read_can2(){
  if (cbus2.read(rxmsg)){

    switch (rxmsg.id) {
      case 261:
        read_PDM_11(rxmsg);
        break;
      case 274:
        read_PDM_24(rxmsg);
        break;
      case 281:
        read_PDM_281(rxmsg);
        break;
      case 280:
      case 712:
        read_driver_message(rxmsg);
        break;
      case 410:
        read_ATCCF_10(rxmsg);
        break;
      case 411:
        read_ATCCF_11(rxmsg);
        break;
      case 710:
        read_USER_10(rxmsg);
        break;
      case 711:
        read_USER_11(rxmsg);
        break;
      case 461:
        read_ATCCR_11(rxmsg);
        break;
      case 412:
        read_ATCCF_12(rxmsg);
        break;
    } // end switch statement

  }
}

#endif
