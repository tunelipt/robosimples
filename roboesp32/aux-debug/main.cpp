#include <iostream>

//#define __TESTANDO__

#include "myarduino.h"



#include "oem750.h"



int main()
{
  std::cout << "INICIO" << std::endl;
  int now = micros();
  
  ets_delay_us(1000);
  int fim = micros();
  std::cout << fim - now << std::endl;
  std::cout << "FIM" << std::endl;
  
  MoveAxis X(1, 2, 1000);
  return 0;
}
