#include <Arduino.h>
#include <EEPROM.h>
#include "main.h"
#include <Wire.h>
#include "DS1307.h"
#include <SPI.h>
#include <string.h>
#include "moyenne.h"
#include "decalage.h"
#include "structure.h"
#include "BME280I2C.h"


BME280I2C bme;
SoftwareSerial SoftSerial(2, 3); // Serial already used for serial communication GPS connected on D2 port on Grove Shield   
unsigned char interMoyenne,interMoyenneInstante;
unsigned char mode; // initialisation de la variable mode (Standard(1),Config(2),Eco(3) ou Maintenance(4))
unsigned long logTemp;
unsigned long tempsPasse;
unsigned long varTemps=0;

struct capteur lum[n];
struct capteur hygro[n];
struct capteur temp[n];
struct capteur pression[n];
signed int valMoy;

void setup() {

  leds.init(); //initialise la led

  SoftSerial.begin(9600); // Open SoftwareSerial for GPS

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial port to connect. Needed for native USB port only
  }

  // Passage en mode configuration si le bouton rouge est pressé au démarrage :
  int etat = digitalRead(bpRougePin);
  if (etat == LOW){
    mode=2; //Mode de la station est mise à "configuration"
    couleurled('J');
    Serial.println(F("Mode configuration"));
  }
  else {
    mode=1; //Mode de la station est mise à "standard"
    couleurled('V');
    Serial.println(F("Mode Standard"));
  }

  interruption(); // Initialisation interruption

  pinMode(bpRougePin, INPUT); // Initialisation le bouton rouge
  pinMode(bpVertPin, INPUT); // Initialisation le bouton vert

  //Initialize Clock
  DS1307 clock; //define a object of DS1307 class RTC Clock on I2C port on Grove Shield

  clock.begin();
  clock.fillByYMD(2021,10,22);//Oct 22,2021
  clock.fillByHMS(9,0,0);//9:00 00"
  clock.fillDayOfWeek(FRI);//Friday
  clock.setTime();//write time to the RTC chip

  while(!Serial) {} // Attente
  Wire.begin();
  while(!bme.begin())
  {
    Serial.println("Le BME280 est introuvable!");
    //LED ERREUR
  }

  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");
      
  }
}

void loop() {

  switch (mode) {//CHANGEMENT DE MODE 

    case 1: //mode standard
      
      
      //tous les (log_interval) temps :

      logTemp = EEPROM.read(5); // Cherche LOG_INTERVALL dans l'EEPROM
      logTemp = 1; // A SUPPRIMER
      logTemp = logTemp * 60000; //transforme les minutes en milliseconde
      tempsPasse = millis()-varTemps;
      if (1000<tempsPasse){ //logTemp à la place de 1000
        
        varTemps = millis();
        lectureCapteur(&dataString,mode);//Tous les capteurs activés

        Serial.println(dataString);

        interMoyenne++;
        interMoyenneInstante++;
        if (interMoyenne == 10){
          moyenne(lum,&valMoy,n);
          decalageMoyenne(lum,&valMoy,n);
          moyenne(hygro,&valMoy,n);
          decalageMoyenne(hygro,&valMoy,n);
          moyenne(temp,&valMoy,n);
          decalageMoyenne(temp,&valMoy,n);
          moyenne(pression,&valMoy,n);
          decalageMoyenne(pression,&valMoy,n);
          interMoyenne = 0;
          
        }

        //enregistrer(dataString);

      }

    break;

    case 2: //mode configuration

      // si inactif pendant 30min alors passage en mode standard
      configurationParametre();
      delay(2000);
      Serial.println("test3");

    break;
  
    case 3: //mode économique
      
      //tous les x/2 (log interval*2) temps :

      logTemp = EEPROM.read(5); // Cherche LOG_INTERVALL dans l'EEPROM
      logTemp = logTemp * 60000 * 2; // Transforme les minutes en milliseconde et prends les mesures 1 fois sur 2
      tempsPasse = millis()-varTemps;
      if (1000<tempsPasse){ //logTemp à la place de 1000
        
        varTemps = millis();
        lectureCapteur(&dataString,mode); // Certain capteurs activés et prise de mesure du gps 1/2
        Serial.println(dataString);

        interMoyenne++;
        interMoyenneInstante++;
        if (interMoyenne == 10){
          moyenne(lum,&valMoy,n);
          decalageMoyenne(lum,&valMoy,n);
          moyenne(hygro,&valMoy,n);
          decalageMoyenne(hygro,&valMoy,n);
          moyenne(temp,&valMoy,n);
          decalageMoyenne(temp,&valMoy,n);
          moyenne(pression,&valMoy,n);
          decalageMoyenne(pression,&valMoy,n);
          interMoyenne = 0;
        }
        //enregistrer(dataString);

      }
    break;

    case 4: //mode maintenance

      //tous les x(log_interval) temps :

      logTemp = EEPROM.read(5); // Cherche LOG_INTERVALL dans l'EEPROM
      logTemp = logTemp * 60000; // Transforme les minutes en milliseconde
      tempsPasse = millis()-varTemps;
      if (1000<tempsPasse){ //logTemp à la place de 1000
        
        varTemps = millis();
        lectureCapteur(&dataString,mode);//Tous les capteurs activés
        Serial.println(dataString); //Affiche les données des capteurs

        interMoyenne++;
        interMoyenneInstante++;
        if (interMoyenne == 10){
          moyenne(lum,&valMoy,n);
          decalageMoyenne(lum,&valMoy,n);
          moyenne(hygro,&valMoy,n);
          decalageMoyenne(hygro,&valMoy,n);
          moyenne(temp,&valMoy,n);
          decalageMoyenne(temp,&valMoy,n);
          moyenne(pression,&valMoy,n);
          decalageMoyenne(pression,&valMoy,n);
          interMoyenne = 0;
        }
        if (interMoyenneInstante >= 100){
          moyenneMoyenne(lum,n);
          moyenneMoyenne(hygro,n);
          moyenneMoyenne(temp,n);
          moyenneMoyenne(pression,n);
          interMoyenneInstante = 0;
        }

      }
    break;
  }
}

void interruption(){
  attachInterrupt(digitalPinToInterrupt(bpRougePin), bpRouge, CHANGE); // utilise une pin(2) pour l'interruption
  attachInterrupt(digitalPinToInterrupt(bpVertPin), bpVert, CHANGE); // utilise la 2ème pin(3) pour l'interruption
}

unsigned long previousTime, elapsedTime;
volatile bool bascule=false, bascule2=false;
unsigned char histoMode;


void bpRouge(){
  
  bascule = !bascule;
  if (bascule) {
    previousTime=millis();
    elapsedTime=0;
    
  }
  else {
    elapsedTime=millis()-previousTime;
  }
  
  if (((mode==1) || (mode==3)) && elapsedTime>5000){
    histoMode=mode;  
    mode=4;
    couleurled('O');
    Serial.println(F("Mode maintenance"));
  } 
  else if ((mode==4) && elapsedTime>5000){
    if (histoMode==1){
      mode=1;
      couleurled('V');
      Serial.println(F("Mode Standard"));
    }
    else if (histoMode==3){
      mode=3;
      couleurled('B');
      Serial.println(F("Mode économique"));
    }
  } 
}

void bpVert(){
  

  bascule2 = !bascule2;
  if (bascule2) {
    previousTime=millis();
    elapsedTime=0;
  }
  else {
   elapsedTime=millis()-previousTime;
  }
  
  if ((mode==1) && elapsedTime>5000){  
    mode=3;
    couleurled('B');
    Serial.println(F("Mode économique"));
  }
  else {
    if ((mode==3) && elapsedTime>5000){
      mode=1;
      couleurled('V');
      Serial.println(F("Mode Standard"));
    }
  }
}


void couleurled(unsigned char Couleur) // Change la couleur de la led
{
    switch(Couleur){
      case 'R' :
         leds.setColorRGB(0, 255, 0, 0); //Rouge
         break;
      case 'V' :
         leds.setColorRGB(0, 0, 255, 0); //Vert
         break;
      case 'B' :
         leds.setColorRGB(0, 0, 0,255); //Bleu
         break;
      case 'O' :
         leds.setColorRGB(0, 255, 80, 0); //Orange
         break;
      case 'J' :
         leds.setColorRGB(0, 255, 255, 0); //Jaune
         break;
      case 'b' :
         leds.setColorRGB(0, 255, 255, 255); //Blanc
         break;
   }
}

bool t,erreur;
String gpsData;
unsigned int delayX;
 
void lectureCapteur(String *dataString, unsigned char mode){ // si mode economique activé alors certain capteur est désactiver (et prise de mesure du gps 1/2)
// make a string for assembling the data to log:
  *dataString = "";

  // Read time from RTC CLock

  String time = getTime();
  if (!time){
    delayX=EEPROM.read(6);//delay(TIMEOUT)
    delay(delayX*1000);
    time = getTime();
    if (!time){
      *dataString += "NA";
      //LED intermittente rouge et bleue (fréquence 1Hz, durée identique pour les 2 couleurs)
    }
    else {
      *dataString += time;
    }
  }
  else {
    *dataString += time;
  }                                           
  *dataString+=" ; ";
  
  // Luminosity captor reading
  int sensor = analogRead(A0);
  if (!sensor){
    delayX=EEPROM.read(6);//delay(TIMEOUT)
    delay(delayX*1000);
    sensor = analogRead(A0);
    if (!sensor){
      *dataString += "NA";
      erreur = 1;
      decalageErreur(lum,erreur,n);
      //LED intermittente rouge et verte (fréquence 1Hz, durée identique pour les 2 couleurs)
    }
    else {
      *dataString += String(sensor);
      erreur=0;
      decalageErreur(lum,erreur,n);
    }
  }
  else {

    *dataString += String(sensor);
    erreur=0;
    decalageErreur(lum,erreur,n);
  }
  
  *dataString += " ; ";
  decalageValeur(lum,sensor,n);
  
  // GPS Reading
  gpsData="";
  if (SoftSerial.available()) // if data is coming from software serial port ==> data is coming from SoftSerial GPS
  {
    t=true;
    while(t){
      gpsData = SoftSerial.readStringUntil('\n');
      if (gpsData.startsWith("$GPGGA",0)){
        t=false; 
      }
    }
  }
  else {
    delayX=EEPROM.read(6);//delay(TIMEOUT)
    delay(delayX*1000);
    if (SoftSerial.available()){ // if data is coming from software serial port ==> data is coming from SoftSerial GPS
      t=true;
      while(t){
        gpsData = SoftSerial.readStringUntil('\n');
        if (gpsData.startsWith("$GPGGA",0)){
          t=false; 
        }
      }
    }
    else {
      *dataString += "NA";
      //LED intermittente rouge et jaune (fréquence 1Hz, durée identique pour les 2 couleurs)
      
    }
  }
  *dataString += gpsData;

  //Temperature & Hygrométrie & Pression à rajouter
  //si Temerature ou Hygrométrie ou Pression = ø
  //delay(TIMEOUT)
  //si sensor = ø alors Temerature ou Hygrométrie ou Pression = "NA"
  //LED intermittente rouge et verte (fréquence 1Hz, durée identique pour les 2 couleurs)
  
}

void printBME280Data (Stream* client)
{
   float temp(NAN), hum(NAN), pres(NAN);

   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

   bme.read(pres, temp, hum, tempUnit, presUnit);

   client->print("Temp: ");
   client->print(temp);
   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(pres);
   client->println(" Pa");

   delay(2000); 
}
