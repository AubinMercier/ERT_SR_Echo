#ifndef MPU_h
#define MPU_h
#include "Data.h"
#include <Wire.h>
class MPU  {
  public:
  MPU();
  void setupMPU();
  void recordData(Data *d);
  void printdata(const Data *d);
  };
#endif 
 
