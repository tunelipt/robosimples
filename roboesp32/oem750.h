

#ifndef _OEM750_H_
#define _OEM750_H_

#define MAX_VEL 50000
#define MAX_ACCEL 50000
 
class MoveAxis{
 public:
  MoveAxis(int pinstep, int pindir, int vel, int acc);

  int move(int tostep);
  int rmove(int nstep);
  
  int set_position(int pos=0);
  int get_position();

  int set_velocity(int vel);
  int get_velocity();

  int set_acceleration(int acc);
  int get_acceleration();
  
 private:
  int _position;     ///< Actual position
  int _pstep;        ///< Step pin
  int _pdir;         ///< Direction pin
  int _vel;          ///< Velocity steps/s
  int _acc;         ///< Acceleration steps/s^2
};


#endif //_OEM750_H_
