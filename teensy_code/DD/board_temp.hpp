#ifndef BOARD_IS_PIPING_HOT_HPP
#define BOARD_IS_PIPING_HOT_HPP

// board temp is kinda a little muffed up, so here's a class to sample it and what not.
// it's not like a normal sensor, but this class makes it act like one.

class BoardTemp{
  private:
    int pin_;
    int read_bits_;
    float max_read_val_;

    float t_cal_; // temperature at calibration
    float mv_cal_; // millivoltage at calibration

    float uC_millivoltage_ = 3300; // teensies are 3.3V

    float value_;

    unsigned int running_read_total_ = 0;
    unsigned int read_count_ = 0;
    float read_mvoltage_;
    bool values_updated_ = false;

    void reset();
    void calculate();

  public:
    // constructors
    BoardTemp() = delete;
    BoardTemp(int pin, int read_bits, float temp_cal, float mv_cal) :
              pin_(pin), read_bits_(read_bits), t_cal_(temp_cal), mv_cal_(mv_cal) {};

    void begin();
    void sample();
    float value();
};

// must call in setup();
void BoardTemp::begin(){
  this->max_read_val_ = pow(2, this->read_bits_) - 1; // max value that analogRead can return
  pinMode(this->pin_, INPUT); // initialize pin
}

void BoardTemp::sample(){
  this->running_read_total_ += analogRead(this->pin_);
  this->read_count_++;
  this->values_updated_ = false;
}

void BoardTemp::reset(){
  this->running_read_total_ = 0;
  this->read_count_ = 0;
}

// this is where the wonk begins
void BoardTemp::calculate(){
   // get the average
  this->running_read_total_ /= this->read_count_;
  // turn the average into the millivoltage
  this->read_mvoltage_ = (this->running_read_total_ / this->max_read_val_) * this->uC_millivoltage_;

  // formula here: https://www.openimpulse.com/blog/2012/07/low-cost-diode-temperature-sensor/
  // the diode that we use changes -2mV/ºC
  this->value_ = this->t_cal_ + (this->mv_cal_ - this->read_mvoltage_) * 0.5;

  this->values_updated_ = true;
}

float BoardTemp::value(){
  this->calculate();
  this->reset();
  return this->value_;
}





#endif
