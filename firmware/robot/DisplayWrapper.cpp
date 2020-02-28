#include "src/Adafruit_OLED/Adafruit_GFX.h" //Library for display graphics. Adafruit repository: https://github.com/adafruit/Adafruit-GFX-Library
#include "src/Adafruit_OLED/Adafruit_SSD1306.h" //Library for display hardware. Adafruit repository: https://github.com/adafruit/Adafruit_SSD1306
#include <SPI.h>          //Library for communicating via SPI. Reference: https://www.arduino.cc/en/Reference/SPI
#include <Wire.h>
#include "Arduino.h"     //Standard arduino library
#include "DisplayWrapper.h"  //The header file for this class
#include "Bitmaps.h"      //The header file that stores the bitmaps for the icons and squirrel image

DisplayWrapper::DisplayWrapper() : OLED(128, 32) {}//Constructor- gets called automatically when you make an object from this class. Creates a library object for our OLED module.

void DisplayWrapper::initialize(){
  if(OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, true)) { //initialise the display to: use internal step-up for VCC, 128x32, don't reset, initialize I2C. Returns true if sucessful. Allocates RAM for image buffer, initializes peripherals and pins.
    OLED.clearDisplay();
    OLED.drawBitmap(4,0,SQUIRRELIMAGE, SQUIRREL_WIDTH, SQUIRREL_HEIGHT, WHITE);//bitmaps, widths, and heights are stored in Bitmaps.h
    OLED.display();           //show the squirrel splash screen
    OLED.setTextSize(1);      // Normal 1:1 pixel scale
    OLED.setTextColor(WHITE); // Draw white text
    OLED.setTextWrap(false);  //do not automatically wrap to a new line
    OLED.cp437(true);         // Use full 256 char 'Code Page 437' character set
    return;                   //return if sucessful
  }
  errorCount++;               //If initialization failed, increment the error count
  lastError="Error: DisplayWrapper.initialize(); Initialization failed.";//And generate the error
  return;    
}

String DisplayWrapper::clearErrors(){//Replaces the error with "All good; No errors!" and sets errorCount to 0. Returns "OK"
  errorCount=0;                    //Clears errorCount to 0
  lastError="All good; No errors!";//Clears error to default text
  return "OK";
}

String DisplayWrapper::getLastError(){//Returns the last error that occured (string)
  return lastError;
}

String DisplayWrapper::getErrorCount(){
  errorCount=errorCount%10000000;//use the remainder to limit the size of the number. The effect is that it'll rollover when it gets too big.
  String count=String(errorCount,DEC);//Convert from number to string
  return count;                       //Return the string
}

String DisplayWrapper::getUpdateCount(){
  updateCount=updateCount%10000000;//use the remainder to limit the size of the number. The effect is that it'll rollover when it gets too big.
  String count=String(updateCount,DEC);//Convert from number to string
  return count;                       //Return the string
}

String DisplayWrapper::showAll(){
  showingTitle=true;
  showingLoopCount=true;
  showingCommandCount=true;
  showingTotalErrorCount=true;
  showingLine0=true;
  showingLine1=true;
  showingLine2=true;
  showingLine3=true;
  showingSignal=true;
  showingBattery=true;
  showingVolume=true;
  showingTimeout=true;
  return "OK";
}

String DisplayWrapper::showTitle(){
  showingTitle=true;
  return "OK";
}

String DisplayWrapper::showLoopCount(){
  showingLoopCount=true;
  return "OK";
}

String DisplayWrapper::showCommandCount(){
  showingCommandCount=true;
  return "OK";
}

String DisplayWrapper::showTotalErrorCount(){
  showingTotalErrorCount=true;
  return "OK";
}

String DisplayWrapper::showLine0(){
  showingLine0=true;
  return "OK";
}

String DisplayWrapper::showLine1(){
  showingLine1=true;
  return "OK";
}

String DisplayWrapper::showLine2(){
  showingLine2=true;
  return "OK";
}

String DisplayWrapper::showLine3(){
  showingLine3=true;
  return "OK";
}

String DisplayWrapper::showSignal(){
  showingSignal=true;
  return "OK";
}

String DisplayWrapper::showBattery(){
  showingBattery=true;
  return "OK";
}

String DisplayWrapper::showVolume(){
  showingVolume=true;
  return "OK";
}

String DisplayWrapper::showTimeout(){
  showingTimeout=true;
  return "OK";
}

String DisplayWrapper::hideAll(){
  showingTitle=false;
  showingLoopCount=false;
  showingCommandCount=false;
  showingTotalErrorCount=false;
  showingLine0=false;
  showingLine1=false;
  showingLine2=false;
  showingLine3=false;
  showingSignal=false;
  showingBattery=false;
  showingVolume=false;
  showingTimeout=false;
  OLED.clearDisplay();
  return "OK";
}

String DisplayWrapper::hideTitle(){
  showingTitle=false;
  OLED.clearDisplay();
  return "OK";
}

String DisplayWrapper::hideLoopCount(){
  showingLoopCount=false;
  OLED.clearDisplay();
  return "OK";
}

String DisplayWrapper::hideCommandCount(){
  showingCommandCount=false;
  OLED.clearDisplay();
  return "OK";
}

String DisplayWrapper::hideTotalErrorCount(){
  showingTotalErrorCount=false;
  OLED.clearDisplay();
  return "OK";
}

String DisplayWrapper::hideLine0(){
  showingLine0=false;
  OLED.clearDisplay();
  return "OK";
}

String DisplayWrapper::hideLine1(){
  showingLine1=false;
  OLED.clearDisplay();
  return "OK";
}

String DisplayWrapper::hideLine2(){
  showingLine2=false;
  OLED.clearDisplay();
  return "OK";
}

String DisplayWrapper::hideLine3(){
  showingLine3=false;
  OLED.clearDisplay();
  return "OK";
}

String DisplayWrapper::hideSignal(){
  showingSignal=false;
  OLED.clearDisplay();
  return "OK";
}

String DisplayWrapper::hideBattery(){
  showingBattery=false;
  OLED.clearDisplay();
  return "OK";
}

String DisplayWrapper::hideVolume(){
  showingVolume=false;
  OLED.clearDisplay();
  return "OK";
}

String DisplayWrapper::hideTimeout(){
  showingTimeout=false;
  OLED.clearDisplay();
  return "OK";
}

String DisplayWrapper::isShowingTitle(){
  if(showingTitle){return "true";}
  return "false";
}

String DisplayWrapper::isShowingLoopCount(){
  if(showingLoopCount){return "true";}
  return "false";
}

String DisplayWrapper::isShowingCommandCount(){
  if(showingCommandCount){return "true";}
  return "false";
;}

String DisplayWrapper::isShowingTotalErrorCount(){
  if(showingTotalErrorCount){return "true";}
  return "false";
}

String DisplayWrapper::isShowingLine0(){
  if(showingLine0){return "true";}
  return "false";
}

String DisplayWrapper::isShowingLine1(){
  if(showingLine1){return "true";}
  return "false";
}

String DisplayWrapper::isShowingLine2(){
  if(showingLine2){return "true";}
  return "false";
}

String DisplayWrapper::isShowingLine3(){
  if(showingLine3){return "true";}
  return "false";
}

String DisplayWrapper::isShowingSignal(){
  if(showingSignal){return "true";}
  return "false";
}

String DisplayWrapper::isShowingBattery(){
  if(showingBattery){return "true";}
  return "false";
}

String DisplayWrapper::isShowingVolume(){
  if(showingVolume){return "true";}
  return "false";
}

String DisplayWrapper::isShowingTimeout(){
  if(showingTimeout){return "true";}
  return "false";
}

String DisplayWrapper::setTextLine0(String line0Text){
  line0=line0Text;
  return "OK";
}

String DisplayWrapper::setTextLine1(String line1Text){
  line1=line1Text;
  return "OK";
}

String DisplayWrapper::setTextLine2(String line2Text){
  line2=line2Text;
  return "OK";
}

String DisplayWrapper::setTextLine3(String line3Text){
  line3=line3Text;
  return "OK";
}

String DisplayWrapper::getTextLine0(){
  return line0;
}

String DisplayWrapper::getTextLine1(){
  return line1;
}

String DisplayWrapper::getTextLine2(){
  return line2;
}

String DisplayWrapper::getTextLine3(){
  return line3;
}

void DisplayWrapper::renderText(int line, String text){
  line=constrain(line, 0,3);
  OLED.fillRect(0, line*8, 128, 8, BLACK);
  OLED.setCursor(0,line*8);
  OLED.print(text);
}

void DisplayWrapper::renderVariable(int line, String label, unsigned long value){ //label text longer than 7 characters might be overwritten depending on the size of value
  renderText(line,label);
  OLED.setCursor(42,line*8);
  char placeholder = 249;//bullet character is 249
  if(value<10000000){OLED.print(placeholder);}
  if(value<1000000){OLED.print(placeholder);}
  if(value<100000){OLED.print(placeholder);}
  if(value<10000){OLED.print(placeholder);}
  if(value<1000){OLED.print(placeholder);}
  if(value<100){OLED.print(placeholder);}
  if(value<10){OLED.print(placeholder);}
  OLED.print(value);
}

void DisplayWrapper::renderSignal(String signalStrength){
  OLED.fillRect(95, 0, 33, 8, BLACK);
  int signal=signalStrength.toInt();
  signal=constrain(signal,0,99);
  int index=0;
  if(signal>8){index=1;}
  if(signal>17){index=2;}
  if(signal>25){index=3;}
  if(signal>33){index=4;}
  if(signal>42){index=5;}
  if(signal>50){index=6;}
  if(signal>58){index=7;}
  if(signal>67){index=8;}
  if(signal>75){index=9;}
  if(signal>83){index=10;}
  if(signal>92){index=11;}
  if((index==0)&&(((getUpdateCount().toInt()%5)==0)||((getUpdateCount().toInt()%5)==1))){index=1;}//if index 0 (blank), blink the icon between index 0 (blank) and index 1 (empty) every few frames (based on updateCount) (3 frames off and 2 frames on)
  OLED.drawBitmap(96,0,SIGNALICONS[index], ICON_WIDTH, ICON_HEIGHT, WHITE); //bitmaps, widths, and heights are stored in Bitmaps.h
  OLED.setCursor(110,0);
  if(signal<10){OLED.setCursor(116,0);}
  OLED.print(signal);
  OLED.print("%");
}

void DisplayWrapper::renderBattery(String batteryCharge){
  OLED.fillRect(95, 8, 33, 8, BLACK);
  int charge=batteryCharge.toInt();
  charge=constrain(charge,0,99);
  int index=0;
  if(charge>8){index=1;}
  if(charge>17){index=2;}
  if(charge>25){index=3;}
  if(charge>33){index=4;}
  if(charge>42){index=5;}
  if(charge>50){index=6;}
  if(charge>58){index=7;}
  if(charge>67){index=8;}
  if(charge>75){index=9;}
  if(charge>83){index=10;}
  if(charge>92){index=11;}
  if((index==0)&&(((getUpdateCount().toInt()%5)==0)||((getUpdateCount().toInt()%5)==1))){index=1;}//if index 0 (blank), blink the icon between index 0 (blank) and index 1 (empty) every few frames (based on updateCount) (3 frames off and 2 frames on)
  OLED.drawBitmap(96,8,BATTERYICONS[index], ICON_WIDTH, ICON_HEIGHT, WHITE);//bitmaps, widths, and heights are stored in Bitmaps.h
  OLED.setCursor(110,8);
  if(charge<10){OLED.setCursor(116,8);}
  OLED.print(charge);
  OLED.print("%");
}

void DisplayWrapper::renderVolume(String audioVolume){
  OLED.fillRect(95, 16, 33, 8, BLACK);
  int volume=audioVolume.toInt();
  volume=constrain(volume,0,99);
  int index=0;
  if(volume>17){index=1;}
  if(volume>33){index=2;}
  if(volume>50){index=3;}
  if(volume>67){index=4;}
  if(volume>83){index=5;}
  OLED.drawBitmap(96,16,VOLUMEICONS[index], ICON_WIDTH, ICON_HEIGHT, WHITE);//bitmaps, widths, and heights are stored in Bitmaps.h
  OLED.setCursor(110,16);
  if(volume<10){OLED.setCursor(116,16);}
  OLED.print(volume);
  OLED.print("%");
}

void DisplayWrapper::renderTimeout(int minutes, int seconds){
  OLED.fillRect(95, 24, 33, 8, BLACK);
  minutes=constrain(minutes,0,99);
  seconds=constrain(seconds,0,99);
  OLED.drawBitmap(96,24,SLEEPICON, ICON_WIDTH, ICON_HEIGHT, WHITE);//bitmaps, widths, and heights are stored in Bitmaps.h
  OLED.setCursor(102,24);
  if(minutes<10){OLED.setCursor(108,24);}
  if(minutes!=0){OLED.print(minutes);}
  OLED.setCursor(112,24);
  OLED.print(":");
  OLED.setCursor(116,24);
  if(seconds<10){OLED.print("0");}
  OLED.print(seconds);
}

void DisplayWrapper::updateScreen(){
  OLED.display();
  updateCount++;
}
