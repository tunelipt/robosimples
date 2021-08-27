/**
 * Basic ADC reading example.
 * - connects to ADC
 * - reads value from channel
 * - converts value to analog voltage
 */

#include "oem750.h"


#define PINO1        33   // Pino saída 1
#define PINO2        32   // Saída 2
#define PINO3        19   // Pino saida 3
#define PINO4        21   // Saída 4
#define PINO5        23   // Saída 5
#define PINO6        22   // Saída 6

const uint8_t pinos[] = {PINO1, PINO2, PINO3, PINO4, PINO5, PINO6};
const int NPINOS = 6;


            
  

MoveAxis Z(PINO1, PINO2, 39500, 19750); // Vel 20 mm/s
MoveAxis Y(PINO3, PINO4, 14070, 7035); // Vel 30 mm/s
MoveAxis X(PINO5, PINO6, 42150, 21075); // Vel 30 mm/s

MoveAxis *axes[] = {&Z, &Y, &X}; 
void setup() {

  
  Serial.begin(9600);
  X.set_position(0);
  delay(1000);
       
}

// Check if the string is a valid + or  integer
int checkstr(String s)
{
  int i1 = 0;
  char c = s[0];
  if (c != '+' && c != '-' && (c < '0' || c > '9') )
  {
    return 0;
  }
  
  for (int i = 1; i < s.length(); ++i)
  { 
    c = s[i];
    if (c < '0' || c > '9') return 0;
  }
  return 1;
}

/*
 # Communication protocol

 The command has the following structure, using regular expressions:
 
  * `[PMRAV][0-2](!|?)[+-]?[0-9]+`

 The following commands are possible
  * `P`: get/set postion
  * `M`: move to a position
  * `R`: Relative motion
  * `A`: Acceleration and deceleration ramp (pulses/s^2)
  * `V`: Terminal velocity (pulses/s)

 Each command is applied to an axis (0,1 or 2) and the command asks for 
 something(`?`) or sets something (`!`).
 
 A few examples will show better how this works:
   * `P0?` Get the position of axis 0
   * `M0!123` Move axis 0 to position 123
   * `R0!10` Move axis 0 10 units forward with respect to present position
   * `P1?` Get the position of axis 1
   * `M1!123` Move axis 1 to position 123
   * `R1!10` Move axis 1 10 units forward with respect to present position
   * `A2?` Read the acceleration of axis 2
   * `A2!1000` Set the acceleration of axis 2 to 1000 pulses/s^2
   * `V1?` Get the velocity of axis 1
   * `V2!12000` Set the velocity of axis 2 to 12000 pulses/s

  The functionality is basic but in the future the system should be able to move
  the 3 axes in lines simultaneously. Some emergency stop is important and the 
  ability to detect end of line sensors must be added.
  
 */
void loop() {
  char cmd;
  int query = 0;
  int axis = -1;
  String s;
  int len = 0;
  int val = 0;
  int resp = 0;
    
  if (Serial.available() > 0)
  {
    delay(20);  // Wait a bit to make sure to receiveto receive an entire line! 
    // Read the command character
    cmd = Serial.read();
    
    // For now, only move ('M') or relative move ('R') is accepted
    if (cmd != 'M' && cmd != 'R' && cmd != 'P' && cmd != 'V' && cmd != 'A')
    {
      Serial.print("ERR - Unknown command! --> ");
      Serial.println(cmd);
      
      delay(50);
      s = Serial.readString();
      return;
      
    }
    delay(20);
    s = Serial.readStringUntil('\n'); 
    s.trim();
    len = s.length();
    // Read the axis character
    if (s[0] == '0')
      axis = 0;
    else if (s[0] == '1')
      axis = 1;
    else if (s[0] == '2')
      axis = 2;
    else{
      Serial.print("ERR - Unavailable axis. It should be 1,2 or 3. Received: ");
      Serial.println(s[1]); 
      
      delay(50);
      s = Serial.readString();
      return;
    }

    // Read the query character
    if (s[1] == '!')
      query = 0;
    else if (s[1] == '?')
      query = 1;
    else{
      Serial.print("ERR - Unknown option. Expected ! or ? received: ");
      Serial.println(s[1]);
      
      delay(50);
      s = Serial.readString();
      return;
    }


    // If a query, return a value:
    if (query == 1)
    {
      if (cmd == 'A')
      {
        Serial.print("A");
        Serial.print(axis);
        Serial.print("?");
        Serial.println(axes[axis]->get_acceleration());
        return;
      }
      else if (cmd == 'V')
      {
        Serial.print("V");
        Serial.print(axis);
        Serial.print("?");
        Serial.println(axes[axis]->get_velocity());
        return;
      }
      else if (cmd == 'P')
      {
        Serial.print("P");
        Serial.print(axis);
        Serial.print("?");
        Serial.println(axes[axis]->get_position());
        return;
      }
      else
      {
        // It doesn't make much sense
        Serial.print(cmd);
        Serial.print(axis);
        Serial.println("?0");
      }
    }
    else
    {
      // Read the integer value:
      String snum = s.substring(2);
      if (!checkstr(snum)){
        Serial.print("ERR - The argument should be an integer. Received ");
        Serial.println(snum); 
      
        delay(50);
        s = Serial.readString();
        return;
      }
      int val = snum.toInt();
      int rval = 0;
      if (cmd=='A'){
         rval = axes[axis]->set_acceleration(val);
      }else if (cmd=='V'){
        rval = axes[axis]->set_velocity(val);
      }else if (cmd=='P'){
        rval = axes[axis]->set_position(val);
      }else if (cmd=='R'){
        rval = axes[axis]->rmove(val);
      }else if (cmd=='M'){
        rval = axes[axis]->move(val);
      }else{
        Serial.println("ERR - Don't know how we got here!");
          
        delay(50);
        s = Serial.readString();
        return;
      }
      // Build the response
      Serial.print(cmd);
      Serial.print(axis);
      Serial.print("!");
      Serial.println(rval);    
      return;
    }
  }

}     
 
