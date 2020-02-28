#include  "src/Adafruit_VS1053/Adafruit_VS1053.h"    //Library for audio. Adafruit repository: https://github.com/adafruit/Adafruit_VS1053_Library
#include  <SPI.h>                //Library for communicating via SPI. Reference: https://www.arduino.cc/en/Reference/SPI
#include  <SD.h>                 //Library for SD card. Reference: https://www.arduino.cc/en/Reference/SD
#include  "Arduino.h"            //Standard arduino library
#include  "AudioWrapper.h"       //The header file for this class

#define ACS      6     //VS1053 chip select pin
#define ADCS     10    //VS1053 Data/command select pin
#define ADREQ    9     //VS1053 Data request, ideally an Interrupt pin  
#define ACCS     5     //Card chip select pin

AudioWrapper::AudioWrapper() : VS1053b(ACS, ADCS, ADREQ, ACCS) {}//Constructor- gets called automatically when you make an object from this class. Creates a library object for our VS1053b module.

void AudioWrapper::initialize(){//Call this once before you use any of the other functions below. Configures audio settings.
  if ((VS1053b.begin()&&SD.begin(ACCS))==true) { // initialise the music player
    setVolume("70%");  
    VS1053b.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  //Setup data request interrupt
    return;                                         //return
  }
  errorCount++;                   //If initialization failed, increment the error count
  lastError="Error: AudioWrapper.initialize(); Initialization failed.";//And generate the error
  return;                                       //return
}

String AudioWrapper::clearErrors(){//Replaces the error with "All good; No errors!" and sets errorCount to 0. Returns "OK"
  errorCount=0;                    //Clears errorCount to 0
  lastError="All good; No errors!";//Clears error to default text
  return "OK";
}

String AudioWrapper::getLastError(){//Returns the last error that occured (string)
  return lastError;
}

String AudioWrapper::getErrorCount(){ //Returns the number of errors that have occured since the program start or since clearErrors() was called (String)
  errorCount=errorCount%10000000;//use the remainder to limit the size of the number. The effect is that it'll rollover when it gets too big.
  String count=String(errorCount,DEC);//Convert from number to string
  return count;                       //Return the string
}

String AudioWrapper::beep(){         //Generate a diagnostic beep from the speaker. Returns "OK"
  VS1053b.sineTest(0x44, 10);    //Make a beep to indicate VS1053 is working
  return "OK";
}

String AudioWrapper::setVolume(String volume){//Volume is expressed as a percent in a string. The first characters must be an integer number between 0 and 100, higher numbers being louder volume. example: "85%" If first character is not a number an error will be generated. Returns "OK" or an error
  if(isDigit(volume.charAt(0))) {//if first character is a digit
    int vol=volume.toInt();         //convert to int
    volumePercent=vol;              //store for later
    vol=map(vol,100,0,0,100);       //invert range
    vol=constrain(vol,0,100);       //constrain to 0-100
    VS1053b.setVolume(vol,vol);  //100 is quiet(est), 0 is loudest, values above 100 are quietier but inaudible
    return String("OK");
  }
  errorCount++;                   //increment the error count
  lastError="Error: AudioWrapper.setVolume(); Invalid parameter. Try '70%'";//And generate the error
  return lastError;
}

String AudioWrapper::play(String filename){//Starts playing the MP3 file specified from the SD card. May be called if audio is stopped. Returns "OK" or error
  if(isPlaying()=="true"){       //if already playing audio,
    errorCount++;                        //increment the error count
    lastError="Error: AudioWrapper.play(); Can't play if already playing.";//And generate the error
    return lastError;
  }
  if(isPaused()=="true"){        //if paused
    errorCount++;                        //increment the error count
    lastError="Error: AudioWrapper.play(); Can't play if paused.";//And generate the error
    return lastError;
  }
  if(filename.substring(filename.length()-4).equalsIgnoreCase(".mp3")==false){//if the last 4 characters are not the right file extension, add the right file extension. 
      filename.concat(".mp3");
  }
  if(filename.indexOf('.')!=filename.length()-4){
    filename.concat("InvalidFileName");//a "." anywhere except fourth from the end is invalid, so make the name too long so it will be caught and tagged as invalid
  }
  if((filename.length()>12)){ //if filename is longer than 12 character 8.3 format, it is an invalid file name. 
    errorCount++;                        //invalid filename, increment the error count
    lastError="Error: AudioWrapper.play(); Invalid filename.";//And generate the error
    return lastError;
  }
  char file[] = "track001.mp3";          //dummy file to overwrite in 8.3 format
  filename.toCharArray(file, filename.length()+1);
  if(VS1053b.startPlayingFile(file)==false){ //Starts playing file. Returns false if SD failed to read track
    errorCount++;                     //If SD read failed, increment the error count
    lastError="Error: AudioWrapper.play(); SD read failure.";//And generate the error
    return lastError;
  }
  return "OK";
}

String AudioWrapper::isPlaying(){//returns "true" if audio is currently playing and "false" if it is not
  if(VS1053b.playingMusic==true){return "true";}
  else{return "false";}
}

String AudioWrapper::pause(){//Pauses audio. May be called if audio is playing. Returns "OK" or error
  if(isPaused()=="true"){        //if paused
    errorCount++;                        //increment the error count
    lastError="Error: AudioWrapper.play(); Can't pause if already paused.";//And generate the error
    return lastError;
  }
  if(isStopped()=="true"){       //if stopped
    errorCount++;                        //increment the error count
    lastError="Error: AudioWrapper.play(); Can't pause if stopped.";//And generate the error
    return lastError;
  }
  VS1053b.pausePlaying(true);
  return "OK";
}

String AudioWrapper::resume(){//Resumes audio. May be called if audio is paused. Returns "OK" or error
  if(isPlaying()=="true"){       //if playing
    errorCount++;                        //increment the error count
    lastError="Error: AudioWrapper.play(); Can't resume if already playing.";//And generate the error
    return lastError;
  }
  if(isStopped()=="true"){       //if stopped
    errorCount++;                        //increment the error count
    lastError="Error: AudioWrapper.play(); Can't resume if stopped.";//And generate the error
    return lastError;
  }
  VS1053b.pausePlaying(false);
  return "OK";
}

String AudioWrapper::isPaused(){//Returns "true" when paused and "false" when not paused
  if(VS1053b.paused()==true){return "true";}
  else{return "false";}
}

String AudioWrapper::stop(){//Stops audio. May be called if audio is playing. Returns "OK" or error
  if(isPaused()=="true"){      //if paused
    errorCount++;                        //increment the error count
    lastError="Error: AudioWrapper.play(); Can't stop if paused.";//And generate the error
    return lastError;
  }
  if(isStopped()=="true"){         //if stopped
    errorCount++;                        //increment the error count
    lastError="Error: AudioWrapper.play(); Can't stop if already stopped.";//And generate the error
    return lastError;
  }
  VS1053b.stopPlaying();
  return "OK";
}

String AudioWrapper::isStopped(){//Returns "true" when stopped and "false" when not stopped
  if(VS1053b.stopped()==true){return "true";}
  else{return "false";}
}

String AudioWrapper::getLastFileName(){//Returns a string with the filename of the last audio file passed to the play function in format "track001.mp3"
  return VS1053b.currentTrack.name();
}

String AudioWrapper::getVolume(){
  return String(volumePercent)+'%';
}
