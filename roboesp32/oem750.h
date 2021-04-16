

#ifndef _OEM750_H_
#define _OEM750_H_

class MoveAxis{
 public:
  MoveAxis(int pinstep, int pindir, int vel, int acc=0);

  int move(int tostep);
  int rmove(int nstep);
  
  int set_position(int pos=0);
  int get_position();
  
 private:
  int _position;     ///< Actual position
  int _pstep;        ///< Step pin
  int _pdir;         ///< Direction pin
  int _vel;          ///< Velocity steps/s
  int _acc;         ///< Acceleration steps/s^2
  int _nus;          ///< Number of us per step/2
};


#endif //_OEM750_H_
