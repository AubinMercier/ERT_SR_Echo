#include <Arduino.h>
#include "SDCard.h"

void SDCard::setupSDCard(){
  //Serial.print("Initializing SD card...");
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    //Serial.println("Card failed, or not present");
    // don't do anything more:
    tone(22,1000,20000);//buzzer si problème d'initialisation
    return;
  }
  //Serial.println("card initialized.");
  }

void SDCard::createFile(){
  dataFile = SD.open("data.csv", FILE_WRITE);
  String dataString = "Altitude,OrientationX,OrientationY,OrientationZ,AccelerationX,AccelerationY,AccelerationZ,Temperature,Battery,";
  if(dataFile){
    dataFile.println(dataString);
  }
  }

void SDCard::printData (Data * d){
  String dataString = d->altitude + ",";
  for(int i=0 ; i<3 ; ++i)        //request gyr data from MPU
    dataString += String(d->orientation[i])+ ",";
  for(int i=0 ; i<3 ; ++i)        //request acc data
    dataString += String(d->acceleration[i])+ ",";
  dataString = d->temperature + ",";  
  dataString = d->battery_level + ","; 
  if (dataFile) {
    dataFile.println(dataString);
    
    // print to the serial port too:
    //Serial.println(dataString);
  }  
  // if the file isn't open, pop up an error:
  else {
    //Serial.println("error opening datalog.txt");
  }  
}

void SDCard::closeFile(){
  dataFile.close();
  }
