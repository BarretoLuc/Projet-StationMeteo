#include <Arduino.h>
#include <EEPROM.h>
#include "DS1307.h"
#include "configurationParametre.h"
#include <math.h>
#include <stdlib.h>


/*
  Remise à zéro des données
 */
void parametreParDefaut(void){

    EEPROM.write(LUMIN,1);
    eeprom_write_int(LUMIN_LOW,255);
    eeprom_write_int(LUMIN_HIGH,768);
    EEPROM.write(TEMP_AIR,1);
    EEPROM.write(MIN_TEMP_AIR,-10);
    EEPROM.write(MAX_TEMP_AIR,60);
    EEPROM.write(HYGR,1);
    EEPROM.write(HYGR_MINT,0);
    EEPROM.write(HYGR_MAXT, 50);
    EEPROM.write(PRESSURE,1);
    eeprom_write_int(PRESSURE_MIN,850);
    eeprom_write_int(PRESSURE_MAX,1080);
    EEPROM.write(LOG_INTERVALL,10);
    eeprom_write_int(FILE_MAX_SIZE,4096);
    EEPROM.write(VERSION,0);
    EEPROM.write(TIMEOUT,30);

}

/**
 * param addr -> EEPROM Memory Address
 * param n -> Value to write
 */

void eeprom_write_int(int addr, int n) {
    unsigned char byte1 = n & 0xFF;
    unsigned char byte2 = n >> 8 & 0xFF;
    EEPROM.write(addr, byte1);
    EEPROM.write(addr+1, byte2);
}

/*
 * param addr -> EEPROM Memory Address
 * param s -> True signed | False unsigned
 * return int -> Integer value
 */
int eeprom_read_int(int addr, bool s) {
    unsigned char byte1 = EEPROM.read(addr);
    unsigned char byte2 = EEPROM.read(addr+1);
    if (s) {
        signed int n_u = (byte2 << 8) + byte1;
        return n_u;
    } else {
        unsigned int n_s = (byte2 << 8) + byte1;
        return n_s;
    }
}

void configurationParametre(){

    //Modifier les variables à partir d'une console sur l'interface série du microcontrôleur(UART) :
    sortie = 1;
    DS1307 clock; //define a object of DS1307 class RTC Clock on I2C port on Grove Shield
    while (sortie != 0)
    {
        Serial.println(F("Quel paramètre souhaitez-vous changer ? (chiffre_du_paramètre) ou souhaitez-vous partir du mode config ? (0)"));
        while(Serial.available() < 1){ //attends la saisie du paramètre
            delay(500);
        }
        lectureParametre = Serial.read();
        
        Serial.println(lectureParametre);

        switch (lectureParametre)
        {
            case 0: //sortir du mode configuration
                sortie = 0;
            break;

            case LUMIN: //LUMI {0-1} 1
                Serial.println(F("Entrer la valeur du parametre :"));
                valeurParametre = Serial.read();
                if ((valeurParametre == 0) || (valeurParametre == 1)){ //regarde si la valeur est dans le domaine de définition
                    EEPROM.write(LUMIN, valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case LUMIN_LOW: //LUMIN_LOW {0-1023} 255 
                Serial.println(F("Entrer la valeur du parametre :"));
                valeurParametre = Serial.read();
                if ((valeurParametre >= 0) && (valeurParametre <= 1023)){
                    eeprom_write_int(LUMIN_LOW,valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case LUMIN_HIGH: //LUMIN_HIGH {0-1023} 768
                Serial.println(F("Entrer la valeur du parametre :"));
                valeurParametre = Serial.read();
                if ((valeurParametre >= 0) && (valeurParametre <= 1023)){
                    eeprom_write_int(LUMIN_HIGH,valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case TEMP_AIR: //TEMP_AIR {0,1} 1
                Serial.println(F("Entrer la valeur du parametre :"));
                valeurParametre = Serial.read();
                if ((valeurParametre == 0) || (valeurParametre == 1)){
                    EEPROM.write(TEMP_AIR, valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case MIN_TEMP_AIR: //MIN_TEMP_AIR {-40-85} -10
                Serial.println(F("Entrer la valeur du parametre :"));
                valeurParametre = Serial.read();
                if ((valeurParametre >= -40) && (valeurParametre <= 85)){
                    EEPROM.write(MIN_TEMP_AIR, valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case MAX_TEMP_AIR: //MAX_TEMP_AIR {-40-85} 60
                Serial.println(F("Entrer la valeur du parametre :"));
                valeurParametre = Serial.read();
                if ((valeurParametre >= -40) && (valeurParametre <= 85)){
                    EEPROM.write(MAX_TEMP_AIR, valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;
        
            case HYGR: //HYGR {0,1} 1
                Serial.println(F("Entrer la valeur du parametre :"));
                valeurParametre = Serial.read();
                if ((valeurParametre == 0) || (valeurParametre == 1)){
                    EEPROM.write(HYGR, valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case HYGR_MINT: //HYGR_MINT {-40-85} 0
                Serial.println(F("Entrer la valeur du parametre :"));
                valeurParametre = Serial.read();
                if ((valeurParametre >= -40) && (valeurParametre <= 85)){
                    EEPROM.write(HYGR_MINT, valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case HYGR_MAXT: //HYGR_MAXT {-40-85} 50
                Serial.println(F("Entrer la valeur du parametre :"));
                valeurParametre = Serial.read();
                if ((valeurParametre >= -40) && (valeurParametre <= 85)){
                    EEPROM.write(HYGR_MAXT, valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case PRESSURE: //PRESSURE {0,1} 1
                Serial.println(F("Entrer la valeur du parametre :"));
                valeurParametre = Serial.read();
                if ((valeurParametre == 0) || (valeurParametre == 1)){
                    EEPROM.write(PRESSURE, valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case PRESSURE_MIN: //PRESSURE_MIN {300-1100} 850
                Serial.println(F("Entrer la valeur du parametre :"));
                valeurParametre = Serial.read();
                if ((valeurParametre >= 300) && (valeurParametre <= 1100)){
                    eeprom_write_int(PRESSURE_MIN,valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case PRESSURE_MAX: //PRESSURE_MAX {300-1100} 1080
                Serial.println(F("Entrer la valeur du parametre :"));
                valeurParametre = Serial.read();
                if ((valeurParametre >= 300) && (valeurParametre <= 1100)){
                    eeprom_write_int(PRESSURE_MAX,valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;
        
            case 12: //configuration de l’heure du jour au format HEURE{0-23}:MINUTE{0-59}:SECONDE{0-59}
                Serial.println(F("Entrer l'heure :"));
                HEURE = Serial.read();
                Serial.println(F("Entrer les minutes :"));
                MINUTE = Serial.read();
                Serial.println(F("Entrer les secondes :"));
                SECONDE = Serial.read();
                if ((HEURE >= 0) && (HEURE <= 23) && (MINUTE >= 0) && (MINUTE <= 59) && (SECONDE >= 0) && (SECONDE <= 59)){
                    clock.fillByHMS(HEURE,MINUTE,SECONDE);
                    Serial.println(F("Valeur enregistée."));
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;
            
            case 13: //configuration de la date du jour au format MOIS{1-12},JOUR{1-31},ANNEE{2000-2099}
                Serial.println(F("Entrer l'année (2 chiffres) :"));
                ANNEE = Serial.read();
                Serial.println(F("Entrer le mois :"));
                MOIS = Serial.read();
                Serial.println(F("Entrer le jour :"));
                JOUR = Serial.read();
                if ((MOIS >= 1) && (MOIS <= 12) && (JOUR >= 1) && (JOUR <= 31) && (ANNEE >= 0) && (SECONDE <= 99)){
                    clock.fillByYMD(ANNEE,MOIS,JOUR);
                    Serial.println(F("Valeur enregistée."));
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case 14: //configuration du jour de la semaine{MON,TUE,WED,THU,FRI,SAT,SUN}
                Serial.println(F("Entrer le jour de la semaine (MON,TUE,WED,THU,FRI,SAT,SUN) :"));
                JOUR_SEMAINE = Serial.read();
                if ((JOUR_SEMAINE == MON) || (JOUR_SEMAINE == TUE) || (JOUR_SEMAINE == WED) || (JOUR_SEMAINE == THU) || (JOUR_SEMAINE == FRI) || (JOUR_SEMAINE == SAT) || (JOUR_SEMAINE == SUN)){
                    clock.fillDayOfWeek(JOUR_SEMAINE);
                    Serial.println(F("Valeur enregistée."));
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case LOG_INTERVALL: //définition de l’intervalle entre 2 mesures, 10 minutes par défaut.
                Serial.println(F("Entrer la valeur de l'intervalle entre les 2 mesures comprise entre 1s et 60min :"));
                valeurParametre = Serial.read();
                if ((valeurParametre >= 1) && (valeurParametre <= 60)){
                    EEPROM.write(LOG_INTERVALL, valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case FILE_MAX_SIZE: //définition de la taille maximale (en octets) d'un fichier de log, une taille de 4ko provoque son archivage.
                Serial.println(F("Entrer la valeur de la taille maximale (en octets) d'un fichier de log, une taille de 4ko provoque son archivage :"));
                valeurParametre = Serial.read();
                if ((valeurParametre >= 2000) && (valeurParametre <= 4096)){
                    eeprom_write_int(FILE_MAX_SIZE,valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case 15: //réinitialisation de l’ensemble des paramètres à leurs valeurs par défaut.
                parametreParDefaut();
                Serial.println(F("Valeur enregistée.")); 
            break;

            case VERSION: //affiche la version du programme et un numéro de lot (permet de tracer la production)
                Serial.println(F("Entrer la valeur de la version du programme :"));
                valeurParametre = Serial.read();
                if (valeurParametre >= 0){
                    EEPROM.write(VERSION, valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            case TIMEOUT: //durée (en s) au bout de laquelle l’acquisition des données d’un capteur est abandonnée.
                Serial.println(F("Entrer la valeur de la version du programme :"));
                valeurParametre = Serial.read();
                if (valeurParametre > 0){
                    EEPROM.write(TIMEOUT, valeurParametre);
                    Serial.println(F("Valeur enregistée."));
                    valeurParametre = -32700;
                }
                else {
                    Serial.println(F("Erreur : la valeur n'est pas définit."));
                }
            break;

            default:
                Serial.println(F("Erreur dans le nom du paramètre. Veuillez réessayer."));
            break;

        }

        //sortie au bout de 30 min d'inactivité sortie=0

    }
}