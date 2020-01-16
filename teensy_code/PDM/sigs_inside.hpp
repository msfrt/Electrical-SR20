#ifndef DONT_SMOKE_CIGARETTES
#define DONT_SMOKE_CIGARETTES

#include <StateCAN.h>
#include <FlexCAN_T4.h>

StateSignal M400_engineTemp(16, true, 10, 0, -3276, 3276, 100.0);
StateSignal M400_rpm(16, true, 1, 0, -3276.8, 3276.7, 0);
StateSignal M400_batteryVoltage(16, true, 100, 0, -327.68, 327.67, 13.5);

StateSignal USER_brakeLightOverride(16, true, 1, 0, -5, 100, -1, 1000);
StateSignal USER_fanLeftOverride(16, true, 1, 0, -5, 100, -1, 1000);
StateSignal USER_fanRightOverride(16, true, 1, 0, -5, 100, -1, 1000);
StateSignal USER_wpOverride(16, true, 1, 0, -5, 100, -1, 1000);

#endif
