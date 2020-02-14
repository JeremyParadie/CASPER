#include "RadioWrapperRobot.h"
#include "AudioWrapper.h"
#include "GPSWrapper.h"
#include "ServoWrapper.h"
#include "DisplayWrapper.h"
#include "BatteryWrapper.h"
#include "SensorWrapper.h"
#include "src/MemoryFree/MemoryFree.h"//Serial.println(freeMemory());//see how much free ram is left in bytes. If this becomes zero then bad things happen. 
#include "src/Timer/Timer.h" //I modified the fork of the original SAMDtimer repo: https://github.com/kbarni/avdweb_SAMDtimer
#include "RelayPowerControl.h"
#include "FRAMWrapper.h"
#include "src/Adafruit_SleepyDog/Adafruit_SleepyDog.h"

String VERSION = "RB.0.1.8";
bool updateDisplayFlag = false;
int timeoutMinutes = 0;
int timeoutSeconds = 0;
int timeoutTenthSeconds = 0;

unsigned long loopCount = 0;
unsigned long commandCount = 0;
unsigned long totalErrorCount = 0;
unsigned long crashCount = 0;

RadioWrapperRobot Radio;//radio object enables wireless communication
AudioWrapper Audio;//audio object enables playing audio
GPSWrapper GPS;//GPS object enables position sensing
ServoWrapper Servo;
DisplayWrapper Display;
BatteryWrapper Battery;
SensorWrapper Sensor;
FRAMWrapper Fram;



void GPS_ISR() { //Interrupt service routine is called a thousand times per second to tend to the incomming GPS characters
  GPS.read();//reads in a new char from GPS, if there is one. This needs to be called very often, around 1khz (a thousand times per second)
}

void Display_ISR() { //runs once every tenth of a second
  updateDisplayFlag = true;
  timeoutTenthSeconds--;
}

void updateTimeout() {
  if (timeoutTenthSeconds < 0) {
    timeoutSeconds--;
    timeoutTenthSeconds = timeoutTenthSeconds + 10;
  }
  if (timeoutSeconds < 0) {
    timeoutMinutes--;
    timeoutSeconds = timeoutSeconds + 60;
  }
  if (timeoutMinutes <= 0) {
    if ((timeoutSeconds <= 1) && (timeoutTenthSeconds <= 1)) {
      Radio.send("Goodbye!");
      powerDown();
    }
  }
  if (timeoutMinutes == 5 && timeoutSeconds == 0) {
    Radio.send("5 minutes remaining!");
    delay(1000);
  }
  if (timeoutMinutes == 3 && timeoutSeconds == 0) {
    Radio.send("3 minutes remaining!");
    delay(1000);
  }
  if (timeoutMinutes == 1 && timeoutSeconds == 0) {
    Radio.send("ONE MINUTE REMAINING!");
    delay(1000);
  }
}

//void BatteryDetect{
//  if
//}

void updateCounters() {
  loopCount++;
  updateTimeout();
  totalErrorCount = 0;
  totalErrorCount = totalErrorCount + Audio.getErrorCount().toInt();
  totalErrorCount = totalErrorCount + Battery.getErrorCount().toInt();
  totalErrorCount = totalErrorCount + Display.getErrorCount().toInt();
  totalErrorCount = totalErrorCount + GPS.getErrorCount().toInt();
  totalErrorCount = totalErrorCount + Radio.getErrorCount().toInt();
  totalErrorCount = totalErrorCount + Sensor.getErrorCount().toInt();
  totalErrorCount = totalErrorCount + Servo.getErrorCount().toInt();
  totalErrorCount = totalErrorCount + Fram.getErrorCount().toInt();
  loopCount = loopCount % 100000000; //use remainder to restrict the size of the value to 8 characters or less
  commandCount = commandCount % 100000000;
  totalErrorCount = totalErrorCount % 100000000;
}

void updateDisplay() {
  updateDisplayFlag = false;
  if (Display.isShowingLine0() == "true") {
    Display.renderText(0, Display.getTextLine0());
  }
  if (Display.isShowingLine1() == "true") {
    Display.renderText(1, Display.getTextLine1());
  }
  if (Display.isShowingLine2() == "true") {
    Display.renderText(2, Display.getTextLine2());
  }
  if (Display.isShowingLine3() == "true") {
    Display.renderText(3, Display.getTextLine3());
  }
  if (Display.isShowingTitle() == "true") {
    Display.renderText(0, "CASPER " + VERSION);
  }
  if (Display.isShowingLoopCount() == "true") {
    Display.renderVariable(1, "Loops: ", loopCount);
  }
  if (Display.isShowingCommandCount() == "true") {
    Display.renderVariable(2, "Comms: ", commandCount);
  }
  if (Display.isShowingTotalErrorCount() == "true") {
    Display.renderVariable(3, "Errrs: ", totalErrorCount);
  }
  if ((Display.isShowingSignal() == "true") && (commandCount > 0)) {
    Display.renderSignal(Radio.getSignalStrength()); //if there has been at least one command received, then we have signal strength information, so we can render the signal if it is enabled
  }
  if (Display.isShowingBattery() == "true") {
    Display.renderBattery(Battery.getCharge());
  }
  if (Display.isShowingVolume() == "true") {
    Display.renderVolume(Audio.getVolume());
  }
  if (Display.isShowingTimeout() == "true") {
    Display.renderTimeout(timeoutMinutes, timeoutSeconds);
  }
  Display.updateScreen();
}

void setup() {
  Serial.begin(9600);
  int countdownMS = Watchdog.enable(5000);
  loopCount = 0;
  timeoutMinutes = 45;
  timeoutSeconds = 0;
  timeoutTenthSeconds = 0;
  loopCount = 0;
  commandCount = 0;
  totalErrorCount = 0;


  Fram.initialize(); //initialize the FRAM
  Display.initialize();//initialize the display object
  Radio.initialize();  //initialize the radio object
  Audio.initialize();  //initialize the audio object
  GPS.initialize();    //initialize the GPS object
  Servo.initialize();  //initialize the servo opject
  Sensor.initialize(); //initialize the sensor opject
  Battery.initialize(&Fram); //initialize the battery opject


  Fram.writeData(0, 0xF3AB); //TEST

  Timer GPSTimer = Timer(4, GPS_ISR, 1e3);        //create an ISR with the SAMDTimer library that is called every 0.001 seconds (a thousand times per second) (1e3 microseconds) (1e2 was tested but gave bad data)
  Timer DisplayTimer = Timer(5, Display_ISR, 1e5); //create an ISR with the SAMDTimer library that is called every 0.1 seconds (ten times per second) (1e5 microseconds)}
  // to see WDT work, comment (//) the next line, or uncomment to see WDT reset work
}

void loop() {
  Watchdog.reset();
  String commandFromRadio = Radio.receive(); //get response from radio
  if (commandFromRadio != "No Data Received") {
    parse(commandFromRadio);/*Radio.send(parse(commandFromRadio));*/commandCount++; //if there is a response, parse it and send a response
  }
  if (updateDisplayFlag == true) {
    updateDisplay();
  }
  updateCounters();
}

void parse(String command) {
  if (firstWordOf(command, "Radio")) { //if the first word of command is "Radio"
    command.remove(0, String("Radio ").length()); //remove the first word of the command
    if (firstWordOf(command, "clearErrors"))         {
      Radio.send(Radio.clearErrors());
      return;
    }
    if (firstWordOf(command, "getLastError"))        {
      Radio.send(Radio.getLastError());
      return;
    }
    if (firstWordOf(command, "getErrorCount"))       {
      Radio.send(Radio.getErrorCount());
      return;
    }
    if (firstWordOf(command, "getSignalStrength"))   {
      Radio.send(Radio.getSignalStrength());
      return;
    }
    if (firstWordOf(command, "getTemperature"))      {
      Radio.send(Radio.getTemperature());
      return;
    }
  }
  if (firstWordOf(command, "Audio")) { //if the first word of command is "Audio"
    command.remove(0, String("Audio ").length()); //remove the first word of the command
    if (firstWordOf(command, "clearErrors"))         {
      Radio.send(Audio.clearErrors());
      return;
    }
    if (firstWordOf(command, "getLastError"))        {
      Radio.send(Audio.getLastError());
      return;
    }
    if (firstWordOf(command, "getErrorCount"))       {
      Radio.send(Audio.getErrorCount());
      return;
    }
    if (firstWordOf(command, "beep"))                {
      Radio.send(Audio.beep());
      return;
    }
    if (firstWordOf(command, "setVolume"))           {
      command.remove(0, String("setVolume ").length());
      Radio.send(Audio.setVolume(command));
      return;
    }
    if (firstWordOf(command, "getVolume"))           {
      Radio.send(Audio.getVolume());
      return;
    }
    if (firstWordOf(command, "play"))                {
      command.remove(0, String("play ").length());
      Radio.send(Audio.play(command));
      return;
    }
    if (firstWordOf(command, "isPlaying"))           {
      Radio.send(Audio.isPlaying());
      return;
    }
    if (firstWordOf(command, "pause"))               {
      Radio.send(Audio.pause());
      return;
    }
    if (firstWordOf(command, "resume"))              {
      Radio.send(Audio.resume());
      return;
    }
    if (firstWordOf(command, "isPaused"))            {
      Radio.send(Audio.isPaused());
      return;
    }
    if (firstWordOf(command, "stop"))                {
      Radio.send(Audio.stop());
      return;
    }
    if (firstWordOf(command, "isStopped"))           {
      Radio.send(Audio.isStopped());
      return;
    }
    if (firstWordOf(command, "getLastFileName"))     {
      Radio.send(Audio.getLastFileName());
      return;
    }
  }
  if (firstWordOf(command, "GPS")) { //if the first word of command is "GPS"
    command.remove(0, String("GPS ").length()); //remove the first word of the command
    if (firstWordOf(command, "clearErrors"))         {
      Radio.send(GPS.clearErrors());
      return;
    }
    if (firstWordOf(command, "getLastError"))        {
      Radio.send(GPS.getLastError());
      return;
    }
    if (firstWordOf(command, "getErrorCount"))       {
      Radio.send(GPS.getErrorCount());
      return;
    }
    if (firstWordOf(command, "getLocation"))         {
      Radio.send(GPS.getLocation());
      return;
    }
    if (firstWordOf(command, "getAltitude"))         {
      Radio.send(GPS.getAltitude());
      return;
    }
    if (firstWordOf(command, "getTime"))             {
      Radio.send(GPS.getTime());
      return;
    }
    if (firstWordOf(command, "getDate"))             {
      Radio.send(GPS.getDate());
      return;
    }
    if (firstWordOf(command, "getFix"))              {
      Radio.send(GPS.getFix());
      return;
    }
    if (firstWordOf(command, "getSatelliteCount"))   {
      Radio.send(GPS.getSatelliteCount());
      return;
    }
  }
  if (firstWordOf(command, "Servo")) { //if the first word of command is "Servo"
    command.remove(0, String("Servo ").length()); //remove the first word of the command
    if (firstWordOf(command, "clearErrors"))         {
      Radio.send(Servo.clearErrors());
      return;
    }
    if (firstWordOf(command, "getLastError"))        {
      Radio.send(Servo.getLastError());
      return;
    }
    if (firstWordOf(command, "getErrorCount"))       {
      Radio.send(Servo.getErrorCount());
      return;
    }
    if (firstWordOf(command, "getVoltage"))          {
      Radio.send(Servo.getVoltage());
      return;
    }
    if (firstWordOf(command, "0")) {
      command.remove(0, String("0 ").length()); //remove the first word of the command
      if (firstWordOf(command, "getAngle"))          {
        Radio.send(Servo.getAngle(0));
        return;
      }
      if (firstWordOf(command, "setAngle"))          {
        command.remove(0, String("setAngle ").length());
        Radio.send(Servo.setAngle(0, command));
        return;
      }
      if (firstWordOf(command, "goLimp"))            {
        Radio.send(Servo.goLimp(0));
        return;
      }
    }
    if (firstWordOf(command, "1")) {
      command.remove(0, String("1 ").length()); //remove the first word of the command
      if (firstWordOf(command, "getAngle"))          {
        Radio.send(Servo.getAngle(1));
        return;
      }
      if (firstWordOf(command, "setAngle"))          {
        command.remove(0, String("setAngle ").length());
        Radio.send(Servo.setAngle(1, command));
        return;
      }
      if (firstWordOf(command, "goLimp"))            {
        Radio.send(Servo.goLimp(1));
        return;
      }
    }
    if (firstWordOf(command, "2")) {
      command.remove(0, String("2 ").length()); //remove the first word of the command
      if (firstWordOf(command, "getAngle"))          {
        Radio.send(Servo.getAngle(2));
        return;
      }
      if (firstWordOf(command, "setAngle"))          {
        command.remove(0, String("setAngle ").length());
        Radio.send(Servo.setAngle(2, command));
        return;
      }
      if (firstWordOf(command, "goLimp"))            {
        Radio.send(Servo.goLimp(2));
        return;
      }
    }
    if (firstWordOf(command, "3")) {
      command.remove(0, String("3 ").length()); //remove the first word of the command
      if (firstWordOf(command, "getAngle"))          {
        Radio.send(Servo.getAngle(3));
        return;
      }
      if (firstWordOf(command, "setAngle"))          {
        command.remove(0, String("setAngle ").length());
        Radio.send(Servo.setAngle(3, command));
        return;
      }
      if (firstWordOf(command, "goLimp"))            {
        Radio.send(Servo.goLimp(3));
        return;
      }
    }
    if (firstWordOf(command, "4")) {
      command.remove(0, String("4 ").length()); //remove the first word of the command
      if (firstWordOf(command, "getAngle"))          {
        Radio.send(Servo.getAngle(4));
        return;
      }
      if (firstWordOf(command, "setAngle"))          {
        command.remove(0, String("setAngle ").length());
        Radio.send(Servo.setAngle(4, command));
        return;
      }
      if (firstWordOf(command, "goLimp"))            {
        Radio.send(Servo.goLimp(4));
        return;
      }
    }
    if (firstWordOf(command, "5")) {
      command.remove(0, String("5 ").length()); //remove the first word of the command
      if (firstWordOf(command, "getAngle"))          {
        Radio.send(Servo.getAngle(5));
        return;
      }
      if (firstWordOf(command, "setAngle"))          {
        command.remove(0, String("setAngle ").length());
        Radio.send(Servo.setAngle(5, command));
        return;
      }
      if (firstWordOf(command, "goLimp"))            {
        Radio.send(Servo.goLimp(5));
        return;
      }
    }
    if (firstWordOf(command, "6")) {
      command.remove(0, String("6 ").length()); //remove the first word of the command
      if (firstWordOf(command, "getAngle"))          {
        Radio.send(Servo.getAngle(6));
        return;
      }
      if (firstWordOf(command, "setAngle"))          {
        command.remove(0, String("setAngle ").length());
        Radio.send(Servo.setAngle(6, command));
        return;
      }
      if (firstWordOf(command, "goLimp"))            {
        Radio.send(Servo.goLimp(6));
        return;
      }
    }
    if (firstWordOf(command, "7")) {
      command.remove(0, String("7 ").length()); //remove the first word of the command
      if (firstWordOf(command, "getAngle"))          {
        Radio.send(Servo.getAngle(7));
        return;
      }
      if (firstWordOf(command, "setAngle"))          {
        command.remove(0, String("setAngle ").length());
        Radio.send(Servo.setAngle(7, command));
        return;
      }
      if (firstWordOf(command, "goLimp"))            {
        Radio.send(Servo.goLimp(7));
        return;
      }
    }
  }
  if (firstWordOf(command, "Display")) { //if the first word of command is "Display"
    command.remove(0, String("Display ").length()); //remove the first word of the command
    if (firstWordOf(command, "clearErrors"))         {
      Radio.send(Display.clearErrors());
      return;
    }
    if (firstWordOf(command, "getLastError"))        {
      Radio.send(Display.getLastError());
      return;
    }
    if (firstWordOf(command, "getErrorCount"))       {
      Radio.send(Display.getErrorCount());
      return;
    }
    if (firstWordOf(command, "getUpdateCount"))      {
      Radio.send(Display.getUpdateCount());
      return;
    }
    if (firstWordOf(command, "show")) {
      command.remove(0, String("show ").length()); //remove the first word of the command
      if (firstWordOf(command, "all"))               {
        Radio.send(Display.showAll());
        return;
      }
      if (firstWordOf(command, "title"))             {
        Radio.send(Display.showTitle());
        return;
      }
      if (firstWordOf(command, "loopCount"))         {
        Radio.send(Display.showLoopCount());
        return;
      }
      if (firstWordOf(command, "commandCount"))      {
        Radio.send(Display.showCommandCount());
        return;
      }
      if (firstWordOf(command, "totalErrorCount"))   {
        Radio.send(Display.showTotalErrorCount());
        return;
      }
      if (firstWordOf(command, "line0"))             {
        Radio.send(Display.showLine0());
        return;
      }
      if (firstWordOf(command, "line1"))             {
        Radio.send(Display.showLine1());
        return;
      }
      if (firstWordOf(command, "line2"))             {
        Radio.send(Display.showLine2());
        return;
      }
      if (firstWordOf(command, "line3"))             {
        Radio.send(Display.showLine3());
        return;
      }
      if (firstWordOf(command, "signal"))            {
        Radio.send(Display.showSignal());
        return;
      }
      if (firstWordOf(command, "battery"))           {
        Radio.send(Display.showBattery());
        return;
      }
      if (firstWordOf(command, "volume"))            {
        Radio.send(Display.showVolume());
        return;
      }
      if (firstWordOf(command, "timeout"))           {
        Radio.send(Display.showAll());
        return;
      }
    }
    if (firstWordOf(command, "hide")) {
      command.remove(0, String("hide ").length()); //remove the first word of the command
      if (firstWordOf(command, "all"))               {
        Radio.send(Display.hideAll());
        return;
      }
      if (firstWordOf(command, "title"))             {
        Radio.send(Display.hideTitle());
        return;
      }
      if (firstWordOf(command, "loopCount"))         {
        Radio.send(Display.hideLoopCount());
        return;
      }
      if (firstWordOf(command, "commandCount"))      {
        Radio.send(Display.hideCommandCount());
        return;
      }
      if (firstWordOf(command, "totalErrorCount"))   {
        Radio.send(Display.hideTotalErrorCount());
        return;
      }
      if (firstWordOf(command, "line0"))             {
        Radio.send(Display.hideLine0());
        return;
      }
      if (firstWordOf(command, "line1"))             {
        Radio.send(Display.hideLine1());
        return;
      }
      if (firstWordOf(command, "line2"))             {
        Radio.send(Display.hideLine2());
        return;
      }
      if (firstWordOf(command, "line3"))             {
        Radio.send(Display.hideLine3());
        return;
      }
      if (firstWordOf(command, "signal"))            {
        Radio.send(Display.hideSignal());
        return;
      }
      if (firstWordOf(command, "battery"))           {
        Radio.send(Display.hideBattery());
        return;
      }
      if (firstWordOf(command, "volume"))            {
        Radio.send(Display.hideVolume());
        return;
      }
      if (firstWordOf(command, "timeout"))           {
        Radio.send(Display.hideTimeout());
        return;
      }
    }
    if (firstWordOf(command, "isShowing")) {
      command.remove(0, String("isShowing ").length()); //remove the first word of the command
      if (firstWordOf(command, "title"))             {
        Radio.send(Display.isShowingTitle());
        return;
      }
      if (firstWordOf(command, "loopCount"))         {
        Radio.send(Display.isShowingLoopCount());
        return;
      }
      if (firstWordOf(command, "commandCount"))      {
        Radio.send(Display.isShowingCommandCount());
        return;
      }
      if (firstWordOf(command, "totalErrorCount"))   {
        Radio.send(Display.isShowingTotalErrorCount());
        return;
      }
      if (firstWordOf(command, "line0"))             {
        Radio.send(Display.isShowingLine0());
        return;
      }
      if (firstWordOf(command, "line1"))             {
        Radio.send(Display.isShowingLine1());
        return;
      }
      if (firstWordOf(command, "line2"))             {
        Radio.send(Display.isShowingLine2());
        return;
      }
      if (firstWordOf(command, "line3"))             {
        Radio.send(Display.isShowingLine3());
        return;
      }
      if (firstWordOf(command, "signal"))            {
        Radio.send(Display.isShowingSignal());
        return;
      }
      if (firstWordOf(command, "battery"))           {
        Radio.send(Display.isShowingBattery());
        return;
      }
      if (firstWordOf(command, "volume"))            {
        Radio.send(Display.isShowingVolume());
        return;
      }
      if (firstWordOf(command, "timeout"))           {
        Radio.send(Display.isShowingTimeout());
        return;
      }
    }
    if (firstWordOf(command, "setText")) {
      command.remove(0, String("setText ").length()); //remove the first word of the command}
      if (firstWordOf(command, "line0"))             {
        command.remove(0, String("line0 ").length());
        Radio.send(Display.setTextLine0(command));
        return;
      }
      if (firstWordOf(command, "line1"))             {
        command.remove(0, String("line1 ").length());
        Radio.send(Display.setTextLine1(command));
        return;
      }
      if (firstWordOf(command, "line2"))             {
        command.remove(0, String("line2 ").length());
        Radio.send(Display.setTextLine2(command));
        return;
      }
      if (firstWordOf(command, "line3"))             {
        command.remove(0, String("line3 ").length());
        Radio.send(Display.setTextLine3(command));
        return;
      }
    }
    if (firstWordOf(command, "getText")) {
      command.remove(0, String("getText ").length()); //remove the first word of the command}
      if (firstWordOf(command, "line0"))             {
        Radio.send(Display.getTextLine0());
        return;
      }
      if (firstWordOf(command, "line1"))             {
        Radio.send(Display.getTextLine1());
        return;
      }
      if (firstWordOf(command, "line2"))             {
        Radio.send(Display.getTextLine2());
        return;
      }
      if (firstWordOf(command, "line3"))             {
        Radio.send(Display.getTextLine3());
        return;
      }
    }
  }
  if (firstWordOf(command, "Battery")) { //if the first word of command is "Battery"
    command.remove(0, String("Battery ").length()); //remove the first word of the command
    if (firstWordOf(command, "clearErrors"))         {
      Radio.send(Battery.clearErrors());
      return;
    }
    if (firstWordOf(command, "getLastError"))        {
      Radio.send(Battery.getLastError());
      return;
    }
    if (firstWordOf(command, "getErrorCount"))       {
      Radio.send(Battery.getErrorCount());
      return;
    }
    if (firstWordOf(command, "getVoltage"))          {
      Radio.send(Battery.getVoltage());
      return;
    }
    if (firstWordOf(command, "getCharge"))           {
      Radio.send(Battery.getCharge());
      return;
    }
    if (firstWordOf(command, "getHealth"))           {
      Radio.send(Battery.getHealth());
      return;
    }
    if (firstWordOf(command, "getCurrent"))          {
      Radio.send(Battery.getCurrent());
      return;
    }
    if (firstWordOf(command, "getPower"))            {
      Radio.send(Battery.getPower());
      return;
    }
    if (firstWordOf(command, "getCapacity"))         {
      Radio.send(Battery.getCapacity());
      return;
    }
    if (firstWordOf(command, "getMinimum"))         {
      Radio.send(Battery.getMinimum());
      return;
    }
    if (firstWordOf(command, "isCharging"))          {
      Radio.send(Battery.isCharging());
      return;
    }
    if (firstWordOf(command, "setMaxCapacity"))      {
      command.remove(0, String("setMaxCapacity ").length());
      Radio.send(Battery.setMaxCapacity(command));
      return;
    }
    if (firstWordOf(command, "setMinimum"))          {
      command.remove(0, String("setMinimum ").length());
      Radio.send(Battery.setMinimum(command));
      return;
    }
  }
  if (firstWordOf(command, "Sensor")) { //if the first word of command is "Sensor"
    command.remove(0, String("Sensor ").length()); //remove the first word of the command
    if (firstWordOf(command, "clearErrors"))         {
      Radio.send(Sensor.clearErrors());
      return;
    }
    if (firstWordOf(command, "getLastError"))        {
      Radio.send(Sensor.getLastError());
      return;
    }
    if (firstWordOf(command, "getErrorCount"))       {
      Radio.send(Sensor.getErrorCount());
      return;
    }
    if (firstWordOf(command, "getTemperature"))      {
      Radio.send(Sensor.getTemperature());
      return;
    }
    if (firstWordOf(command, "getPressure"))         {
      Radio.send(Sensor.getPressure());
      return;
    }
    if (firstWordOf(command, "getHumidity"))         {
      Radio.send(Sensor.getHumidity());
      return;
    }
  }
  if (firstWordOf(command, "FRAM")) { //if the first word of command is "FRAM"
    command.remove(0, String("FRAM ").length()); //remove the first word of the command
    if (firstWordOf(command, "clearErrors"))         {
      Radio.send(Fram.clearErrors());
      return;
    }
    if (firstWordOf(command, "getLastError"))        {
      Radio.send(Fram.getLastError());
      return;
    }
    if (firstWordOf(command, "getErrorCount"))       {
      Radio.send(Fram.getErrorCount());
      return;
    }
    if (firstWordOf(command, "getData"))             {
      command.remove(0, String("getData ").length());
      Radio.send(Fram.getData(command));
      return;
    }
    if (firstWordOf(command, "clearData"))           {
      Radio.send(Fram.clearData());
      return;
    }
  }
  if (firstWordOf(command, "System")) { //if the first word of command is "System"
    command.remove(0, String("System ").length()); //remove the first word of the command
    if (firstWordOf(command, "wakeUp"))              {
      Radio.send(wakeUp());
      return;
    }
    if (firstWordOf(command, "goToSleep"))           {
      Radio.send(goToSleep());
      return;
    }
    if (firstWordOf(command, "restart"))             {
      Radio.send(restart());
      return;
    }
    if (firstWordOf(command, "getAvailableMemory"))  {
      Radio.send(String(freeMemory()) + " bytes");
      return;
    }
    if (firstWordOf(command, "getVersion"))          {
      Radio.send(VERSION);
      return;
    }
    if (firstWordOf(command, "ping"))                {
      Radio.send("pong");
      return;
    }
    if (firstWordOf(command, "forceCrash"))          {
      Radio.send(forceCrash());
      return;
    }
    if (firstWordOf(command, "shutDown"))            {
      Radio.send(shutDown());
      return;
    }
    if (firstWordOf(command, "getTimeLeft"))         {
      Radio.send(getTimeLeft());
      return;
    }
  }
  Radio.send("EpicError: " + command); return;
}


bool firstWordOf(String command, String key) { //is key the first word in command? If so return true; if not return false.
  if (firstWord(command).equals(key)) {
    return true; //note that this is case sensitive
  }
  else {
    return false;
  }
}

String firstWord(String command) { //returns the first word of a string, denoted by a space, not including the space.
  int index = command.indexOf(" ");
  if (index == -1) {
    index = command.length();
  }
  return command.substring(0, index);
}

String getTimeLeft() {
  String timeLeft = String(timeoutMinutes) + String(":") + String(timeoutSeconds) + String(".") + String(timeoutTenthSeconds) + String(" remaining");
  return timeLeft;
}
String wakeUp() {
  //rtc.attachInterrupt(match);
  return "";
}
String goToSleep() {
  //rtc.standbyMode();
  return "Sleeping";
}
String restart() {
  Watchdog.enable(10); //Setting a very quick watchdog timer that's impossible to interrupt
  return "Restarting CASPER RB.0.1.8";
}
String shutDown() {
  powerDown();
  return "OK";
}
String forceCrash() {
  //totalErrorCount++;
  //lastError="Error: System Crash; Restart required";
  while (1); //while 1, watchdog will cause reset in specified time
  return "System Crash";
}
