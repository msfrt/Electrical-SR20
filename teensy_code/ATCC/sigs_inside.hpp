#ifndef SIGS_INSIDE_OF_ATCC_HPP
#define SIGS_INSIDE_OF_ATCC_HPP

#include <StateCAN.h>

// format
// StateSignal ATCC_mySig(bit_len, signed, inverse_factor, offset, min, max, secondary_val [default to 0 if N/A])

// front ATCC
StateSignal ATCCF_boardTemp(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCF_brakePressureF(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCF_brakePressureR(16, true, 10, 0, -3276, 3276, 0); //Is this correct in the .dbc

StateSignal ATCCF_coolantTempRadMiddle(16, true, 10, 0, -3276, 3276, 0);

StateSignal ATCCF_rotorTempFL(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCF_rotorTempFR(16, true, 10, 0, -3276, 3276, 0);

StateSignal ATCCF_steeringWheelAngle(16, true, 10, 0, -3276, 3276, 0);

StateSignal ATCCF_suspensionTravelFL(16, true, 1000, 0, -32, 32, 0);
StateSignal ATCCF_suspensionTravelFR(16, true, 1000, 0, -32, 32, 0);

StateSignal ATCCF_teensyTemp(16, true, 10, 0, -3276, 3276, 0);

StateSignal ATCCF_tireTempFLI(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCF_tireTempFLM(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCF_tireTempFLO(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCF_tireTempFRI(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCF_tireTempFRM(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCF_tireTempFRO(16, true, 10, 0, -3276, 3276, 0);

StateSignal ATCCF_suspensionTravelFL(16, true, 1000, 0, -32, 32, 0);
StateSignal ATCCF_suspensionTravelFR(16, true, 1000, 0, -32, 32, 0);

// rear ATCC
StateSignal ATCCR_boardTemp(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCR_coolantTempRadInlet(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCR_coolantTempRadOutlet(16, true, 10, 0, -3276, 3276, 0);

StateSignal ATCCR_rotorTempRL(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCR_rotorTempRR(16, true, 10, 0, -3276, 3276, 0);

StateSignal ATCCR_suspensionTravelRL(16, true, 1000, 0, -32, 32, 0);
StateSignal ATCCR_suspensionTravelRR(16, true, 1000, 0, -32, 32, 0);

StateSignal ATCCR_teensyTemp(16, true, 10, 0, -3276, 3276, 0);

StateSignal ATCCR_tireTempRLI(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCR_tireTempRLM(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCR_tireTempRLO(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCR_tireTempRRI(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCR_tireTempRRM(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCR_tireTempRRO(16, true, 10, 0, -3276, 3276, 0);


#endif
