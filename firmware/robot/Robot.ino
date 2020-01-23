#include "RadioWrapperRobot.h"
#include "AudioWrapper.h"
#include "GPSWrapper.h"
#include "ServoWrapper.h"
#include "DisplayWrapper.h"
#include "BatteryWrapper.h"
#include "SensorWrapper.h"
#include "src/MemoryFree/MemoryFree.h"//Serial.println(freeMemory());//see how much free ram is left in bytes. If this becomes zero then bad things happen. 
#include "src/avdweb_SAMDtimer/avdweb_SAMDtimer.h"

RadioWrapperRobot Radio;//radio object enables wireless communication
AudioWrapper Audio;//audio object enables playing audio
GPSWrapper GPS;//GPS object enables position sensing
ServoWrapper Servo;
DisplayWrapper Display;
BatteryWrapper Battery;
SensorWrapper Sensor;

void GPS_ISR(struct tc_module *const module_inst){ //Interrupt service routine is called a thousand times per second to tend to the incomming GPS characters
  GPS.read();//reads in a new char from GPS, if there is one. This needs to be called very often, around 1khz (a thousand times per second)
}

SAMDtimer GPSTimer = SAMDtimer(4, GPS_ISR, 1e3);//create the ISR with the SAMDTimer library that is called every .001 seconds (a thousand times per second) (1e3 microseconds) (1e2 was tested but gave bad data)

void setup(){
  Serial.begin(9600);//serial port to debug to computer
  Radio.initialize();//initialize the radio object
  Audio.initialize();//initialize the Audio object
  GPS.initialize();//initialize the GPS object
}

void loop(){
  String commandFromRadio=Radio.receive();//get response from radio
  if(commandFromRadio!="No Data Received"){//if there is a response,
    parse(commandFromRadio);//parse it and send a response
  }
}

void parse(String command){
  if(firstWordOf(command,"Radio")){//if the first word of command is "Radio"
    command.remove(0,String("Radio ").length());//remove the first word of the command
    if(firstWordOf(command,"clearErrors"))         {Radio.send(Radio.clearErrors());return;}
    if(firstWordOf(command,"getLastError"))        {Radio.send(Radio.getLastError());return;}
    if(firstWordOf(command,"getErrorCount"))       {Radio.send(Radio.getErrorCount());return;}
    if(firstWordOf(command,"getSignalStrength"))   {Radio.send(Radio.getSignalStrength());return;}
    if(firstWordOf(command,"getTemperature"))      {Radio.send(Radio.getTemperature());return;}
  }
  if(firstWordOf(command,"Audio")){//if the first word of command is "Audio"
    command.remove(0,String("Audio ").length());//remove the first word of the command
    if(firstWordOf(command,"clearErrors"))         {Radio.send(Audio.clearErrors());return;}
    if(firstWordOf(command,"getLastError"))        {Radio.send(Audio.getLastError());return;}
    if(firstWordOf(command,"getErrorCount"))       {Radio.send(Audio.getErrorCount());return;}
    if(firstWordOf(command,"beep"))                {Radio.send(Audio.beep());return;}
    if(firstWordOf(command,"setVolume"))           {command.remove(0,String("setVolume ").length());Radio.send(Audio.setVolume(command));return;}
    if(firstWordOf(command,"getVolume"))           {Radio.send(Audio.getVolume());return;}
    if(firstWordOf(command,"play"))                {command.remove(0,String("play ").length());Radio.send(Audio.play(command));return;}
    if(firstWordOf(command,"isPlaying"))           {Radio.send(Audio.isPlaying());return;}
    if(firstWordOf(command,"pause"))               {Radio.send(Audio.pause());return;}
    if(firstWordOf(command,"resume"))              {Radio.send(Audio.resume());return;}
    if(firstWordOf(command,"isPaused"))            {Radio.send(Audio.isPaused());return;}
    if(firstWordOf(command,"stop"))                {Radio.send(Audio.stop());return;}
    if(firstWordOf(command,"isStopped"))           {Radio.send(Audio.isStopped());return;}
    if(firstWordOf(command,"getLastFileName"))     {Radio.send(Audio.getLastFileName());return;}
  }
  if(firstWordOf(command,"GPS")){//if the first word of command is "GPS"
    command.remove(0,String("GPS ").length());//remove the first word of the command
    if(firstWordOf(command,"clearErrors"))         {Radio.send(GPS.clearErrors());return;}
    if(firstWordOf(command,"getLastError"))        {Radio.send(GPS.getLastError());return;}
    if(firstWordOf(command,"getErrorCount"))       {Radio.send(GPS.getErrorCount());return;}
    if(firstWordOf(command,"getLocation"))         {Radio.send(GPS.getLocation());return;}
    if(firstWordOf(command,"getAltitude"))         {Radio.send(GPS.getAltitude());return;}
    if(firstWordOf(command,"getTime"))             {Radio.send(GPS.getTime());return;}
    if(firstWordOf(command,"getDate"))             {Radio.send(GPS.getDate());return;}
    if(firstWordOf(command,"getFix"))              {Radio.send(GPS.getFix());return;}
    if(firstWordOf(command,"getSatelliteCount"))   {Radio.send(GPS.getSatelliteCount());return;}
  }
  if(firstWordOf(command,"Servo")){//if the first word of command is "Servo"
    command.remove(0,String("Servo ").length());//remove the first word of the command
    if(firstWordOf(command,"clearErrors"))         {Radio.send(Servo.clearErrors());return;}
    if(firstWordOf(command,"getLastError"))        {Radio.send(Servo.getLastError());return;}
    if(firstWordOf(command,"getErrorCount"))       {Radio.send(Servo.getErrorCount());return;}
    if(firstWordOf(command,"0")){
      command.remove(0,String("0 ").length());//remove the first word of the command
      if(firstWordOf(command,"getAngle"))          {Radio.send(Servo.getAngle(0));return;}
      if(firstWordOf(command,"setAngle"))          {command.remove(0,String("setAngle ").length());Radio.send(Servo.setAngle(0,command));return;}
    }
    if(firstWordOf(command,"1")){
      command.remove(0,String("1 ").length());//remove the first word of the command
      if(firstWordOf(command,"getAngle"))          {Radio.send(Servo.getAngle(1));return;}
      if(firstWordOf(command,"setAngle"))          {command.remove(0,String("setAngle ").length());Radio.send(Servo.setAngle(1,command));return;}
    }
    if(firstWordOf(command,"2")){
      command.remove(0,String("2 ").length());//remove the first word of the command
      if(firstWordOf(command,"getAngle"))          {Radio.send(Servo.getAngle(2));return;}
      if(firstWordOf(command,"setAngle"))          {command.remove(0,String("setAngle ").length());Radio.send(Servo.setAngle(2,command));return;}
    }
    if(firstWordOf(command,"3")){
      command.remove(0,String("3 ").length());//remove the first word of the command
      if(firstWordOf(command,"getAngle"))          {Radio.send(Servo.getAngle(3));return;}
      if(firstWordOf(command,"setAngle"))          {command.remove(0,String("setAngle ").length());Radio.send(Servo.setAngle(3,command));return;}
    }
    if(firstWordOf(command,"4")){
      command.remove(0,String("4 ").length());//remove the first word of the command
      if(firstWordOf(command,"getAngle"))          {Radio.send(Servo.getAngle(4));return;}
      if(firstWordOf(command,"setAngle"))          {command.remove(0,String("setAngle ").length());Radio.send(Servo.setAngle(4,command));return;}
    }
    if(firstWordOf(command,"5")){
      command.remove(0,String("5 ").length());//remove the first word of the command
      if(firstWordOf(command,"getAngle"))          {Radio.send(Servo.getAngle(5));return;}
      if(firstWordOf(command,"setAngle"))          {command.remove(0,String("setAngle ").length());Radio.send(Servo.setAngle(5,command));return;}
    }
    if(firstWordOf(command,"6")){
      command.remove(0,String("6 ").length());//remove the first word of the command
      if(firstWordOf(command,"getAngle"))          {Radio.send(Servo.getAngle(6));return;}
      if(firstWordOf(command,"setAngle"))          {command.remove(0,String("setAngle ").length());Radio.send(Servo.setAngle(6,command));return;}
    }
    if(firstWordOf(command,"7")){
      command.remove(0,String("7 ").length());//remove the first word of the command
      if(firstWordOf(command,"getAngle"))          {Radio.send(Servo.getAngle(7));return;}
      if(firstWordOf(command,"setAngle"))          {command.remove(0,String("setAngle ").length());Radio.send(Servo.setAngle(7,command));return;}
    }
  }
  if(firstWordOf(command,"Display")){//if the first word of command is "Display"
    command.remove(0,String("Display ").length());//remove the first word of the command
    if(firstWordOf(command,"clearErrors"))         {Radio.send(Display.clearErrors());return;}
    if(firstWordOf(command,"getLastError"))        {Radio.send(Display.getLastError());return;}
    if(firstWordOf(command,"getErrorCount"))       {Radio.send(Display.getErrorCount());return;}
    if(firstWordOf(command,"clear"))               {Radio.send(Display.clear());return;}
    if(firstWordOf(command,"turnOn"))              {Radio.send(Display.turnOn());return;}
    if(firstWordOf(command,"turnOff"))             {Radio.send(Display.turnOff());return;}
    if(firstWordOf(command,"getState"))            {Radio.send(Display.getState());return;}
    if(firstWordOf(command,"Line1")){
      command.remove(0,String("Line1 ").length());//remove the first word of the command
      if(firstWordOf(command,"getText"))           {Radio.send(Display.getText(1));return;}
      if(firstWordOf(command,"setText"))           {command.remove(0,String("setText ").length());Radio.send(Display.setText(1,command));return;}
    }
    if(firstWordOf(command,"Line2")){
      command.remove(0,String("Line2 ").length());//remove the first word of the command
      if(firstWordOf(command,"getText"))           {Radio.send(Display.getText(2));return;}
      if(firstWordOf(command,"setText"))           {command.remove(0,String("setText ").length());Radio.send(Display.setText(2,command));return;}
    }
    if(firstWordOf(command,"Line3")){
      command.remove(0,String("Line3 ").length());//remove the first word of the command
      if(firstWordOf(command,"getText"))           {Radio.send(Display.getText(3));return;}
      if(firstWordOf(command,"setText"))           {command.remove(0,String("setText ").length());Radio.send(Display.setText(3,command));return;}
    }
  }
  if(firstWordOf(command,"Battery")){//if the first word of command is "Battery"
    command.remove(0,String("Battery ").length());//remove the first word of the command
    if(firstWordOf(command,"clearErrors"))         {Radio.send(Battery.clearErrors());return;}
    if(firstWordOf(command,"getLastError"))        {Radio.send(Battery.getLastError());return;}
    if(firstWordOf(command,"getErrorCount"))       {Radio.send(Battery.getErrorCount());return;}
    if(firstWordOf(command,"getVoltage"))          {Radio.send(Battery.getVoltage());return;}
    if(firstWordOf(command,"getCharge"))           {Radio.send(Battery.getCharge());return;}
    if(firstWordOf(command,"getCurrent"))          {Radio.send(Battery.getCurrent());return;}
    if(firstWordOf(command,"getPower"))            {Radio.send(Battery.getPower());return;}
    if(firstWordOf(command,"getCapacity"))         {Radio.send(Battery.getCapacity());return;}
    if(firstWordOf(command,"isCharging"))          {Radio.send(Battery.isCharging());return;}
  }
  if(firstWordOf(command,"Sensor")){//if the first word of command is "Sensor"
    command.remove(0,String("Sensor ").length());//remove the first word of the command
    if(firstWordOf(command,"clearErrors"))         {Radio.send(Sensor.clearErrors());return;}
    if(firstWordOf(command,"getLastError"))        {Radio.send(Sensor.getLastError());return;}
    if(firstWordOf(command,"getErrorCount"))       {Radio.send(Sensor.getErrorCount());return;}
    if(firstWordOf(command,"getTemperature"))      {Radio.send(Sensor.getTemperature());return;}
    if(firstWordOf(command,"getPressure"))         {Radio.send(Sensor.getPressure());return;}
    if(firstWordOf(command,"getHumidity"))         {Radio.send(Sensor.getHumidity());return;}
  }
  if(firstWordOf(command,"System")){//if the first word of command is "Sensor"
    command.remove(0,String("System ").length());//remove the first word of the command
    if(firstWordOf(command,"wakeUp"))              {Radio.send(wakeUp());return;}
    if(firstWordOf(command,"goToSleep"))           {Radio.send(goToSleep());return;}
    if(firstWordOf(command,"restart"))             {Radio.send(restart());return;}
    if(firstWordOf(command,"getAvailableMemory"))  {Radio.send(String(freeMemory()));return;}
    if(firstWordOf(command,"getVersion"))          {Radio.send("V1.1");return;}
    if(firstWordOf(command,"ping"))                {Radio.send("pong");return;}
  }
  Radio.send("EpicError: "+command);return;
}

bool firstWordOf(String command, String key){//is key the first word in command? If so return true; if not return false.
  if(firstWord(command).equals(key)){return true;} //note that this is case sensitive
  else{return false;}
}

String firstWord(String command){//returns the first word of a string, denoted by a space, not including the space.
  int index=command.indexOf(" ");
  if(index==-1){index=command.length();}
  return command.substring(0,index);
}

String wakeUp(){return "Not implemented yet";}
String goToSleep(){return "Not implemented yet";}
String restart(){return "Not implemented yet";}
