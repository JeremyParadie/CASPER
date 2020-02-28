#include <Arduino.h>
#define powerPin 19

  void powerDown(){
      /**/
      //Setting to "High Impedence"
      pinMode(powerPin,OUTPUT);
      digitalWrite(powerPin,HIGH);
    }
  
