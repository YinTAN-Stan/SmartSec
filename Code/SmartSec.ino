#include <String.h>
#include "DHT.h"      // la bibliotheque des capteurs de température et d'humidité DHT

#define DHTPIN 7     // 7: le num de la broche numérique(Digital pin) connectée au capteur DHT

#define DHTTYPE DHT22   // le type de capteur DHT utilisé : DHT22

DHT dht(DHTPIN, DHTTYPE);   // initialiser le capteur DHT
int FanPin = 3;       // 3: le num de la broche numérique(Digital pin) connectée au ventilateur
String string_t;    // la valeur de temperature en temps reel de type string
String string_h;    // la valeur d'humidité en temps reel de type string
String data_t_h;    // les données de température et d'humidité envoyées vers l'application Android(SmartSec)
String data_s;     // les données envoyées par l'application Android(SmartSec) vers la carte
float val_hum_max;  // la valuer d'humidité maximale pour le séchage de produit
float val_temp_max; // la valuer de temperature maximale pour le séchage de produit
int temps_sech_max;  // le temps de séchage maximal pour le séchage de produit
unsigned long temps_sech=0;
void setup() {
  Serial.begin(9600);     // carte arduino peut transmettre 9600 bits/s
  dht.begin();    //le capteur commence la lecture de données 
  pinMode(FanPin,OUTPUT); //initialiser le ventilateur
  analogWrite(FanPin,0);  // ventilateur en mode repos au debut
  //initiliser les valeurs de recommendations 
  val_temp_max=0.00;
  val_hum_max=0.00;
  temps_sech_max=0;
  temps_sech=0;
}

void loop() {
  if (Serial.available()>0){
    data_s= Serial.readString(); //lire les données envoyées par l'app
  }
  unsigned long temps_courant = millis(); //le nombre de millisecondes depuis que la carte Arduino a commencé à exécuter le programme
  
  delay(2000);   // la lecture de température et d'humidité chaque  2 sec

  float hum_actuel = dht.readHumidity();   //  la lecture d'humidité
  float temp_actuel = dht.readTemperature();  // la lecture de la température en °C


  // Vérifiez si les lectures ont échoué et quittez tôt (pour réessayer).
  if (isnan(hum_actuel) || isnan(temp_actuel)) {
    Serial.println(F("Impossible de lire à partir du capteur DHT!"));
    return;
  }
  string_t = String(temp_actuel);  // changer le type de valeur de temperature en String 
  string_h=String(hum_actuel);     // changer le type de valeur d'humiditée en String
  
  data_t_h=string_t+"°C"+" "+ string_h+"%"; // concaténation les Strings de température et d'humidité
  
  Serial.print(data_t_h);     //envoyer les données vers l'app

  if (data_s =="piment rouge") { //si le nom de produit envoyé par l'app est : piment rouge
    val_temp_max=86;
    val_hum_max=60;
    temps_sech_max=2;
    temps_sech=temps_courant; //le temps de depart de secher le produit
    analogWrite(FanPin,128); // ventilateur marche à vitesse moyenne
    data_s=" ";
  }
  else if (data_s =="persil") { //si le nom de produit envoyé par l'app est : persil
    val_temp_max=64;
    val_hum_max=60;
    temps_sech_max=2;
    temps_sech=temps_courant;
    analogWrite(FanPin,128);
    data_s=" ";
  }
  else if (data_s =="basilic") {  //si le nom de produit envoyé par l'app est : basilic
    val_temp_max=65;
    val_hum_max=60;
    temps_sech_max=4;
    temps_sech=temps_courant;
    analogWrite(FanPin,128);
    data_s=" ";
  }
  else if (data_s =="menthe") {  //si le nom de produit envoyé par l'app est : menthe
    val_temp_max=58;
    val_hum_max=60;
    temps_sech_max=4;
    temps_sech=temps_courant;
    analogWrite(FanPin,128);
    data_s=" ";
  }
  else if (data_s =="coriandre") {   //si le nom de produit envoyé par l'app est : coriandre
    val_temp_max=62;
    val_hum_max=60;
    temps_sech_max=5;
    temps_sech=temps_courant;
    analogWrite(FanPin,128);
    data_s=" ";
  }
  else if (data_s =="ciboulette") {  //si le nom de produit envoyé par l'app est : ciboulette
    val_temp_max=54;
    val_hum_max=60;
    temps_sech_max=5;
    temps_sech=temps_courant;
    analogWrite(FanPin,128);
    data_s=" ";
  }
  else if (data_s =="citronnelle") {  //si le nom de produit envoyé par l'app est : citronnelle
    val_temp_max=55;
    val_hum_max=60;
    temps_sech_max=5;
    temps_sech=temps_courant;
    analogWrite(FanPin,128);
    data_s=" ";
  }
  else if (data_s =="arret") {   //si la donnée envoyée par l'app est : arret
    val_temp_max=0.00;
    val_hum_max=0.00;
    temps_sech_max=0;
    temps_sech=0;
    analogWrite(FanPin,0);
    data_s=" ";
  }

  if ( (temp_actuel >=  val_temp_max) || (hum_actuel >= val_hum_max) ) {  //si la temp est sup à la temp maximale de séchage ou si l'hum est sup à l'hum maximale de séchage
    analogWrite(FanPin,255);  //ventilateur marche à vitesse maximale
  }
  else if ( (temp_actuel <  val_temp_max) && (hum_actuel < val_hum_max) && (val_temp_max > 0) && (val_hum_max > 0) ) { //si la temp et l'hum < aux vals maximales 
    analogWrite(FanPin,128); //ventilateur marche à vitesse moyenne
  }
  else {
    analogWrite(FanPin,0);  // ventilateur en mode repos
  }
  if ( ((temps_courant-temps_sech) / 3600000) >= temps_sech_max ) {  // arreter le processus de séchage si la durée de séchage de produit est téerminée
    val_temp_max=0;
    val_hum_max=0;
    temps_sech_max=0;
    temps_sech=0;
    analogWrite(FanPin,0);
  }
  

  
}
