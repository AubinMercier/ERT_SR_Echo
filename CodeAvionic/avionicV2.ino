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
unsigned long currentTime;

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
    unsigned long deltaT = 20000;//intervalle d'un cycle en microsecondes
    unsigned long beginTime = micros();

    //lire capteur
    mpu.recordData(&d);  mpu.printdata(&d);
    bmp.recordData(&d);   bmp.printData(&d);
    //envoyer memoire et SD
    sd.printData(&d);
    //envoyer XBee
    XBeeTX.sendData(&d);

    //detection lift-off
    double normeAcc = sqrt(pow(d.acceleration[0],2)+pow(d.acceleration[1],2)+pow(d.acceleration[2],2));
    double normeDecollage = ; //insérer norme
    if(normeAcc>normeDecollage){
      State = DECOLLAGE;
    }

    do{
      currentTime = micros();
    }while(currentTime < beginTime+deltaT);
    
    break;
  case LIFTOFF:
    //ligne lift-off sur memoire
    
    unsigned long T_liftoff = millis();
    State = ASCENT;
    break;
  case ASCENT:
    //enregistrement 100Hz
    unsigned long deltaT = 10000;//intervalle d'un cycle en microsecondes
    unsigned long beginTime = micros();
    
    //lire capteur
    mpu.recordData(&d);  // mpu.printdata(&d);
    bmp.recordData(&d);  // bmp.printData(&d); 
    //envoyer memoire et SD
    sd.printData(&d);
    //envoyer XBee
    XBeeTX.sendData(&d);
    
    //detection apogee

    //fail détection:
    unsigned long T_ascent = 20000; //20sec de montée max ?
    if(millis() > T_ascent+T_liftoff){
      //écrire le fail dans mémoire+SD
      State = DESCENT;
    }

    do{
      currentTime = micros();
    }while(currentTime < beginTime+deltaT);
    
    break;
  case APOGEE:
    //ligne apogee memoire
    //trigger first recovery event
    break;
  case DESCENT1:
    //enregistrement 100Hz
    unsigned long deltaT = 10000; //intervalle d'un cycle en microsecondes
    unsigned long beginTime = micros();
    
    //lire capteur
    mpu.recordData(&d);  // mpu.printdata(&d);
    bmp.recordData(&d);  // bmp.printData(&d); 
    //envoyer memoire et SD
    sd.printData(&d);
    //envoyer XBee
    XBeeTX.sendData(&d);
    //detection second recovery event

    do{
      currentTime = micros();
    }while(currentTime < beginTime+deltaT);
    
    break;
  case RECOVERYEVENT:
    //ligne second recovery event
    break;
  case DESCENT2:
    //enregistrement 100Hz
    unsigned long deltaT = 10000;//intervalle d'un cycle en microsecondes
    unsigned long beginTime = micros();
    
    //lire capteur 
    mpu.recordData(&d);  // mpu.printdata(&d);
    bmp.recordData(&d);  // bmp.printData(&d); 
    //envoyer memoire et SD
    sd.printData(&d);
    //envoyer XBee
    XBeeTX.sendData(&d);
    //detection touchdown

    do{
      currentTime = micros();
    }while(currentTime < beginTime+deltaT);
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