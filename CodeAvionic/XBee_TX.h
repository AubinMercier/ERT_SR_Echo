#ifndef XBee_TX_h
#define XBee_TX_h
#include "Data.h"
#include <SoftwareSerial.h>

class XBee_TX :public SoftwareSerial {
  public:
  XBee_TX (int RX1 , int TX1);
  void setXBee(int RX1 , int TX1);
  void sendData(const Data *d);
  void sendMessage(String outgoing);
  private:
  void writeHeader();
  String outgoing;              // outgoing message
  byte msgCount = 0;            // count of outgoing messages
  const char *PANID = "1111"; //adresse réseau
  const char *MYID = "1001";  //adresse du XBee
  const char *DLID = "1002"; // adresse de destination
};
#endif 
