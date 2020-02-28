#ifndef DisplayWrapper_h
#define DisplayWrapper_h

#include "src/Adafruit_OLED/Adafruit_GFX.h" //Library for display graphics. Adafruit repository: https://github.com/adafruit/Adafruit-GFX-Library
#include "src/Adafruit_OLED/Adafruit_SSD1306.h" //Library for display hardware. Adafruit repository: https://github.com/adafruit/Adafruit_SSD1306
#include "Arduino.h"                  //Standard arduino library

class DisplayWrapper{                 //Wrapper class for the Adafruit_OLED library- this will make it much easier to work with the OLED
  private:                            //Private- can't access these directly
    unsigned long errorCount=0;        //The number of errors since program start or since errors have been wiped
    String lastError="All good; No errors!";//The default error message, to be replaced with errors when they occur
    Adafruit_SSD1306 OLED;             //The display object from the OLED library, which this class is wrapping. 
    String line0="Line0 text here";
    String line1="Line1 text here";
    String line2="Line2 text here";
    String line3="Line3 text here";
    bool showingTitle=true;
    bool showingLoopCount=true;
    bool showingCommandCount=true;
    bool showingTotalErrorCount=true;
    bool showingLine0=true;
    bool showingLine1=true;
    bool showingLine2=true;
    bool showingLine3=true;
    bool showingSignal=true;
    bool showingBattery=true;
    bool showingVolume=true;
    bool showingTimeout=true;
    unsigned long updateCount=0;//contains the number of times updateScreen() has been called; this is the numer of times the screen has refreshed
    
  public:                             //Public- feel free to use these
    DisplayWrapper();                 //Constructor- gets called automatically when you make an object from this class
    void initialize();                //Call this once before you use any of the other functions below. Configures ____ settings.
    String clearErrors();             //Replaces the error with "All good; No errors!" and sets errorCount to 0.
    String getLastError();            //Returns the last error that occured (string)
    String getErrorCount();           //Returns the number of errors that have occured since the program start or since clearErrors() was called (string)
    String getUpdateCount();
    String showAll();
    String showTitle();
    String showLoopCount();
    String showCommandCount();
    String showTotalErrorCount();
    String showLine0();
    String showLine1();
    String showLine2();
    String showLine3();
    String showSignal();
    String showBattery();
    String showVolume();
    String showTimeout();
    String hideAll();
    String hideTitle();
    String hideLoopCount();
    String hideCommandCount();
    String hideTotalErrorCount();
    String hideLine0();
    String hideLine1();
    String hideLine2();
    String hideLine3();
    String hideSignal();
    String hideBattery();
    String hideVolume();
    String hideTimeout();
    String isShowingTitle();
    String isShowingLoopCount();
    String isShowingCommandCount();
    String isShowingTotalErrorCount();
    String isShowingLine0();
    String isShowingLine1();
    String isShowingLine2();
    String isShowingLine3();
    String isShowingSignal();
    String isShowingBattery();
    String isShowingVolume();
    String isShowingTimeout();
    String setTextLine0(String line0Text);
    String setTextLine1(String line1Text);
    String setTextLine2(String line2Text);
    String setTextLine3(String line3Text);
    String getTextLine0();
    String getTextLine1();
    String getTextLine2();
    String getTextLine3();
    void renderText(int line, String text);
    void renderVariable(int line, String label, unsigned long value);
    void renderSignal(String signalStrength);
    void renderBattery(String batteryCharge);
    void renderVolume(String audioVolume);
    void renderTimeout(int minutes, int seconds);
    void updateScreen();
};

#endif
