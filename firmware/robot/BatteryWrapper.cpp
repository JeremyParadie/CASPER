#include "src/SparkFun_BQ27441/SparkFunBQ27441.h"
#include <Wire.h>
#include "Arduino.h"     //Standard arduino library
#include "BatteryWrapper.h"  //The header file for this class
#include "FRAMWrapper.h"

BatteryWrapper::BatteryWrapper() : lipo(){}
void BatteryWrapper::initialize(FRAMWrapper* fram){
  if (lipo.begin() == 1) // begin() will return true if communication is successful
  {
 lipo.setCapacity(2500); //Our default capacity value in mAh
 return;                                         //return
  }
  errorCount++;                   //If initialization failed, increment the error count
  lastError="Error: BatteryWrapper.initialize(); Initialization failed.";//And generate the error
  return;   
  }
  
String BatteryWrapper::clearErrors(){
  errorCount=0;                    //Clears errorCount to 0
  lastError="All good; No errors!";//Clears error to default text
  return "OK";
  }
String BatteryWrapper::getLastError(){
  return lastError;
  }
String BatteryWrapper::getErrorCount(){
  errorCount=errorCount%10000000;//use the remainder to limit the size of the number. The effect is that it'll rollover when it gets too big.
  String count=String(errorCount,DEC);//Convert from number to string
  return count;                       //Return the string
}
  
String BatteryWrapper::getVoltage(){return String(double(lipo.voltage())/1000) + " V";}
String BatteryWrapper::getCharge(){return String(lipo.soc())+"%";}
String BatteryWrapper::getCurrent(){return String(double(lipo.current()*-1)) + " mA";}
String BatteryWrapper::getPower(){return String(double(lipo.power()*-1)) + " mW";}
String BatteryWrapper::getCapacity(){return String(lipo.capacity(REMAIN)) + " of " + String(lipo.capacity(FULL)) + " mAh remaining";}
String BatteryWrapper::getMinimum(){return String(minBatteryLevel) + "%";}
String BatteryWrapper::getHealth(){return String(lipo.soh())+"%";}
String BatteryWrapper::isCharging(){return "Not implemented yet";}
String BatteryWrapper::setMaxCapacity(String capacity){
    if(isDigit(capacity.charAt(0))){//if first character is a digit
    long cap = capacity.toInt();         //convert to int
    if(cap > 500){ //just to ensure we don't break the babysitter right now 
      unsigned int capacityValue = cap;              //store for later
      unsigned writeAddress = 10;
      //Fram.writeData(writeAddress,capacityValue);
      lipo.setCapacity(cap);  //According to the instructions online, there may be a 1-2 second delay for the changes to occur
      return String("OK");
  }
    }
  errorCount++;                   //increment the error count
  lastError="Please type in an INTEGER above 500 mAh";//And generate the error
  return lastError;
}
String BatteryWrapper::setMinimum(String minimum){
    if(isDigit(minimum.charAt(0))){//if first character is a digit
      int minbat = minimum.toInt();         //convert to int
      if((minbat < 90) && (minbat > 5)){ //just to ensure we don't break the babysitter right now 
        minBatteryLevel = minbat;              //store for later
        return String("OK");
  }
    }
  errorCount++;                   //increment the error count
  lastError="Please type in an INTEGER between 5 and 90%";//And generate the error
  return lastError;
}
