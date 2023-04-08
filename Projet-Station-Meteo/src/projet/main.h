#include "ChainableLED.h"
#include <SoftwareSerial.h>

#define bpRougePin 3    
#define bpVertPin 2

extern unsigned long logTemp;
extern unsigned char mode; // initialisation de la variable mode (Standard(1),Config(2),Eco(3) ou Maintenance(4))
extern unsigned long tempsPasse;
extern unsigned long varTemps;
String dataString;
ChainableLED leds(7, 8, 1);




//void couleurled(char Couleur);
void interruption();
void bpRouge();
void bpVert();
void configurationParametre();
void couleurled(unsigned char Couleur);
void lectureCapteur(String *dataString, unsigned char mode);
void enregistrer (String dataString);
void printBME280Data (Stream* client);

String getTime();
String getTimeShort();