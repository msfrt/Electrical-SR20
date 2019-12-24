#include "Arduino.h"
#include "CanNetwork.h"

const int& Signal::operator=(int new_value){
  this->value_ = new_value;
  return value_;
}

const int& Signal::operator=(float new_value){
  this->value_ = static_cast<int>(new_value);
  return value_;
}

const int& Signal::operator=(unsigned int new_value){
  this->value_ = static_cast<int>(new_value);
  return value_;
}

void Signal::can_value(unsigned int& val){
  // need to figure out how to handle unsigned and signed ints better than
  // last year. Last year we converted everything to an int, which was okay because we never had a 16bit
  // unsigned value, however that may not always be the case. Then, we would have some sort of negative value
  // is we read an unisnged int and assume that it is signed.
}

// return the average
// int Signal::avg(){
//   sample_average_ = sample_total_ / sample_count_;
//   return sample_average_;
// }

// reset the sensor sample values to their defaults
// void Signal::reset(){
//   sample_min_ = 2147483647;
//   sample_max_ = -2147483647; // min value for a int
//   sample_average_ = 0;
//   sample_count_ = 0;
// }
