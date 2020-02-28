#ifndef GPSWrapper_h
#define GPSWrapper_h

#include "src/Adafruit_GPS/Adafruit_GPS.h"             //Library for GPS. Adafruit repository: https://github.com/adafruit/Adafruit_GPS
#include "Arduino.h"                  //Standard arduino library

class GPSWrapper{                     //Wrapper class for the GPS library- this will make it much easier to work with the GPS
  private:                            //Private- can't access these directly
    unsigned long errorCount=0;        //The number of errors since program start or since errors have been wiped
    String lastError="All good; No errors!";//The default error message, to be replaced with errors when they occur
    Adafruit_GPS GPS;                 //The GPS object from the GPS library, which this class is wrapping. 
    double convertCoordinate (float degMin, char dir);//internal function that converts lat/long from Adafruit degree-minute format to standard decimal-degrees
    void parse();                     //parses the next full sentence from the GPS if there is one.
    
  public:                             //Public- feel free to use these
    GPSWrapper();                     //Constructor- gets called automatically when you make an object from this class
    void initialize();                //Call this once before you use any of the other functions below. Configures GPS settings.
    String clearErrors();             //Replaces the error with "All good; No errors!" and sets errorCount to 0. Returns "OK"
    String getLastError();                //Returns the last error that occured (string)
    String getErrorCount();           //Returns the number of errors that have occured since the program start or since clearErrors() was called (string)
    void read();                      //Reads in a new char from GPS, if there is one. this needs to be called very often, around 1khz (a thousand times per second)
    String getLocation();             //If there is a fix, returns the location in decimal degrees lattitude and longitude. if there isnt a fix, returns "Waiting for fix"
    String getAltitude();             //If there is a fix, returns the altitude in meters. example: "76.5 meters" if there isnt a fix, returns "Waiting for fix"
    String getTime();                 //Returns the time in â€œHH:MM:SS.SSS UTCâ€� Time is UTC, not timezone or daylight saving offset.
    String getDate();                 //Returns date in â€œYYYY-MM-DDâ€� format
    String getFix();                  //Returns "No fix" "Bad fix" "Good fix" or "Great fix" based on fix and quality
    String getSatelliteCount();       //Returns the number of satellites available in a string
};

#endif

