#include <Wire.h>
#include "src/Adafruit-PWM-Servo-Driver/Adafruit_PWMServoDriver.h"
#include "Arduino.h"     //Standard arduino library
#include "ServoWrapper.h"  //The header file for this class

ServoWrapper::ServoWrapper(){}

void ServoWrapper::initialize(){
  PCA9685.begin();
  analogReference(AR_DEFAULT);
  analogRead(A3);
  analogRead(A3);
  analogRead(A3);//first few readings might not be accurate after changing the reference
  checkServoVoltage("initialize");
  checkForTransmissionErrors("initialize");
  PCA9685.setPWMFreq(60);  //update servos at 60hz
  checkForTransmissionErrors("initialize");
  for(int thisPin=0; thisPin<8; thisPin++){
    PCA9685.setPWM(thisPin, 0, 4096);
    checkForTransmissionErrors("initialize");
  }
}

String ServoWrapper::clearErrors(){//Replaces the error with "All good; No errors!" and sets errorCount to 0. Returns "OK"
  errorCount=0;                    //Clears errorCount to 0
  lastError="All good; No errors!";//Clears error to default text
  return "OK";
}

String ServoWrapper::getLastError(){//Returns the last error that occured (string)
  return lastError;
}

String ServoWrapper::getErrorCount(){ //Returns the number of errors that have occured since the program start or since clearErrors() was called (String)
  errorCount=errorCount%10000000;//use the remainder to limit the size of the number. The effect is that it'll rollover when it gets too big.
  String count=String(errorCount,DEC);//Convert from number to string
  return count;                       //Return the string
}

String ServoWrapper::getAngle(byte pin){
  checkServoVoltage("getAngle");
  if(angleOfServo[pin]==1000){return "Unknown angle";}
  return String(angleOfServo[pin])+char(248); //print <angle>Â°
}

String ServoWrapper::setAngle(byte pin,String angle){
  if(isDigit(angle.charAt(0))||angle.charAt(0)=='-') {//if first character is a digit (or a '-')
    int intAngle=angle.toInt();
    int pulseLength=map(intAngle,-90,90,pulseMin,pulseMax);//converts from degrees to pulse length. pulseMin and pulseMax are defined in ServoWrapper.h
    if(pulseLength<pulseMin){pulseLength=pulseMin; errorCount++;lastError="Warning: ServoWrapper.setAngle(); Angle exceeds minimum.";}
    if(pulseLength>pulseMax){pulseLength=pulseMax; errorCount++;lastError="Warning: ServoWrapper.setAngle(); Angle exceeds maximum.";}
    checkServoVoltage("setAngle");
    PCA9685.setPWM(pin,0,pulseLength);
    if(PCA9685.getLastStatus()==0){angleOfServo[pin]=int(map(pulseLength,pulseMin,pulseMax,-90,90));}//if transmission was successful, update the array with the current value
    return checkForTransmissionErrors("setAngle");
  }
  errorCount++;                   //increment the error count
  lastError=String("Error: ServoWrapper.setAngle(); Invalid parameter. Try '90")+char(248)+"'";//And generate the error
  return lastError;
}

String ServoWrapper::checkForTransmissionErrors(String functionName){
  byte status=PCA9685.getLastStatus();
  if(status==0){return "OK";}//I2C: success
  if(status==1){lastError="Error: ServoWrapper."+functionName+"(); I2C transmit overflow.";errorCount++;return lastError;}//I2C: Data too long to fit in transmit buffer
  if(status==2){lastError="Error: ServoWrapper."+functionName+"(); I2C address NACK.";errorCount++;return lastError;}//I2C: Received NACK on transmit of address
  if(status==3){lastError="Error: ServoWrapper."+functionName+"(); I2C data NACK.";errorCount++;return lastError;}//I2C: Received NACK on transmit of data
  if(status==4){lastError="Error: ServoWrapper."+functionName+"(); I2C error.";errorCount++;return lastError;}//I2C: Other error
}

String ServoWrapper::goLimp(byte pin){
  PCA9685.setPWM(pin, 0, 4096);
  if(PCA9685.getLastStatus()==0){angleOfServo[pin]=1000;}
  return checkForTransmissionErrors("goLimp");
}

String ServoWrapper::getVoltage(){
  return String(checkServoVoltage("getVoltage"),2)+'V';//convert analog input to a voltage, and multipy by 2 to undo the resistor voltage divider, then convert to string with 2 decimal places, and append a "V".
}

float ServoWrapper::checkServoVoltage(String functionName){
  float voltage=floatMap(analogRead(A3),0,1023,0,3.3)*3;
  if(voltage<4.5){errorCount++;lastError="Warning: ServoWrapper."+functionName+"(); Low servo voltage: "+String(voltage,2).concat('V');}
  if(voltage>6.5){errorCount++;lastError="Warning: ServoWrapper."+functionName+"(); High servo voltage: "+String(voltage,2).concat('V');}
  return voltage;
}

float ServoWrapper::floatMap(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

