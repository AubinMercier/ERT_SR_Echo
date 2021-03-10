#ifndef BMP_h
#define BMP_h
#include "Data.h"
#include <Wire.h>
#define BMP_Addr 0x76

class BMP{
  public:
  BMP();
  void setupBMP();
  void recordData(Data *d);
  void printData(const Data *d);
  private:
  unsigned int dig_T1,dig_P1;
  signed int dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
  unsigned long adc_T, adc_P;
  signed long t_fine;
  double get_altitude(double press_loc, double press_sea);
  void write_ctrl_registers(uint8_t reg_addr, uint8_t data);
  void read_adc_data();
  signed long get_temperature(signed long adc_T);
  unsigned long get_pressure(signed long adc_P);
  };

#endif
