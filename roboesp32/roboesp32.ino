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

MoveAxis X(PINO1, PINO2, 39500, 19750); // Vel 20 mm/s
//MoveAxis Y(PINO3, PINO4, 14070, 4690); // Vel 30 mm/s
//MoveAxis X(PINO5, PINO6, 42150, 1450); // Vel 30 mm/s


void setup() {

  
  Serial.begin(9600);
  X.set_position(0);
  delay(1000);
       
}

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

void loop() {
  int cmd;
  String s;
  int len = 0;
  int val = 0;
  int resp = 0;
    
  if (Serial.available() > 0)
  {
    //delay(200);  // Wait a bit to make sure to receiveto receive an entire line! 
    
    cmd = Serial.read();
    // For now, only move ('M') or relative move ('R') is accepted
    if (cmd != 'M' && cmd != 'R' && cmd != 'P')
    {
      Serial.print("ERR - Unknown command! -->");
      delay(50);
      s = Serial.readString();
      return;
      
    }
    s = Serial.readStringUntil('\n');
    s.trim();
    len = s.length();
    if (cmd=='P' && len==0){
      Serial.print("V:"); 
      Serial.println(Z.get_position());
      return;
    }
    
    if (!checkstr(s))
      {
        Serial.println("ERR - Invalid command. Integer expected");
      }
    
    val = s.toInt();
    
    if (cmd=='P'){
      resp = X.set_position(val);
    }else if (cmd=='R'){
      resp = X.rmove(val);
    }else if (cmd=='M'){
      resp = X.move(val);
    }

    Serial.print("V:");
    Serial.println(resp);

    return;
  }

}     
 
