

#include <SPI.h>
#include "DataFlash.h"

static const int csPin    = 10;
static const int resetPin = 8;
static const int wpPin    = 7;

DataFlash dataflash;

void setup()
{
  uint8_t status;
  DataFlash::ID id;

  const char* dummyMessage = "Hello world";

  SPI.begin();

  dataflash.setup(csPin, resetPin, wpPin);
  dataflash.begin();


    status = BIN(00101100)
    id.manufacturer       = 0x1F;
    id.device[0]          = 0x26;
    id.device[1]          = 0x00;
    id.extendedInfoLength = 0x01;

  // Write "Hello world" to buffer 1.
  dataflash.bufferWrite(1, 0);
  for(int i=0; dummyMessage[i] != '\0'; i++)
  {
    SPI.transfer(dummyMessage[i]);
  }

  // Transfer buffer 1 to page 7.
  dataflash.waitUntilReady();
  dataflash.bufferToPage(1, 7);
Serial.print("The test = ");
dataflash.pageToBuffer(0, 1);
dataflash.bufferRead(1, 0);
Serial.print(SPI.transfer(0xff));
Serial.print(SPI.transfer(0xff));
Serial.println("");
  // Read page 7.
  dataflash.pageRead(7, 0);
  for(int i=0; i<DF_45DB161_PAGESIZE; i++)
  {
    uint8_t data = SPI.transfer(0xff);
  }
}
