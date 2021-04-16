

#ifndef _MYARDUINO_H_
#define _MYARDUINO_H_



#include <unistd.h>
#include <cstdint>

#define LOW 0
#define HIGH 1
#define OUTPUT 1



void pinMode(int pin, int mode);
void digitalWrite(int pin, int level);
int micros();
void ets_delay_us(uint32_t nus);


#endif 




