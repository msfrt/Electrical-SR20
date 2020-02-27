#ifndef EEPROM_SIGNAL_DEFINITIONS_HPP
#define EEPROM_SIGNAL_DEFINITIONS_HPP

#include <EepromHelper.h>

EEPROM_Value<int> eeprom_engine_hours(0x0020);
EEPROM_Value<int> eeprom_engine_minutes(0x0024);
EEPROM_Value<int> eeprom_mileage(0x0028);


void initialize_eeprom_variables(){

  //eeprom_mileage = 0;
  //eeprom.write(eeprom_mileage);
  //
  // eeprom_engine_minutes = 0;
  // eeprom.write(eeprom_engine_minutes);


  // read the signals
  eeprom.read(eeprom_engine_hours);
  eeprom.read(eeprom_engine_minutes);
  eeprom.read(eeprom_mileage);
}

#endif
