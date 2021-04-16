#ifdef __TESTANDO__

#include <chrono>

#include "myarduino.h"


int _pin_[64];

void pinMode(int pin, int mode)
{
}
void digitalWrite(int pin, int level)
{
  _pin_[pin] = level;
}

// Get time stamp in microseconds.
int micros()
{
  uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    return us; 
}
void ets_delay_us(uint32_t nus)
{
  usleep(nus);
}


#endif

