#ifndef EEPROM_HELPER_H
#define EEPROM_HELPER_H

#include <Arduino.h>

class EEPROM_25LC128{
  private:
    const int chip_select_;
    unsigned long next_write_time_ = 0;
    const unsigned long us_delay_time_ = 3200; // microseconds - this delay has been proven to work on TC board
    bool safe_to_write_ = true; // if it's safe to write. Only updated through the setter
    const int max_write_attempts_ = 16; // when verify-write is true, this is the maximum attempts before failure is accepted
    int current_write_attempt_ = 0;


  public:

    // constructors
    EEPROM_25LC128() = delete;
    EEPROM_25LC128(int chip_select_pin) : chip_select_(chip_select_pin) {};

    // prepares chip for reading and writing
    void begin();

    // reads data from the eeprom chip
    uint8_t readByte(uint16_t address);

    // read data from a redundant address
    uint8_t readByteRedundant(uint16_t address1, uint16_t address2, uint16_t address3);

    // writes data to the eeprom chip
    bool writeByte(uint16_t address, uint8_t data, bool verify = false);

    // write redundant data to three adresses
    bool writeByteRedundant(uint16_t address1, uint16_t address2, uint16_t address3, uint8_t data);

};


#endif
