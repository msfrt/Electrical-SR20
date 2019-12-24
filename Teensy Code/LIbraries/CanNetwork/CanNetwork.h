#ifndef CAN_NETWORK_H
#define CAN_NETWORK_H

#include "Arduino.h"

class CanSignal{

  private:
    unsigned long last_recieve_ = 0;
    unsigned int can_value_ = 0;
    double value_ = 0;
    const unsigned int bit_length_ = 0;
    const bool intel_ = true;
    const bool signed_ = false;
    const int inverse_factor_ = 1; // if the factor is 0.1 in the DBC, it should be 10 here. 0.01 -> 100
    const int offset_ = 0;
    int lower_bound_ = 0;
    int upper_bound_ = 0;
    bool valid_ = true;


  public:
    Signal() = default;
    Signal(unsigned int l, bool i, bool s, int f, int o, int lb, int ub) :
              bit_length_(l), intel_(i), signed_(s), inverse_factor_(f), offset_(o), lower_bound_(lb),
              upper_bound_(ub) {}; // new constructor for a signal that sends over CAN

    // getters
    double value() const {return value_;}

    // setters
    void can_value(unsigned int&);

    // allow us to use the = operator to assign values to the Signal
    const int& operator=(int);
    const int& operator=(float);
    const int& operator=(unsigned int);

    // implicit conversion to a int value
    operator int() const { return static_cast<int>(value_); }
    //implicit conversion to a double value
    operator double() const { return value_; }

};

#endif
