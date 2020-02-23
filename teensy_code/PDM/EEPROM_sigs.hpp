#ifndef EEPROM_SIGNAL_DEFINITIONS_HPP
#define EEPROM_SIGNAL_DEFINITIONS_HPP

#include <EepromHelper.h>

EEPROM_Value<int> eeprom_engine_hours(0x0020);
EEPROM_Value<int> eeprom_engine_minutes(0x0024);

#endif
