#ifndef SDCard_h
#define SDCard_h
#include "Data.h"
#include <SD.h>
#include <SPI.h>
class SDCard {
  public:
  void setupSDCard();
  void createFile();
  void printData (unsigned long t, Data* d);
  void printState(int state);
  void closeFile();
  private:
  const int chipSelect = BUILTIN_SDCARD;
  File dataFile;
  };
#endif  
