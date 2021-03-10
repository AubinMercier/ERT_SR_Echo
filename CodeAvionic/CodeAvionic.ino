#include "MPU.h"
#include "BMP.h"
#include "Data.h"
#include "XBee_TX.h"
#include "SDCard.h"
#include <Wire.h>
#define RXPIN 2
#define TXPIN 3
#define BAUDRATE 115200
#define  buzzer 22
MPU mpu;
BMP bmp;
Data d;
SDCard sd;
XBee_TX XBeeTX(RXPIN,TXPIN);
double lastLoop = millis();
double curTime(0);

enum {
    WAITING,
    LIFTOFF,
    ASCENT,
    APOGEE,
    DESCENT1,
    RECOVERYEVENT,
    DESCENT2,
    TOUCHDOWN
} State;

void setup() {
  Wire.begin();
  mpu.setupMPU();
  bmp.setupBMP();
  XBeeTX.begin(BAUDRATE);
  XBeeTX.setXBee(RXPIN,TXPIN);
  sd.setupSDCard();
  sd.createFile();
  Serial.begin(BAUDRATE);
  pinMode(buzzer,OUTPUT);
  tone(buzzer,1000);
  delay(3000);
  noTone(buzzer); 
}

void loop() {
  switch (State) {
  case WAITING:
    //enregistrement 50Hz
    
    //lire capteur
    mpu.recordData(&d);  mpu.printdata(&d);
    bmp.recordData(&d);   bmp.printData(&d);  delay(3000);
    //envoyer memoire et SD
    sd.printData(&d);
    //envoyer XBee
    XBeeTX.sendData(&d);

    //detection lift-off
    break;
  case LIFTOFF:
    //ligne lift-off sur memoire
    break;
  case ASCENT:
    //enregistrement 100Hz
    
    //lire capteur
    mpu.recordData(&d);  // mpu.printdata(&d);
    bmp.recordData(&d);  // bmp.printData(&d); 
    //envoyer memoire et SD
    sd.printData(&d);
    //envoyer XBee
    XBeeTX.sendData(&d);
    //detection apogee
    break;
  case APOGEE:
    //ligne apogee memoire
    //trigger first recovery event
    break;
  case DESCENT1:
    //enregistrement 100Hz
    
    //lire capteur
    mpu.recordData(&d);  // mpu.printdata(&d);
    bmp.recordData(&d);  // bmp.printData(&d); 
    //envoyer memoire et SD
    sd.printData(&d);
    //envoyer XBee
    XBeeTX.sendData(&d);
    //detection second recovery event
    break;
  case RECOVERYEVENT:
    //ligne second recovery event
    break;
  case DESCENT2:
    //enregistrement 100Hz
    
    //lire capteur 
    mpu.recordData(&d);  // mpu.printdata(&d);
    bmp.recordData(&d);  // bmp.printData(&d); 
    //envoyer memoire et SD
    sd.printData(&d);
    //envoyer XBee
    XBeeTX.sendData(&d);
    //detection touchdown
    break;
  case TOUCHDOWN:
    sd.closeFile();
    //transfert flash vers SD
    tone(buzzer,1000);
    delay(3000);
    noTone(buzzer);
    break;
  }

}
