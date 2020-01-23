#include <Wire.h>
#include "src/Adafruit-PWM-Servo-Driver/Adafruit_PWMServoDriver.h"
#include "Arduino.h"     //Standard arduino library
#include "ServoWrapper.h"  //The header file for this class

ServoWrapper::ServoWrapper(){}

void ServoWrapper::initialize(){
  PCA9685.begin();
  PCA9685.setPWMFreq(60);  //update servos at 60hz
}

String ServoWrapper::clearErrors(){return "Not implemented yet";}

String ServoWrapper::getLastError(){return "Not implemented yet";}

String ServoWrapper::getErrorCount(){return "Not implemented yet";}

String ServoWrapper::getAngle(int pin){return "Not implemented yet";}

String ServoWrapper::setAngle(int pin,String angle){return "Not implemented yet";}

