#ifndef BatteryWrapper_h
#define BatteryWrapper_h

#include "src/SparkFun_BQ27441/SparkFunBQ27441.h"
#include "Arduino.h"                  //Standard arduino library
#include "FRAMWrapper.h"

class BatteryWrapper{                 //Wrapper class for the ____ library- this will make it much easier to work with the ____
  private:                            //Private- can't access these directly
    unsigned long errorCount=0;        //The number of errors since program start or since errors have been wiped
    String lastError="All good; No errors!";//The default error message, to be replaced with errors when they occur
    BQ27441 lipo;             //The BQ27441 object from the Adafruit library, which this class is wrapping. 
    int capacityValue;
    int minBatteryLevel;
    
  public:                             //Public- feel free to use these
    BatteryWrapper();                 //Constructor- gets called automatically when you make an object from this class
    void initialize(FRAMWrapper* fram);                //Call this once before you use any of the other functions below. Configures ____ settings.
    String clearErrors();             //Replaces the error with "All good; No errors!" and sets errorCount to 0.
    String getLastError();                //Returns the last error that occured (string)
    String getErrorCount();           //Returns the number of errors that have occured since the program start or since clearErrors() was called (string)
    String getVoltage();
    String getCharge();
    String getCurrent();
    String getPower();
    String getCapacity();
    String isCharging();
    String getHealth();
    String getMinimum();
    String setMaxCapacity(String capacity);
    String setMinimum(String minimum);
    
    
    
};

#endif
