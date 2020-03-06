#include "src/Adafruit_GPS/Adafruit_GPS.h"//Library for GPS. Adafruit repository: https://github.com/adafruit/Adafruit_GPS
#include "Arduino.h"     //Standard arduino library
#include "GPSWrapper.h"  //The header file for this class
#include <math.h>;       //required for fmod() in cordinate conversion

GPSWrapper::GPSWrapper() : GPS(&Serial1) {}//Constructor- gets called automatically when you make an object from this class. Creates a library object for our GPS module.

void GPSWrapper::initialize(){//Call this once before you use any of the other functions below. Configures GPS settings. 
  GPS.begin(9600);// 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);  //turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  Serial1.println(PMTK_Q_RELEASE);          //request firmware response
  if(!Serial1.find("PMTK")){                //look for firmware response. If GPS is not responding to firmware request, initialization failed. Is it plugged in?
    errorCount++;                 //increment the error count
    lastError="Error: GPSWrapper.initialize(); Initialization failed.";//And generate the error
  }
}

String GPSWrapper::clearErrors(){  //Replaces the error with "All good; No errors!" and sets errorCount to 0. Returns "OK"
  errorCount=0;                    //Clears errorCount to 0
  lastError="All good; No errors!";//Clears error to default text
  return "OK";
}

String GPSWrapper::getLastError(){//Returns the last error that occured (string)
  return lastError;
}

String GPSWrapper::getErrorCount(){ //Returns the number of errors that have occured since the program start or since clearErrors() was called (String)
  errorCount=errorCount%10000000;//use the remainder to limit the size of the number. The effect is that it'll rollover when it gets too big.
  String count=String(errorCount,DEC);//Convert from number to string
  return count;                       //Return the string
}

double GPSWrapper::convertCoordinate(float degMin, char dir) {  //converts lat/long from Adafruit degree-minute format to standard decimal-degrees
  parse();//looks for new sentence from the GPS and parses it
  double min = fmod((double)degMin, 100.0);     //get the minutes, fmod() requires double
  degMin = (int) ( degMin / 100 );              //rebuild coordinates in decimal degrees
  double decDeg = degMin + ( min / 60 );        //note: convertCoordinate() is a modified function from http://arduinodev.woofex.net/2013/02/06/adafruit_gps_forma/
  if((dir=='S')||(dir=='W')){decDeg=decDeg*-1;} //if direction is south or west, make negative
  return decDeg;                                //return converted coordinate
}

void GPSWrapper::parse(){           //parses the next full sentence from the GPS if there is one
  if (GPS.newNMEAreceived()){       //if a sentence is received, 
    if (!GPS.parse(GPS.lastNMEA())){//parse it. This also sets the newNMEAreceived() flag to false
      errorCount++;                 //If parse failed, increment the error count
      lastError="Error: GPSWrapper.parse(); Parse failed.";//And generate the error
    }
  }
}

void GPSWrapper::read(){//reads in a new char from GPS, if there is one. This needs to be called very often, around 1khz (a thousand times per second)
  GPS.read(); //read another character from the GPS
}

String GPSWrapper::getLocation(){//if there is a fix, returns the location in decimal degrees lattitude and longitude. if there isnt a fix, returns "Waiting for fix"
  parse();//looks for new sentence from the GPS and parses it
  if((((int)GPS.fix)==0)||(((int)GPS.fixquality)==0)){return "Waiting for fix";}//if there is no fix say so
    else{return String(convertCoordinate(GPS.latitude,GPS.lat),6)+String(", ")+String(convertCoordinate(GPS.longitude,GPS.lon),6);}//otherwise return the formatted location
}

String GPSWrapper::getAltitude(){//If there is a fix, returns the altitude in meters. example: "76.5 meters" if there isnt a fix, returns "Waiting for fix"
  parse();//looks for new sentence from the GPS and parses it
  if((((int)GPS.fix)==0)||(((int)GPS.fixquality)==0)){return "Waiting for fix";}
    else{return String(GPS.altitude,1)+String(" meters");}
}

String GPSWrapper::getTime(){                   //returns the time in "HH:MM:SS.SSS UTC" Time is UTC, not timezone or daylight saving offset.
  parse();//looks for new sentence from the GPS and parses it
  String Time="";                               //make a blank string
  if(GPS.hour<10){Time.concat("0");}            //add a "0" if there is less than 10 hours
  Time.concat(GPS.hour);                        //add hours
  Time.concat(":");                             //add colon
  if(GPS.minute<10){Time.concat("0");}          //add a "0" if there is less than 10 minutes
  Time.concat(GPS.minute);                      //add minutes
  Time.concat(":");                             //add colon
  if(GPS.seconds<10){Time.concat("0");}         //add a "0" if there is less than 10 seconds
  Time.concat(GPS.seconds);                     //add seconds
  Time.concat(".");                             //add decimal point
  if(GPS.milliseconds<100){Time.concat("0");}   //add a "0" if less than 100 milliseconds
  if(GPS.milliseconds<10){Time.concat("0");}    //add a "0" if less than 10 milliseconds
  Time.concat(GPS.milliseconds);                //add miliseconds
  Time.concat(" UTC");                          //add " UTC"
  return Time;                                  //return formatted time
}

String GPSWrapper::getDate(){                   //returns date in "YYYY-MM-DD" format
  parse();//looks for new sentence from the GPS and parses it
  return String("20")+String(GPS.year)+String("-")+String(GPS.month)+String("-")+String(GPS.day); //return date in "YYYY-MM-DD" format
}

String GPSWrapper::getFix(){                                            //returns "No fix" "Bad fix" "Good fix" or "Great fix" based on fix and quality
  parse();//looks for new sentence from the GPS and parses it
  String Fix="No fix";                                                  //default to no fix
  if((((int)GPS.fix)==1)&&(((int)GPS.fixquality)==0)){Fix="Bad fix";}   //if fix and low quality- bad fix
  if((((int)GPS.fix)==1)&&(((int)GPS.fixquality)==1)){Fix="Good fix";}  //if fix and ok quality- good fix
  if((((int)GPS.fix)==1)&&(((int)GPS.fixquality)==2)){Fix="Great fix";} //if fix and high quality- great fix
  return Fix;                                                           //return fix
}

String GPSWrapper::getSatelliteCount(){     //returns the number of satellites available in a string
  parse();//looks for new sentence from the GPS and parses it
  return String(GPS.satellites);          //return the number of satellites available in a string
}

