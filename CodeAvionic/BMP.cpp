#include <Arduino.h>
#include "BMP.h"
BMP::BMP(){
  }
void BMP::setupBMP(){
 
  uint8_t osrs_t = 1;       //temperature oversampling x 1
  uint8_t osrs_p = 1;       //pressure oversampling x 1
  uint8_t mode = 3;         //bmp activated (0 if sleep mode)
  uint8_t t_sb = 5;         //standby time 1000 ms (0 = 0.5ms, 1 = 62.5ms, 2 = 125ms, 3 = 250ms, 4 = 500ms, 5 = 1000ms)
  uint8_t filter = 0;       //filter off
  uint8_t spi3w_en = 0;     //SPI Off

  uint8_t ctrl_meas_reg = (osrs_t << 5) | (osrs_p << 2) | mode;
  uint8_t config_reg    = (t_sb << 5) | (filter << 2) | spi3w_en;  

  write_ctrl_registers(0xF4, ctrl_meas_reg);
  write_ctrl_registers(0xF5, config_reg);
  
  uint8_t data[24], i = 0;
  Wire.beginTransmission(BMP_Addr);
  Wire.write(0x88);
  Wire.endTransmission();
  Wire.requestFrom(BMP_Addr, 24);
  while(Wire.available()){
    data[i] = Wire.read();
    i++;
  }
  
  dig_T1 = (data[1] << 8) | data[0];
  dig_T2 = (data[3] << 8) | data[2];
  dig_T3 = (data[5] << 8) | data[4];
  dig_P1 = (data[7] << 8) | data[6];
  dig_P2 = (data[9] << 8) | data[8];
  dig_P3 = (data[11]<< 8) | data[10];
  dig_P4 = (data[13]<< 8) | data[12];
  dig_P5 = (data[15]<< 8) | data[14];
  dig_P6 = (data[17]<< 8) | data[16];
  dig_P7 = (data[19]<< 8) | data[18];
  dig_P8 = (data[21]<< 8) | data[20];
  dig_P9 = (data[23]<< 8) | data[22];
}
 
void BMP::recordData(Data *d){
  d->temperature=(double)get_temperature(adc_T)/100;
  double pressure =(double)get_pressure(adc_P)/100;
  d->altitude = get_altitude(pressure, 1005);
  }
void BMP::printData(const Data *d){
  Serial.print("Temperature "); Serial.print(d->temperature);
  Serial.print(" || Altitude "); Serial.print(d->altitude);
  } 

void BMP::write_ctrl_registers(uint8_t reg_addr, uint8_t data){
  Wire.beginTransmission(BMP_Addr);
  Wire.write(reg_addr);
  Wire.write(data);
  Wire.endTransmission();
}

void BMP::read_adc_data(){
  uint32_t data[6], i = 0;                      
  Wire.beginTransmission(BMP_Addr);
  Wire.write(0xF7);
  Wire.endTransmission();
  Wire.requestFrom(BMP_Addr, 6);
  while(Wire.available()){
    data[i] = Wire.read();
    i++;    
  }
  adc_P = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
  adc_T = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
}
double BMP::get_altitude(double press_loc, double press_sea){
  double altitude = 0;
  altitude = 44330 * (1- pow(press_loc/press_sea, 1/5.255));
  return altitude;
}

signed long BMP::get_temperature(signed long adc_T){
   
    signed long int var1, var2, t;
    var1 = ((((adc_T >> 3) - ((signed long int)dig_T1<<1))) * ((signed long int)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((signed long int)dig_T1)) * ((adc_T>>4) - ((signed long int)dig_T1))) >> 12) * ((signed long int)dig_T3)) >> 14;
    t_fine = var1 + var2;
    t = (t_fine * 5 + 128) >> 8;
    return t;
}

unsigned long BMP::get_pressure(signed long adc_P){

    signed long int var1, var2;
    unsigned long int p;
    var1 = (((signed long int)t_fine)>>1) - (signed long int)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11) * ((signed long int)dig_P6);
    var2 = var2 + ((var1*((signed long int)dig_P5))<<1);
    var2 = (var2>>2)+(((signed long int)dig_P4)<<16);
    var1 = (((dig_P3 * (((var1>>2)*(var1>>2)) >> 13)) >>3) + ((((signed long int)dig_P2) * var1)>>1))>>18;
    var1 = ((((32768+var1))*((signed long int)dig_P1))>>15);
    if (var1 == 0)
    {
        return 0;
    }   
    p = (((unsigned long int)(((signed long int)1048576)-adc_P)-(var2>>12)))*3125;
    if(p<0x80000000)
    {
       p = (p << 1) / ((unsigned long int) var1);   
    }
    else
    {
        p = (p / (unsigned long int)var1) * 2;   
    }
    var1 = (((signed long int)dig_P9) * ((signed long int)(((p>>3) * (p>>3))>>13)))>>12;
    var2 = (((signed long int)(p>>2)) * ((signed long int)dig_P8))>>13;
    p = (unsigned long int)((signed long int)p + ((var1 + var2 + dig_P7) >> 4));
    return p;
}     
