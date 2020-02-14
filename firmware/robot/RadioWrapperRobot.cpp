#include "src/SFE_RFM69/RFM69.h"       //Library for radio. SparkFun repository: https://github.com/sparkfun/RFM69HCW_Breakout
#include <SPI.h>         //Library for communicating via SPI. Reference: https://www.arduino.cc/en/Reference/SPI
#include "Arduino.h"     //Standard arduino library
#include "RadioWrapperRobot.h"//The header file for this class

#define RCS      8       //RFM69 slave select pin
#define RINT     3       //RFM69 interrupt pin
#define RHW      true    //is it a RFM69HW?
#define RINUM    3       //RFM69 interrupt number

RadioWrapperRobot::RadioWrapperRobot() : RFM69HCW(RCS,RINT,RHW,RINUM) {}//Constructor- gets called automatically when you make an object from this class. Creates a library object for our RFM69HCW module.

void RadioWrapperRobot::initialize(){//Call this once before you use any of the other functions below. Configures radio settings.
  pinMode(LED,OUTPUT);            //Set up the indicator LED
  digitalWrite(LED,LOW);          //Set up the indicator LED
  if(RFM69HCW.initialize(FREQUENCY, MYNODEID, NETWORKID)==true){ //Initialize the RFM69HCW
    RFM69HCW.setHighPower();      //Always use this for RFM69HCW, the high power module
    RFM69HCW.encrypt(ENCRYPTKEY); //Turn on encryption
    RFM69HCW.setPowerLevel(31);   //Set power to max 31
    return;                       //return 
  }
  errorCount++;                   //If initialization failed, increment the error count
  lastError="Error: RadioWrapper.initialize(); Initialization failed.";//And generate the error
  return;                         //return
}

String RadioWrapperRobot::clearErrors(){  //Replaces the error with "All good; No errors!" and sets errorCount to 0. Returns "OK"
  errorCount=0;                    //Clears errorCount to 0
  lastError="All good; No errors!";//Clears error to default text
  return "OK";
}

String RadioWrapperRobot::getLastError(){//Returns the last error that occured (string)
  return lastError;             //Return the error
}

String RadioWrapperRobot::getErrorCount(){//Returns the number of errors that have occured since the program start or since clearErrors() was called (String)
  errorCount=errorCount%10000000;//use the remainder to limit the size of the number. The effect is that it'll rollover when it gets too big.
  String count=String(errorCount,DEC);//Convert from number to string
  return count;//Return the string
}

String RadioWrapperRobot::getSignalStrength(){//Returns the signal strength expressed as a percent- 0% is a poor signal and 100% is a strong signal. Value updates when a new packet is received during receive(). 
  int value=RSSI;                      //Get saved received signal strength indication from last recieve
  value=map(value,-100,-25,0,100);     //Map the number to a percent signal strength
  value=constrain(value, 0, 100);      //Constrain the value to be within 100%
  String percent=String(value,DEC);    //Convert percent to string
  percent.concat("%");                 //Add percent character
  return percent;                      //Return the string
}

void RadioWrapperRobot::send(String data){   //Sends the passesd string parameter over the radio as a packet. Must be 61 or less characters.
  if(data.length()>61){                 //Making sure there are 61 characters or less
    errorCount++;                       //If there are too many characters, increment the error count
    lastError="Error: RadioWrapper.send(); Packet too long, not sent.";//And generate the error
  }else{                                                               //Otherwise
    char sendbuffer[data.length()+1];            //Make a character array (62 chars max)
    data.toCharArray(sendbuffer,data.length()+1);//Transfer 62 chars max into sendbuffer
    sendbuffer[data.length()]='\0';              //Set last element, at index 61, (the 62nd element) to null terminator
    if(RFM69HCW.sendWithRetry(TONODEID, sendbuffer, data.length()+1)){//Send the packet. If successfully received ack,
    }else{         //Otherwise
      errorCount++;//No ack was recieved, so increment the error count
      lastError="Error: RadioWrapper.send(); Packet sent, no acknowledgement.";//And generate the error
    }
  }
}

String RadioWrapperRobot::receive(){         //Returns a newly received packet, if there is one. If not, returns "No Data Received"
  String data;                               //Make a string
  if(RFM69HCW.receiveDone()){                //If there is a packet
    for (byte i=0; i<RFM69HCW.DATALEN; i++){ //The actual message is contained in the DATA array, and is DATALEN bytes in size
      if((char)RFM69HCW.DATA[i]!='\0'){ //if the character is null, ignore it
        data.concat((char)RFM69HCW.DATA[i]);   //Iterate through the received packet, transferring it to the string character by character
      }
    }
    RSSI=RFM69HCW.RSSI;     //Save the received signal strength indication for later
    RFM69HCW.sendACK();     //Send an ACKnowledgement
  }else{                    //Otherwise, if there was not a packet,
    data="No Data Received";//Set the data to "No Data Received"
  }
  return data;//Return the string
}

String RadioWrapperRobot::getTemperature(){//Returns the temperature in a string formatted like "30Â°C"  Is not very accurate- just gives an estimate
  String temp=String(RFM69HCW.readTemperature(TEMPOFFSET),DEC);//Read the temperature from the radio's temp sensor, and put it into a string as a decimal number
  temp.concat(String(char(248))+"C"); //Tack on a Â°C at the end
  return temp;        //Return the string
}

void RadioWrapperRobot::goToSleep(){//Puts the radio in a low power state. Call wakeFromSleep() to turn it back on
  RFM69HCW.sleep();
}

void RadioWrapperRobot::wakeFromSleep(){//Wakes the radio from sleep after calling goToSleep()
  RFM69HCW.receiveDone();
}


