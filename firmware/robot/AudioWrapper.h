#ifndef AudioWrapper_h
#define AudioWrapper_h

#include "src/Adafruit_VS1053/Adafruit_VS1053.h"          //Library for audio. Adafruit repository: https://github.com/adafruit/Adafruit_VS1053_Library
#include <SPI.h>                      //Library for communicating via SPI. Reference: https://www.arduino.cc/en/Reference/SPI
#include <SD.h>                       //Library for SD card. Reference: https://www.arduino.cc/en/Reference/SD
#include "Arduino.h"                  //Standard arduino library

class AudioWrapper{                   //Wrapper class for the VS1053 library- this will make it much easier to work with audio
  private:                            //Private- can't access these directly
    unsigned long errorCount=0;        //The number of errors since program start or since errors have been wiped
    String lastError="All good; No errors!";//The default error message, to be replaced with errors when they occur
    Adafruit_VS1053_FilePlayer VS1053b;//The audio object from the VS1053 library, which this class is wrapping. 
    int volumePercent;
     
  public:                             //Public- feel free to use these
    AudioWrapper();                   //Constructor- gets called automatically when you make an object from this class
    void initialize();                //Call this once before you use any of the other functions below. Configures audio settings.
    String clearErrors();             //Replaces the error with "All good; No errors!" and sets errorCount to 0. Returns "OK"
    String getLastError();                //Returns the last error that occured (string)
    String getErrorCount();           //Returns the number of errors that have occured since the program start or since clearErrors() was called (string)
    String beep();                    //Generate a diagnostic beep from the speaker. Returns "OK"
    String setVolume(String volume);  //Volume is expressed as a percent in a string. The first characters must be an integer number between 0 and 100, higher numbers being louder volume. example: "85%" If first character is not a number an error will be generated. Returns "OK" or an error
    String getVolume();
    String play(String filename);     //Starts playing the MP3 file specified from the SD card. May be called if audio is stopped. Returns "OK" or error
    String isPlaying();               //Returns "true" if audio is currently playing and "false" if it is not
    String pause();                   //Pauses audio. May be called if audio is playing. Returns "OK" or error
    String resume();                  //Resumes audio. May be called if audio is paused. Returns "OK" or error
    String isPaused();                //Returns "true" when paused and "false" when not paused
    String stop();                    //Stops audio. May be called if audio is playing. Returns "OK" or error
    String isStopped();               //Returns "true" when stopped and "false" when not stopped
    String getLastFileName();         //Returns a string with the filename of the last audio file passed to the play function in format "track001.mp3"
};

#endif
