#ifndef SDCard_h
#define SDCard_h
#include "Data.h"
#include <SD.h>
#include <SPI.h>
class SDCard {
  public:
  void setupSDCard();
  void createFile();
  void printData (Data* d);
  void closeFile();
  private:
  const int chipSelect = BUILTIN_SDCARD;
  File dataFile;
  };
#endif  
