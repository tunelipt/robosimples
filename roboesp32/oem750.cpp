

#define __TESTE_COMP__



#include <cstdint>
#include <math.h>

#ifdef __TESTANDO__

#include "myarduino.h"

#else

#include "Arduino.h"
#include <rom/ets_sys.h>

#endif


#include "oem750.h"




MoveAxis::MoveAxis(int pinstep, int pindir, int vel, int acc):
  _position(0), _pstep(pinstep), _pdir(pindir),
  _vel(vel), _acc(acc)
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

int MoveAxis::get_velocity()
{
  return _vel;
}

int MoveAxis::set_velocity(int vel)
{
  if (vel <= 0 || vel > MAX_VEL)
    return -1;
    
  _vel = vel;

  return _vel;
}

int MoveAxis::get_acceleration()
{
  return _acc;
}

int MoveAxis::set_acceleration(int acc)
{
  if (acc <= 0 || acc > MAX_VEL)
    return -1;
    
  _acc = acc;

  return _acc;
}

int MoveAxis::rmove(int nstep)
{
  int dir = 0;
  
  if (nstep < 0)
    {
      nstep = -nstep;
      dir = 1;
    }

  // Plan the motion
  int X1=0, X2=0, X3=0;
  float dt = sqrt(1.0  / (8*_acc));

  X1 = (int) round(_vel*_vel / (2*_acc) );
  
  if (nstep <= 2*X1){
    X3 = nstep / 2;
    X2 = 0;
    X1 = nstep - X3;
  }else{
    X3 = X1;
    X2 = nstep - X3 - X1;
  }
  
  // Calculate half of the time interval per timestep:
  int incpos = (dir==0) ? 1 : -1;
  
  
  // Setting direction:
  int us; 
  int t1, t2;
  
  digitalWrite(_pdir, dir);
  for (int i = 0; i < X1; ++i)
    {
      digitalWrite(_pstep, 1);
      t1 = micros();
      us = (int) round(1e6 * dt); // us / (1/2 step)
      dt = dt*(1.0 - (4*_acc)*dt*dt); // Calculating for next step
      _position += incpos;  // Increment position
      t2 = micros();
      ets_delay_us(us-(t2-t1));
      
      digitalWrite(_pstep, 0);
      ets_delay_us(us);
    }

  for (int i = 0; i < X2; ++i)
    {
      digitalWrite(_pstep, 1);
      _position += incpos;  // Increment position
      ets_delay_us(us);
      
      digitalWrite(_pstep, 0);
      ets_delay_us(us);
    }

  for (int i = 0; i < X3; ++i)
    {
      digitalWrite(_pstep, 1);
      t1 = micros();
      us = (int) round(1e6 * dt); // us / (1/2 step)
      dt = dt*(1.0 + (4*_acc)*dt*dt); // Calculating for next step
      _position += incpos;  // Increment position
      t2 = micros();
      ets_delay_us(us-(t2-t1));
      
      digitalWrite(_pstep, 0);
      ets_delay_us(us);
    }
  
  return _position;
		
}




int MoveAxis::move(int tostep)
{
  int nstep = tostep - _position;

  return rmove(nstep);
}
