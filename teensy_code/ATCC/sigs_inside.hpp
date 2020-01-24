#ifndef SIGS_INSIDE_OF_ATCC_HPP
#define SIGS_INSIDE_OF_ATCC_HPP

#include <StateCAN.h>

// format
// StateSignal ATCC_mySig(bit_len, signed, inverse_factor, offset, min, max, secondary_val [default to 0 if N/A])

// front ATCC
StateSignal ATCCF_boardTemp(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCF_brakePressureF(16, true, 10, 0, -3276, 3276, 0);

StateSignal ATCCF_suspensionTravelFL(16, true, 1000, 0, -32, 32, 0);
StateSignal ATCCF_suspensionTravelFR(16, true, 1000, 0, -32, 32, 0);

// rear ATCC
StateSignal ATCCR_boardTemp(16, true, 10, 0, -3276, 3276, 0);
StateSignal ATCCR_coolantTempRadInlet(16, true, 10, 0, -3276, 3276, 0);


#endif
