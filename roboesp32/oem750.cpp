

#define __TESTE_COMP__



#include <cstdint>

#ifdef __TESTANDO__

#include "myarduino.h"

#else

#include "Arduino.h"
#include <rom/ets_sys.h>

#endif


#include "oem750.h"




MoveAxis::MoveAxis(int pinstep, int pindir, int vel, int acc):
  _position(0), _pstep(pinstep), _pdir(pindir),
  _vel(vel), _acc(acc), _nus(-1)
{

  pinMode(_pstep, OUTPUT);
  pinMode(_pdir, OUTPUT);

  digitalWrite(_pstep, LOW);
  digitalWrite(_pdir, LOW);

}

int MoveAxis::get_position()
{
  return _position;
}

int MoveAxis::set_position(int pos)
{
  int oldpos = _position;
  _position = pos;
  return oldpos;
}


int MoveAxis::rmove(int nstep)
{
  int dir = 0;
  
  if (nstep < 0)
    {
      nstep = -nstep;
      dir = 1;
    }

  // Calculate half of the time interval per timestep:
  int _nus = 500000 / _vel; // us / (1/2 step)
  int incpos = (dir==0) ? 1 : -1;
  
  
  // Setting direction:
  digitalWrite(_pdir, dir);
  for (int i = 0; i < nstep; ++i)
    {
      digitalWrite(_pstep, 1);
      ets_delay_us(_nus);
      digitalWrite(_pstep, 0);
      _position += incpos;
      ets_delay_us(_nus);
    }
  return _position;
		
}




int MoveAxis::move(int tostep)
{
  int nstep = tostep - _position;

  return rmove(nstep);
}
