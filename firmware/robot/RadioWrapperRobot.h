#ifndef RadioWrapperRobot_h
#define RadioWrapperRobot_h

#include "src/SFE_RFM69/RFM69.h"                    //Library for radio. SparkFun repository: https://github.com/sparkfun/RFM69HCW_Breakout
#include <SPI.h>                      //Library for communicating via SPI. Reference: https://www.arduino.cc/en/Reference/SPI
#include "Arduino.h"                  //Standard arduino library

class RadioWrapperRobot{              //Wrapper class for the rfm69 library- this will make it much easier to work with the radio
  private:                            //Private- can't access these directly
    const int NETWORKID=0;            //Must be the same for all nodes (0 to 255)
    const int MYNODEID=2;             //My node ID (0 to 255) (robot radio is 2)
    const int TONODEID=1;             //Destination node ID (0 to 254, 255 = broadcast) (relay radio is 1)
    const int FREQUENCY=RF69_915MHZ;  //Set wireless frequency
    const char ENCRYPTKEY[17]={'T','o','p','S','e','c','r','e','t','P','a','s','s','w','r','d','\0'};//Use the same 16-byte key on all nodes. AES encryption
    const byte TEMPOFFSET=0;          //Change this value to calibrate the temperature sensor on the radio
    const int LED=13;                 //Packet sent/received indicator LED positive pin
    unsigned long errorCount=0;        //The number of errors since program start or since errors have been cleared
    String lastError="All good; No errors!";//The default error message, to be replaced with errors when they occur
    int RSSI=-100;                    //The signal strength expressed between -100 (poor signal) to -25 (strong signal)
    RFM69 RFM69HCW;                   //The radio object from the RFM69 library, which this class is wrapping. 
    
  public:                             //Public- feel free to use these
    RadioWrapperRobot();              //Constructor- gets called automatically when you make an object from this class
    void initialize();                //Call this once before you use any of the other functions below. Configures radio settings. 
    String clearErrors();             //Replaces the error with "All good; No errors!" and sets errorCount to 0. Returns "OK"
    String getLastError();                //Returns the last error that occured (string)
    String getErrorCount();           //Returns the number of errors that have occured since the program start or since clearErrors() was called (string)
    String getSignalStrength();       //Returns the signal strength expressed as a percent- "0%" is a poor signal and "100%" is a strong signal. Value updates when a new packet is received during receive(). 
    void send(String data);           //Sends the passesd string parameter over the radio as a packet. Must be 61 or less characters.
    String receive();                 //Returns a newly received packet, if there is one, and blinks LED pin. If not, returns "No Data Received"
    String getTemperature();          //Returns the temperature in a string formatted like "30Â°C"  Is not very accurate- just gives an estimate
    void goToSleep();                 //Puts the radio in a low power state. Call wakeFromSleep() to turn it back on
    void wakeFromSleep();             //Wakes the radio from sleep some time after calling goToSleep()
};

#endif

