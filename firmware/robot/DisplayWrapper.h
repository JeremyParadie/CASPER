#ifndef DisplayWrapper_h
#define DisplayWrapper_h

#include "Arduino.h"                  //Standard arduino library

class DisplayWrapper{                 //Wrapper class for the ____ library- this will make it much easier to work with the ____
  private:                            //Private- can't access these directly
    int errorCount=0;                 //The number of errors since program start or since errors have been wiped
    String lastError="All good; No errors!";//The default error message, to be replaced with errors when they occur
    //Adafruit_____ ____;             //The ____ object from the ____ library, which this class is wrapping. 

  public:                             //Public- feel free to use these
    DisplayWrapper();                 //Constructor- gets called automatically when you make an object from this class
    void initialize();                //Call this once before you use any of the other functions below. Configures ____ settings.
    String clearErrors();             //Replaces the error with "All good; No errors!" and sets errorCount to 0.
    String getLastError();                //Returns the last error that occured (string)
    String getErrorCount();           //Returns the number of errors that have occured since the program start or since clearErrors() was called (string)
    String clear();
    String turnOn();
    String turnOff();
    String getState();
    String getText(int line);
    String setText(int lint, String text);
};

#endif
