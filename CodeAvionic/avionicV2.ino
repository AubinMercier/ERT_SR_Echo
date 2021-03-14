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
unsigned long deltaT;
unsigned long beginTime;
unsigned long T_liftoff;
double normeAcc;
double normeDecollage;
unsigned long T_ascent;

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
  sd.printState(1);
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
    deltaT = 20000;//intervalle d'un cycle en microsecondes
    beginTime = micros();

    //lire capteur
    mpu.recordData(&d);  mpu.printdata(&d);
    bmp.recordData(&d);   bmp.printData(&d);
    //envoyer memoire et SD
    sd.printData(micros(), &d);
    //envoyer XBee
    XBeeTX.sendData(&d);

    //detection lift-off
    normeAcc = sqrt(pow(d.acceleration[0],2)+pow(d.acceleration[1],2)+pow(d.acceleration[2],2));
    normeDecollage = 9999999999; //insérer norme
    if(normeAcc>normeDecollage){
      State = LIFTOFF;
    }

    do{
      currentTime = micros();
    }while(currentTime < beginTime+deltaT);
    
    break;
  case LIFTOFF:
    //ligne lift-off sur memoire
    sd.printState(2);
    
    T_liftoff = millis();
    State = ASCENT;
    break;
  case ASCENT:
    //enregistrement 100Hz
    deltaT = 10000;//intervalle d'un cycle en microsecondes
    beginTime = micros();
    
    //lire capteur
    mpu.recordData(&d);  // mpu.printdata(&d);
    bmp.recordData(&d);  // bmp.printData(&d); 
    //envoyer memoire et SD
    sd.printData(micros(), &d);
    //envoyer XBee
    XBeeTX.sendData(&d);
    
    //detection apogee

    //fail détection:
    T_ascent = 20000; //20sec de montée max ?
    if(millis() > T_ascent+T_liftoff){
      //écrire le fail dans mémoire+SD
      State = DESCENT1;
    }

    do{
      currentTime = micros();
    }while(currentTime < beginTime+deltaT);
    
    break;
  case APOGEE:
    //ligne apogee memoire
    sd.printState(3);
    //trigger first recovery event
    break;
  case DESCENT1:
    //enregistrement 100Hz
    deltaT = 10000; //intervalle d'un cycle en microsecondes
    beginTime = micros();
    
    //lire capteur
    mpu.recordData(&d);  // mpu.printdata(&d);
    bmp.recordData(&d);  // bmp.printData(&d); 
    //envoyer memoire et SD
    sd.printData(micros(), &d);
    //envoyer XBee
    XBeeTX.sendData(&d);
    //detection second recovery event

    do{
      currentTime = micros();
    }while(currentTime < beginTime+deltaT);
    
    break;
  case RECOVERYEVENT:
    //ligne second recovery event
    sd.printState(4);
    break;
  case DESCENT2:
    //enregistrement 100Hz
    deltaT = 10000;//intervalle d'un cycle en microsecondes
    beginTime = micros();
    
    //lire capteur 
    mpu.recordData(&d);  // mpu.printdata(&d);
    bmp.recordData(&d);  // bmp.printData(&d); 
    //envoyer memoire et SD
    sd.printData(micros(), &d);
    //envoyer XBee
    XBeeTX.sendData(&d);
    //detection touchdown

    do{
      currentTime = micros();
    }while(currentTime < beginTime+deltaT);
    break;
  case TOUCHDOWN:
    sd.printState(5);
    //transfert flash vers SD
          
    sd.closeFile();
    tone(buzzer,1000);
    delay(3000);
    noTone(buzzer);
    break;
  }

}
