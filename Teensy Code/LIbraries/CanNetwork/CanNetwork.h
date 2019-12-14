#ifndef CAN_NETWORK_H
#define CAN_NETWORK_H

#include "Arduino.h"

class CanSignal{

  private:
    unsigned long last_recieve_ = 0;
    unsigned int can_value_ = 0;
    int value_ = 0;
    const unsigned int bit_length_ = 0;
    const bool intel_ = true;
    const bool signed_ = false;
    const double factor_ = 1;
    const int offset_ = 0;
    int lower_bound_ = 0;
    int upper_bound_ = 0;
    bool valid_ = true;

  public:
    CanSignal() = delete;
    CanSignal(unsigned int l, bool i, bool s, double f, int o, int lb, int ub) :
              bit_length_(l), intel_(i), signed_(s), factor_(f), offset_(o), lower_bound_(lb),
              upper_bound_(ub) {};

    friend void operator=(int&);
    friend void operator=(unsigned int&);
    friend int operator=(CanSignal&);
};

#endif
