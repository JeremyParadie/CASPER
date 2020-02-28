#include "src/Adafruit_FRAM_I2C/Adafruit_FRAM_I2C.h"
#include "Arduino.h"                  //Standard arduino library
#include "FRAMWrapper.h"  //The header file for this class

FRAMWrapper::FRAMWrapper() : MB85RC256V() {}
void FRAMWrapper::initialize() {
  if (MB85RC256V.begin(0x50)) // begin() will return true if communication is successful
  {
    //uint8_t test = MB85RC256V.read8(0x0);
    return;
  }                                         //return
  errorCount++;                   //If initialization failed, increment the error count
  lastError = "Error: FRAMWrapper.initialize(); Initialization failed."; //And generate the error
  return;
}

String FRAMWrapper::clearErrors() {
  errorCount = 0;                  //Clears errorCount to 0
  lastError = "All good; No errors!"; //Clears error to default text
  return "OK";
}
String FRAMWrapper::getLastError() {
  return lastError;
}
String FRAMWrapper::getErrorCount() {
  errorCount = errorCount % 10000000; //use the remainder to limit the size of the number. The effect is that it'll rollover when it gets too big.
  String count = String(errorCount, DEC); //Convert from number to string
  return count;                       //Return the string
}

void FRAMWrapper::writeData(int address, uint16_t data){
  address *= 2;
  uint8_t MSB = (data & 0xFF00) >> 8;    // Break the value into 2 Byte-parts for storing
  uint8_t LSB = (data & 0x00FF) >> 0;     
  MB85RC256V.write8(address, MSB);              // Store the value MSB at address add1
  MB85RC256V.write8(address + 1, LSB);          // Store the value LSB at address add1 + 1
  return;
}

String FRAMWrapper::getData(String address) {
  unsigned int add = address.toInt()*2;
  unsigned int MSB = MB85RC256V.read8(add);           //Read the 2 bytes from memory
  unsigned int LSB = MB85RC256V.read8(add+1);
  readValue = (256 * MSB) + LSB;
  return String(readValue);
}

String FRAMWrapper::clearData() {
  uint8_t value;
      for (uint16_t a = 0; a < 32768; a++) {
       value = MB85RC256V.read8(a);
       if ((a % 32) == 0) {
         Serial.print("\n 0x"); Serial.print(a, HEX); Serial.print(": ");
       }
       Serial.print("0x");
       if (value < 0x1)
         Serial.print('0');
       Serial.print(value, HEX); Serial.print(" ");
      }
  return "Memory Cleared";
}
