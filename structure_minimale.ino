enum {
    WAITING,
    FLIGHT,
    TOUCHDOWN
} State;

void setup() {
  // put your setup code here, to run once:
  
  //initialisation capteurs,memoire,xbee...

}

void loop() {
  switch (State) {
  case WAITING:
    //enregistrement 50Hz
    
    //lire capteur
    //envoyer memoire et SD
    //envoyer XBee

    //detection lift-off
    break;
  case FLIGHT:
    //enregistrement 100Hz
    
    //lire capteur
    //envoyer memoire et SD
    //envoyer XBee

    //detection touchdown
    break;
  case TOUCHDOWN:
    //transfert flash vers SD
    //buzzer
    break;
  }

}
