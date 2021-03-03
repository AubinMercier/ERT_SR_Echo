enum {
    WAITING,
    LIFTOFF,
    ASCENT,
    APOGEE,
    DESCENT1,
    RECOVERYEVENT,
    DESCENT2,
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
  case LIFTOFF:
    //ligne lift-off sur memoire
    break;
  case ASCENT:
    //enregistrement 100Hz
    
    //lire capteur
    //envoyer memoire et SD
    //envoyer XBee

    //detection apogee
    break;
  case APOGEE:
    //ligne apogee memoire
    //trigger first recovery event
    break;
  case DESCENT1:
    //enregistrement 100Hz
    
    //lire capteur
    //envoyer memoire et SD
    //envoyer XBee

    //detection second recovery event
    break;
  case RECOVERYEVENT:
    //ligne second recovery event
    break;
  case DESCENT2:
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
