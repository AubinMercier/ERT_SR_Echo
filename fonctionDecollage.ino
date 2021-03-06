double normeAcc = sqrt(pow(d.acceleration[0],2)+pow(d.acceleration[1],2)+pow(d.acceleration[2],2));
double normeDecollage = ; //insérer norme
if(normeAcc>normeDecollage){
  State = DECOLLAGE;
}
