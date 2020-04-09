#ifndef FRAMWrapper_h
#define FRAMWrapper_h

#include "src/Adafruit_FRAM_I2C/Adafruit_FRAM_I2C.h"          //Library for FRAM
#include <SPI.h>                      //Library for communicating via SPI. Reference: https://www.arduino.cc/en/Reference/SPI
#include <SD.h>                       //Library for SD card. Reference: https://www.arduino.cc/en/Reference/SD
#include "Arduino.h"                  //Standard arduino library

class FRAMWrapper{                 //Wrapper class for the ____ library- this will make it much easier to work with the ____
  private:                            //Private- can't access these directly
    unsigned long errorCount=0;        //The number of errors since program start or since errors have been wiped
    String lastError="All good; No errors!";//The default error message, to be replaced with errors when they occur
    Adafruit_FRAM_I2C MB85RC256V;             //The FRAM object from the Adafruit library, which this class is wrapping. 
    int readValue;
    uint16_t framAddr = 0;

    public:                             //Public- feel free to use these
    FRAMWrapper();                    //Constructor- gets called automatically when you make an object from this class
    void initialize();                //Call this once before you use any of the other functions below. Configures ____ settings.
    String clearErrors();             //Replaces the error with "All good; No errors!" and sets errorCount to 0.
    String getLastError();                //Returns the last error that occured (string)
    String getErrorCount();           //Returns the number of errors that have occured since the program start or since clearErrors() was called (string)
    void writeData(int address, uint16_t data); //Writes inputed data into FRAM
    String getData(String address);
    String clearData();
};

#endif
