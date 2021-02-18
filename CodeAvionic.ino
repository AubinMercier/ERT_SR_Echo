#include "MPU.h"
#include "BMP.h"
#include "Data.h"
#include "XBee_TX.h"
#include <Wire.h>
#define RXPIN 2
#define TXPIN 3
#define BAUDRATE 115200

MPU mpu;
BMP bmp;
Data d;
XBee_TX XBeeTX(RXPIN,TXPIN);
double lastLoop = millis();
double curTime(0);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  mpu.setupMPU();
  bmp.setupBMP();
  XBeeTX.begin(BAUDRATE);
 // XBeeTX.setXBee(RXPIN,TXPIN);
  Serial.begin(BAUDRATE);
}  
void loop() {
  // put your main code here, to run repeatedly:
  mpu.recordData(&d);
  mpu.printdata(&d);
  bmp.recordData(&d);
  bmp.printData(&d);  
  XBeeTX.sendData(&d);
  curTime = millis();
  Serial.println(curTime - lastLoop);
  lastLoop = curTime;
  delay(2000);
}
