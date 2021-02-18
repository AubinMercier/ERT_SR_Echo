#include <Arduino.h>
#include "MPU.h"
MPU::MPU(){
  } 
void MPU::setupMPU(){
  Wire.beginTransmission(0b1101000);
  Wire.write(0x6B);               //access to power configuration
  Wire.write(0x00);               //sets everything active
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);               //access to accelerometer configuration
  Wire.write(0x00);               //range to +- 2g resolution of 16384 (0x18 for +-16g range resolution of 2048) (0b000XX000)
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);               //access to gyroscope configuartion
  Wire.write(0x00);               //range to +-250degree/s resolution of 131 (0x18 for +-2000 degree/s resolution of 16.4) (0b000XX000)
  Wire.endTransmission();
  }
void MPU::recordData(Data *d){
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);               //access to accerometer data
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 14,true);      
  for(int i=0 ; i<3 ; ++i)        //request acc data from MPU
    d->acceleration[i]=Wire.read() << 8 |Wire.read();
 /*
  d->temperature = Wire.read() << 8 |Wire.read();  //access to temp data
  d->temperature = d->temperature/340 + 36.53;        //temp conversion into degree C*/
  Wire.read();
  Wire.read(); 
  for(int i=0 ; i<3 ; ++i)        //request gyr data
    d->orientation[i]=(Wire.read() << 8 |Wire.read())/131;
  }
void MPU::printdata(const Data   *d){
  Serial.print("AccX = "); Serial.print(d->acceleration[0]);
  Serial.print(" || AccY = "); Serial.print(d->acceleration[1]);
  Serial.print(" || AccZ = "); Serial.print(d->acceleration[2]);
//  Serial.print(" || Temp = "); Serial.print(d->temperature);
  Serial.print(" || GyroX = "); Serial.print(d->orientation[0]);
  Serial.print(" || GyroY = "); Serial.print(d->orientation[1]);
  Serial.print(" || GyroZ = "); Serial.println(d->orientation[2]);
  
  }
