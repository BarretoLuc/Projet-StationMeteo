/*#include <SD.h>
#include <EEPROM.h>
#include "DS1307.h"
#include <SPI.h>
#include <Wire.h>



Sd2Card card;
SdVolume volume;
const int chipSelect = 10;
String date;
String myFileName;
unsigned char numrev;
File myFile;
DS1307 clock; //define a object of DS1307 class RTC Clock on I2C port on Grove Shield
String getTimeShort()
{
    String time="";
    clock.getTime();
    time+=String(clock.year, DEC);
    time+=String(clock.month, DEC);
    time+=String(clock.dayOfMonth, DEC);
    

    return time;
}

void enregistrer(String dataString){
  //ergergergergergergerger
  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {

      Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");

    while (1);

  }

  // print the type and size of the first FAT-type volume
  float volumesize;
  volumesize = volume.blocksPerCluster();         // clusters are collections of blocks
  volumesize *= volume.clusterCount();            // we'll have a lot of clusters
  volumesize /= 2;                                // SD card blocks are always 512 bytes (2 blocks are 1KB)
  volumesize /= 1024;
  volumesize /= 1024;

  if(volumesize > 8);
  {
    Serial.println("La carte SD est pleine, veuillez libérer de l'espace");
    while(1);
  }

  Serial.println("L'espace occupé dans la carte SD est (Go): ");
  Serial.println(volumesize);


  date = getTimeShort();
  myFileName += date;
  myFileName += "_";
  myFileName += numrev;
  myFileName += ".LOG";
  myFile = SD.open(myFileName);

  if(myFile)
  {
    unsigned long myFileSize = myFile.size();
    unsigned long myFileSizeRef = EEPROM.read(100);
    Serial.println("YES");

    if(myFileSize < (myFileSizeRef - 256))  //256 octet represente la taille maximum d'une ligne de mesure, alors s'il reste cet espace sur le fichier, nous pouvons écrire dessus
    {
      Serial.println("YESYES");
      myFile.println(dataString);
      myFile.close;
    }
  }
  else
  {
    Serial.println("NO");
    Serial.println("Les données n'ont pas pu être écrite dans un fichier");
  }
}*/