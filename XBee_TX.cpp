#include <Arduino.h>
#include "XBee_TX.h"
XBee_TX::XBee_TX(int RX1 , int TX1) : SoftwareSerial(RX1,TX1) {
  }
void XBee_TX::setXBee(int RX1 , int TX1){
  char buffer[11];
  this->print("+++"); //entre en mode commande  
  delay(100);
  
  //attente de la réponse
  char res = 0;
  while(res != '\r'){
    if(this->available()>0){
      res = '\r';
    }
  } 
  
  this->print("ATRE\r"); // restaure les paramètres par défaut
  delay(100);
  this->print(strcat(strcat(strcpy(buffer,"ATID"), PANID), "\r")); // modifie l'adresse du reseau PAN 
  delay(100);
  this->print(strcat(strcat(strcpy(buffer,"ATMY"), MYID), "\r")); // adresse du XBee sur 16 bits
  delay(100);
  this->print(strcat(strcat(strcpy(buffer,"ATDL"), DLID), "\r")); // adresse de destination sur 16 bits 
  delay(100);
  this->print("ATDH0\r"); // met les 32 bits MSB du destinataire à 0 
  delay(100);
  this->print("ATWR\r"); // sauvegarde des modifications
  delay(100);
  this->print("ATCN\r"); // quitte le mode commande
  delay(100);
  }  
void XBee_TX::sendData(const Data *d){
  writeHeader();
  this->write(d->altitude);
  for (int i=0; i<3 ; ++i)
    this->write(d->orientation[i]);
  for (int i=0; i<3 ; ++i)
    this->write(d->acceleration[i]);  
  this->write(d->temperature);
  this->write(d->battery_level);  
  msgCount++;
}

void XBee_TX::sendMessage(String outgoing) {
  writeHeader();
  this->write(outgoing.length());        // add payload length
  this->print(outgoing);                 // add payload
  msgCount++;                           // increment message ID*/
}

void XBee_TX::writeHeader(){
  this->write(DLID);              // add destination address
  this->write(MYID);             // add sender address
  this->write(msgCount);                 // add message ID
}
