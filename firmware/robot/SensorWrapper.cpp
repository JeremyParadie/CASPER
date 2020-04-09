#include "Arduino.h"     //Standard arduino library
#include "SensorWrapper.h"  //The header file for this class
#include "src/SparkFun_BME280/SparkFunBME280.h"


SensorWrapper::SensorWrapper(): Sensor(){}

void SensorWrapper::initialize(){
  if (Sensor.beginI2C() == true){return;} //Begin communication over I2C
  errorCount++;                   //If initialization failed, increment the error count
  lastError="Error: SensorWrapper.initialize(); Initialization failed."; //And generate the error
  return;
}
String SensorWrapper::clearErrors(){
  errorCount=0;                    //Clears errorCount to 0
  lastError="All good; No errors!";//Clears error to default text
  return "OK";}
String SensorWrapper::getLastError(){
  return lastError;}
String SensorWrapper::getErrorCount(){
  errorCount=errorCount%10000000;//use the remainder to limit the size of the number. The effect is that it'll rollover when it gets too big.
  String count=String(errorCount,DEC);//Convert from number to string
  return count;                       //Return the string
}
String SensorWrapper::getTemperature(){return String(Sensor.readTempF())+" F";}
String SensorWrapper::getPressure(){return String(Sensor.readFloatPressure()*0.00750062)+" mmHg";}
String SensorWrapper::getHumidity(){return String(Sensor.readFloatHumidity())+"%";}
