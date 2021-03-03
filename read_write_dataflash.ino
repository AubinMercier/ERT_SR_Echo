#include <SPI.h>
#include "DataFlash.h"
#include "Data.h"

static const int csPin    = 10;
static const int resetPin = 8;
static const int wpPin    = 7;
Data d;
DataFlash dataflash;

void setup()
{
  uint8_t status;
  DataFlash::ID id;

  SPI.begin();

  dataflash.setup(csPin, resetPin, wpPin);
  dataflash.begin();

 status = BIN(00101100)
 id.manufacturer       = 0x1F;
 id.device[0]          = 0x26;
 id.device[1]          = 0x00;
 id.extendedInfoLength = 0x01;
 // Write altitude to buffer 1.
   dataflash.bufferWrite(1, 0);
   SPI.transfer(d->altitude);

   
 // Write orientation to buffer 1.
   dataflash.bufferWrite(1, 0);
  for(int i=0; d->orientation[i] != '\0'; i++)
  {
    SPI.transfer(d->orientation[i]);
  }

 
 // Write acceleration to buffer 1.
  dataflash.bufferWrite(1, 0);
  for(int i=0; d->acceleration[i] != '\0'; i++)
  {
    SPI.transfer(d->acceleration[i]);
  }
  
 // Write temperature to buffer 1.
   dataflash.bufferWrite(1, 0);
   SPI.transfer(d->temperature);

   
 // Write battery_level to buffer 1.
   dataflash.bufferWrite(1, 0);
   SPI.transfer(d->battery_level);

  // Transfer buffer 1 to page 7.
  dataflash.bufferToPage(1, 7);

  // Read page 7.
  dataflash.pageRead(7, 0);
  for(int i=0; i<DF_45DB161_PAGESIZE; i++)
  {
    uint8_t dat = SPI.transfer(0xff);
  }
}
