#include "RadioWrapperRelay.h"
#include "src/MemoryFree/MemoryFree.h"//Serial.println(freeMemory());//see how much free ram is left in bytes. If this becomes zero then bad things happen. 

RadioWrapperRelay Radio;//radio object enables wireless communication

void setup(){
  Serial.begin(9600);//serial port to debug to computer
  Radio.initialize();//initialize the radio object
}

void loop(){
  String responseFromRadio=Radio.receive();//get response from radio
  if(responseFromRadio!="No Response Received"){//if there is a response,
    Serial.print(responseFromRadio+'\n');  //relay it to the computer through the serial port
  }
  String commandFromSerial=serialRecieve();//get command from serial
  if(commandFromSerial!="No Command Received"){//if there is a command
    parse(commandFromSerial);//parse the command and either respond back to the computer or relay the command to the robot if appropriate. 
  }
}

bool firstWordOf(String command, String key){//ignoring case, is key the first word in command? If so return true; if not return false.
  if(firstWord(command).equalsIgnoreCase(key)){return true;}
  else{return false;}
}

String firstWord(String command){//returns the first word of a string, denoted by a space, not including the space.
  int index=command.indexOf(" ");
  if(index==-1){index=command.length();}
  return command.substring(0,index);
}

String serialRecieve(){ //gather serial characters and pass them off if we get a carriage return or if the buffer is full (61 characters)
  static char commandBuffer[62]={0};    //Set up a "buffer" for characters. initialize all elements to zero, the null terminator.
  static int sendLength=0;     //used to keep track of length of packet so far
  if(Serial.available()>0){   //If there is any serial data sent from the pc,
    char input=Serial.read();  //Put first byte of incoming serial in input
    if(input=='\b'){//if it is a backspace character, remove the backspace character and the character before it.
      if(sendLength>0){
        sendLength--;
        commandBuffer[sendLength]=0;
      }
      return "No Command Received";
    }
    if(input=='\r'){return "No Command Received";}
    if(input=='\a'){return "No Command Received";}
    if(input=='\t'){return "No Command Received";}
    if(input=='\v'){return "No Command Received";}
    if(input=='\f'){return "No Command Received";}
    if(input=='\e'){return "No Command Received";}
    if(input=='\n'){//If the last input character is a newline, then we have a complete command
      String command=String(commandBuffer);
      sendLength=0;  //Reset the command
      memset(commandBuffer,0,62);//reset buffer to null characters. reference: http://www.cplusplus.com/reference/cstring/memset/
      return command;
    }
    commandBuffer[sendLength]=input;//add it to the end of the commandBuffer
    sendLength++;
    if(sendLength==62){
      String command=String (commandBuffer);
      sendLength=0;  //Reset the command
      memset(commandBuffer,0,62);//reset buffer to null characters. reference: http://www.cplusplus.com/reference/cstring/memset/
      Serial.print("\nCommand too long: '"+command+"'\n");
      return "No Command Received";
    }
  }
  return "No Command Received";
}

void parse(String command){
  command.trim();//remove leading and trailing whitespace
  if(firstWordOf(command,"Relay")){//if the first word of command is "Relay"
    command.remove(0,String("Relay ").length());//remove the first word of the command
    if(command.length()==0)                        {Serial.print("Usage: 'Relay [Radio/System]'\n");return;}
    if(firstWordOf(command,"Radio")){//if the first word of command is "Relay"
      command.remove(0,String("Radio ").length());//remove the first word of the command
      if(command.length()==0)                      {Serial.print("Usage: 'Relay Radio [clearErrors/getLastError/getErrorCount/getSignalStrength/getTemperature]'\n");return;}
      if(firstWordOf(command,"clearErrors"))       {Serial.print(Radio.clearErrors()+'\n');return;}
      if(firstWordOf(command,"getLastError"))      {Serial.print(Radio.getLastError()+'\n');return;}
      if(firstWordOf(command,"getErrorCount"))     {Serial.print(Radio.getErrorCount()+'\n');return;}
      if(firstWordOf(command,"getSignalStrength")) {Serial.print(Radio.getSignalStrength()+'\n');return;}
      if(firstWordOf(command,"getTemperature"))    {Serial.print(Radio.getTemperature()+'\n');return;}
      Serial.print("Unrecognised Command: '"+command+"'\n");return;
    }
    if(firstWordOf(command,"System")){//if the first word of command is "System"
      command.remove(0,String("System ").length());//remove the first word of the command
      if(command.length()==0)                      {Serial.print("Usage: 'Relay System [ping/getVersion/getAvailableMemory]'\n");return;}
      if(firstWordOf(command,"ping"))              {Serial.print("pong\n");return;}
      if(firstWordOf(command,"getVersion"))        {Serial.print("V1.1\n");return;}
      if(firstWordOf(command,"getAvailableMemory")){Serial.print(String(freeMemory())+'\n');return;}
      Serial.print("Unrecognised Command: '"+command+"'\n");return;
    }
    Serial.print("Unrecognised Command: '"+command+"'\n");return;
  }
  if(firstWordOf(command,"Robot")){//if the first word of command is "Robot"
    command.remove(0,String("Robot ").length());//remove the first word of the command
    if(command.length()==0)                        {Serial.print("Usage: 'Robot [Radio/Audio/GPS/Servo/Display/Battery/Sensor/System]'\n");return;}
    if(firstWordOf(command,"Radio")){//if the first word of command is "Radio"
      command.remove(0,String("Radio ").length());//remove the first word of the command
      if(command.length()==0)                        {Serial.print("Usage: 'Robot Radio [clearErrors/getLastError/getErrorCount/getSignalStrength/getTemperature]'\n");return;}
      if(firstWordOf(command,"clearErrors"))         {Radio.send("Radio clearErrors");return;}
      if(firstWordOf(command,"getLastError"))        {Radio.send("Radio getLastError");return;}
      if(firstWordOf(command,"getErrorCount"))       {Radio.send("Radio getErrorCount");return;}
      if(firstWordOf(command,"getSignalStrength"))   {Radio.send("Radio getSignalStrength");return;}
      if(firstWordOf(command,"getTemperature"))      {Radio.send("Radio getTemperature");return;}
      Serial.print("Unrecognised Command: '"+command+"'\n");return;
    }
    if(firstWordOf(command,"Audio")){//if the first word of command is "Audio"
      command.remove(0,String("Audio ").length());//remove the first word of the command
      if(command.length()==0)                        {Serial.print("Usage: 'Robot Audio [clearErrors/getLastError/getErrorCount/beep/setVolume/getVolume/play/isPlaying/pause/resume/isPaused/stop/isStopped/getLastFileName]'\n");return;}
      if(firstWordOf(command,"clearErrors"))         {Radio.send("Audio clearErrors");return;}
      if(firstWordOf(command,"getLastError"))        {Radio.send("Audio getLastError");return;}
      if(firstWordOf(command,"getErrorCount"))       {Radio.send("Audio getErrorCount");return;}
      if(firstWordOf(command,"beep"))                {Radio.send("Audio beep");return;}
      if(firstWordOf(command,"setVolume")){
        command.remove(0,String("setVolume ").length());
        if(command.length()==0)                      {Serial.print("Usage: 'Robot Audio setVolume <percent>'\n");return;}
        Radio.send("Audio setVolume "+command);return;
      }
      if(firstWordOf(command,"getVolume"))           {Radio.send("Audio getVolume");return;}
      if(firstWordOf(command,"play")){
        command.remove(0,String("play ").length());
        if(command.length()==0)                      {Serial.print("Usage: 'Robot Audio play <filename.mp3>'\n");return;}
        Radio.send("Audio play "+command);return;
      }
      if(firstWordOf(command,"isPlaying"))           {Radio.send("Audio isPlaying");return;}
      if(firstWordOf(command,"pause"))               {Radio.send("Audio pause");return;}
      if(firstWordOf(command,"resume"))              {Radio.send("Audio resume");return;}
      if(firstWordOf(command,"isPaused"))            {Radio.send("Audio isPaused");return;}
      if(firstWordOf(command,"stop"))                {Radio.send("Audio stop");return;}
      if(firstWordOf(command,"isStopped"))           {Radio.send("Audio isStopped");return;}
      if(firstWordOf(command,"getLastFileName"))     {Radio.send("Audio getLastFileName");return;}
      Serial.print("Unrecognised Command: '"+command+"'\n");return;
    }
    if(firstWordOf(command,"GPS")){//if the first word of command is "GPS"
      command.remove(0,String("GPS ").length());//remove the first word of the command
      if(command.length()==0)                        {Serial.print("Usage: 'Robot GPS [clearErrors/getLastError/getErrorCount/getLocation/getAltitude/getTime/getDate/getFix/getSatelliteCount]'\n");return;}
      if(firstWordOf(command,"clearErrors"))         {Radio.send("GPS clearErrors");return;}
      if(firstWordOf(command,"getLastError"))        {Radio.send("GPS getLastError");return;}
      if(firstWordOf(command,"getErrorCount"))       {Radio.send("GPS getErrorCount");return;}
      if(firstWordOf(command,"getLocation"))         {Radio.send("GPS getLocation");return;}
      if(firstWordOf(command,"getAltitude"))         {Radio.send("GPS getAltitude");return;}
      if(firstWordOf(command,"getTime"))             {Radio.send("GPS getTime");return;}
      if(firstWordOf(command,"getDate"))             {Radio.send("GPS getDate");return;}
      if(firstWordOf(command,"getFix"))              {Radio.send("GPS getFix");return;}
      if(firstWordOf(command,"getSatelliteCount"))   {Radio.send("GPS getSatelliteCount");return;}
      Serial.print("Unrecognised Command: '"+command+"'\n");return;
    }
    if(firstWordOf(command,"Servo")){//if the first word of command is "Servo"
      command.remove(0,String("Servo ").length());//remove the first word of the command
      if(command.length()==0)                        {Serial.print("Usage: 'Robot Servo [clearErrors/getLastError/getErrorCount/0/1/2/3/4/5/6/7]'\n");return;}
      if(firstWordOf(command,"clearErrors"))         {Radio.send("Servo clearErrors");return;}
      if(firstWordOf(command,"getLastError"))        {Radio.send("Servo getLastError");return;}
      if(firstWordOf(command,"getErrorCount"))       {Radio.send("Servo getErrorCount");return;}
      if(firstWordOf(command,"0")){
        command.remove(0,String("0 ").length());//remove the first word of the command
        if(command.length()==0)                      {Serial.print("Usage: 'Robot Servo 0 [setAngle/getAngle]'\n");return;}
        if(firstWordOf(command,"getAngle"))          {Radio.send("Servo 0 getAngle");return;}
        if(firstWordOf(command,"setAngle")){
          command.remove(0,String("setAngle ").length());//remove the first word of the command
          if(command.length()==0)                    {Serial.print("Usage: 'Robot Servo 0 setAngle <angle>'\n");return;}
          Radio.send("Servo 0 setAngle "+command);return;
        }
        Serial.print("Unrecognised Command: '"+command+"'\n");return;
      }
      if(firstWordOf(command,"1")){
        command.remove(0,String("1 ").length());//remove the first word of the command
        if(command.length()==0)                      {Serial.print("Usage: 'Robot Servo 1 [setAngle/getAngle]'\n");return;}
        if(firstWordOf(command,"getAngle"))          {Radio.send("Servo 1 getAngle");return;}
        if(firstWordOf(command,"setAngle")){
          command.remove(0,String("setAngle ").length());//remove the first word of the command
          if(command.length()==0)                    {Serial.print("Usage: 'Robot Servo 1 setAngle <angle>'\n");return;}
          Radio.send("Servo 1 setAngle "+command);return;
        }
        Serial.print("Unrecognised Command: '"+command+"'\n");return;
      }
      if(firstWordOf(command,"2")){
        command.remove(0,String("2 ").length());//remove the first word of the command
        if(command.length()==0)                      {Serial.print("Usage: 'Robot Servo 2 [setAngle/getAngle]'\n");return;}
        if(firstWordOf(command,"getAngle"))          {Radio.send("Servo 2 getAngle");return;}
        if(firstWordOf(command,"setAngle")){
          command.remove(0,String("setAngle ").length());//remove the first word of the command
          if(command.length()==0)                    {Serial.print("Usage: 'Robot Servo 2 setAngle <angle>'\n");return;}
          Radio.send("Servo 2 setAngle "+command);return;
        }
        Serial.print("Unrecognised Command: '"+command+"'\n");return;
      }
      if(firstWordOf(command,"3")){
        command.remove(0,String("3 ").length());//remove the first word of the command
        if(command.length()==0)                      {Serial.print("Usage: 'Robot Servo 3 [setAngle/getAngle]'\n");return;}
        if(firstWordOf(command,"getAngle"))          {Radio.send("Servo 3 getAngle");return;}
        if(firstWordOf(command,"setAngle")){
          command.remove(0,String("setAngle ").length());//remove the first word of the command
          if(command.length()==0)                    {Serial.print("Usage: 'Robot Servo 3 setAngle <angle>'\n");return;}
          Radio.send("Servo 3 setAngle "+command);return;
        }
        Serial.print("Unrecognised Command: '"+command+"'\n");return;
      }
      if(firstWordOf(command,"4")){
        command.remove(0,String("4 ").length());//remove the first word of the command
        if(command.length()==0)                      {Serial.print("Usage: 'Robot Servo 4 [setAngle/getAngle]'\n");return;}
        if(firstWordOf(command,"getAngle"))          {Radio.send("Servo 4 getAngle");return;}
        if(firstWordOf(command,"setAngle")){
          command.remove(0,String("setAngle ").length());//remove the first word of the command
          if(command.length()==0)                    {Serial.print("Usage: 'Robot Servo 4 setAngle <angle>'\n");return;}
          Radio.send("Servo 4 setAngle "+command);return;
        }
        Serial.print("Unrecognised Command: '"+command+"'\n");return;
      }
      if(firstWordOf(command,"5")){
        command.remove(0,String("5 ").length());//remove the first word of the command
        if(command.length()==0)                      {Serial.print("Usage: 'Robot Servo 5 [setAngle/getAngle]'\n");return;}
        if(firstWordOf(command,"getAngle"))          {Radio.send("Servo 5 getAngle");return;}
        if(firstWordOf(command,"setAngle")){
          command.remove(0,String("setAngle ").length());//remove the first word of the command
          if(command.length()==0)                    {Serial.print("Usage: 'Robot Servo 5 setAngle <angle>'\n");return;}
          Radio.send("Servo 5 setAngle "+command);return;
        }
        Serial.print("Unrecognised Command: '"+command+"'\n");return;
      }
      if(firstWordOf(command,"6")){
        command.remove(0,String("6 ").length());//remove the first word of the command
        if(command.length()==0)                      {Serial.print("Usage: 'Robot Servo 6 [setAngle/getAngle]'\n");return;}
        if(firstWordOf(command,"getAngle"))          {Radio.send("Servo 6 getAngle");return;}
        if(firstWordOf(command,"setAngle")){
          command.remove(0,String("setAngle ").length());//remove the first word of the command
          if(command.length()==0)                    {Serial.print("Usage: 'Robot Servo 6 setAngle <angle>'\n");return;}
          Radio.send("Servo 6 setAngle "+command);return;
        }
        Serial.print("Unrecognised Command: '"+command+"'\n");return;
      }
      if(firstWordOf(command,"7")){
        command.remove(0,String("7 ").length());//remove the first word of the command
        if(command.length()==0)                      {Serial.print("Usage: 'Robot Servo 7 [setAngle/getAngle]'\n");return;}
        if(firstWordOf(command,"getAngle"))          {Radio.send("Servo 7 getAngle");return;}
        if(firstWordOf(command,"setAngle")){
          command.remove(0,String("setAngle ").length());//remove the first word of the command
          if(command.length()==0)                    {Serial.print("Usage: 'Robot Servo 7 setAngle <angle>'\n");return;}
          Radio.send("Servo 7 setAngle "+command);return;
        }
        Serial.print("Unrecognised Command: '"+command+"'\n");return;
      }
      Serial.print("Unrecognised Command: '"+command+"'\n");return;
    }
    if(firstWordOf(command,"Display")){//if the first word of command is "Display"
      command.remove(0,String("Display ").length());//remove the first word of the command
      if(command.length()==0)                        {Serial.print("Usage: 'Robot Display [clearErrors/getLastError/getErrorCount/Line1/Line2/Line3/clear/turnOn/turnOff/getState]'\n");return;}
      if(firstWordOf(command,"clearErrors"))         {Radio.send("Display clearErrors");return;}
      if(firstWordOf(command,"getLastError"))        {Radio.send("Display getLastError");return;}
      if(firstWordOf(command,"getErrorCount"))       {Radio.send("Display getErrorCount");return;}
      if(firstWordOf(command,"clear"))               {Radio.send("Display clear");return;}
      if(firstWordOf(command,"turnOn"))              {Radio.send("Display turnOn");return;}
      if(firstWordOf(command,"turnOff"))             {Radio.send("Display turnOff");return;}
      if(firstWordOf(command,"getState"))            {Radio.send("Display getState");return;}
      if(firstWordOf(command,"Line1")){
        command.remove(0,String("Line1 ").length());//remove the first word of the command
        if(command.length()==0)                      {Serial.print("Usage: 'Robot Display Line1 [getText/setText]'\n");return;}
        if(firstWordOf(command,"getText"))           {Radio.send("Display Line1 getText");return;}
        if(firstWordOf(command,"setText")){
          command.remove(0,String("setText ").length());//remove the first word of the command
          if(command.length()==0)                    {Serial.print("Usage: 'Robot Display Line1 setText <text>'\n");return;}
          Radio.send("Display Line1 setText "+command);return;
        }
        Serial.print("Unrecognised Command: '"+command+"'\n");return;
      }
      if(firstWordOf(command,"Line2")){
        command.remove(0,String("Line2 ").length());//remove the first word of the command
        if(command.length()==0)                      {Serial.print("Usage: 'Robot Display Line2 [getText/setText]'\n");return;}
        if(firstWordOf(command,"getText"))           {Radio.send("Display Line2 getText");return;}
        if(firstWordOf(command,"setText")){
          command.remove(0,String("setText ").length());//remove the first word of the command
          if(command.length()==0)                    {Serial.print("Usage: 'Robot Display Line2 setText <text>'\n");return;}
          Radio.send("Display Line2 setText "+command);return;
        }
        Serial.print("Unrecognised Command: '"+command+"'\n");return;
      }
      if(firstWordOf(command,"Line3")){
        command.remove(0,String("Line3 ").length());//remove the first word of the command
        if(command.length()==0)                      {Serial.print("Usage: 'Robot Display Line3 [getText/setText]'\n");return;}
        if(firstWordOf(command,"getText"))           {Radio.send("Display Line3 getText");return;}
        if(firstWordOf(command,"setText")){
          command.remove(0,String("setText ").length());//remove the first word of the command
          if(command.length()==0)                    {Serial.print("Usage: 'Robot Display Line3 setText <text>'\n");return;}
          Radio.send("Display Line3 setText "+command);return;
        }
        Serial.print("Unrecognised Command: '"+command+"'\n");return;
      }
      Serial.print("Unrecognised Command: '"+command+"'\n");return;
    }
    if(firstWordOf(command,"Battery")){//if the first word of command is "Battery"
      command.remove(0,String("Battery ").length());//remove the first word of the command
      if(command.length()==0)                        {Serial.print("Usage: 'Robot Battery [clearErrors/getLastError/getErrorCount/getVoltage/getCharge/getCurrent/getPower/getCapacity/isCharging]'\n");return;}
      if(firstWordOf(command,"clearErrors"))         {Radio.send("Battery clearErrors");return;}
      if(firstWordOf(command,"getLastError"))        {Radio.send("Battery getLastError");return;}
      if(firstWordOf(command,"getErrorCount"))       {Radio.send("Battery getErrorCount");return;}
      if(firstWordOf(command,"getVoltage"))          {Radio.send("Battery getVoltage");return;}
      if(firstWordOf(command,"getCharge"))           {Radio.send("Battery getCharge");return;}
      if(firstWordOf(command,"getCurrent"))          {Radio.send("Battery getCurrent");return;}
      if(firstWordOf(command,"getPower"))            {Radio.send("Battery getPower");return;}
      if(firstWordOf(command,"getCapacity"))         {Radio.send("Battery getCapacity");return;}
      if(firstWordOf(command,"isCharging"))          {Radio.send("Battery isCharging");return;}
      Serial.print("Unrecognised Command: '"+command+"'\n");return;
    }
    if(firstWordOf(command,"Sensor")){//if the first word of command is "Sensor"
      command.remove(0,String("Sensor ").length());//remove the first word of the command
      if(command.length()==0)                        {Serial.print("Usage: 'Robot Sensor [clearErrors/getLastError/getErrorCount/getTemperature/getPressure/getHumidity]'\n");return;}
      if(firstWordOf(command,"clearErrors"))         {Radio.send("Sensor clearErrors");return;}
      if(firstWordOf(command,"getLastError"))        {Radio.send("Sensor getLastError");return;}
      if(firstWordOf(command,"getErrorCount"))       {Radio.send("Sensor getErrorCount");return;}
      if(firstWordOf(command,"getTemperature"))      {Radio.send("Sensor getTemperature");return;}
      if(firstWordOf(command,"getPressure"))         {Radio.send("Sensor getPressure");return;}
      if(firstWordOf(command,"getHumidity"))         {Radio.send("Sensor getHumidity");return;}
      Serial.print("Unrecognised Command: '"+command+"'\n");return;
    }
    if(firstWordOf(command,"System")){//if the first word of command is "Sensor"
      command.remove(0,String("System ").length());//remove the first word of the command
      if(command.length()==0)                        {Serial.print("Usage: 'Robot System [wakeUp/goToSleep/restart/getAvailableMemory/getVersion/ping]'\n");return;}
      if(firstWordOf(command,"wakeUp"))              {Radio.send("System wakeUp");return;}
      if(firstWordOf(command,"goToSleep"))           {Radio.send("System goToSleep");return;}
      if(firstWordOf(command,"restart"))             {Radio.send("System restart");return;}
      if(firstWordOf(command,"getAvailableMemory"))  {Radio.send("System getAvailableMemory");return;}
      if(firstWordOf(command,"getVersion"))          {Radio.send("System getVersion");return;}
      if(firstWordOf(command,"ping"))                {Radio.send("System ping");return;}
      Serial.print("Unrecognised Command: '"+command+"'\n");return;
    }
    Serial.print("Unrecognised Command: '"+command+"'\n");return;
  }
  if(firstWordOf(command,"Help"))                    {Serial.print("go to www.goo.gl/7wdcSH for commands. Incomplete commands will give usage suggestions. Try 'Robot' or 'Relay'\n");return;}//if the first word of command is "Help"
  if(firstWordOf(command,"?"))                       {Serial.print("go to www.goo.gl/7wdcSH for commands. Incomplete commands will give usage suggestions. Try 'Robot' or 'Relay'\n");return;}//if the first word of command is "?"  
  Serial.print("Unrecognised Command: '"+command+"'\n");return;
}
