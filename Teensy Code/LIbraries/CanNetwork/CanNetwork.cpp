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

void Signal::sample(int& read_val){
  if (read_val < sample_min_) sample_min_ = read_val;
  if (read_val > sample_max_) sample_max_ = read_val;
  sample_total_ += read_val;
  sample_count_++;
}

// return the average
int Signal::avg(){
  sample_average_ = sample_total_ / sample_count_;
  return sample_average_;
}

// reset the sensor sample values to their defaults
void Signal::reset(){
  sample_min_ = 2147483647;
  sample_max_ = -2147483647; // min value for a int
  sample_average_ = 0;
  sample_count_ = 0;
}

