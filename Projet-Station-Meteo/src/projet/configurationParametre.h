/** 
 * DEFINE CHAR
**/

#define LUMIN 1
#define TEMP_AIR 2
#define HYGR 3
#define PRESSURE 4

#define LOG_INTERVALL 5
#define TIMEOUT 6
#define VERSION 7

#define MIN_TEMP_AIR 8
#define MAX_TEMP_AIR 9
#define HYGR_MINT 10
#define HYGR_MAXT 11

/** 
 * DEFINE INT
**/

#define FILE_MAX_SIZE 100
#define LUMIN_LOW 102
#define LUMIN_HIGH 104
#define PRESSURE_MIN 106
#define PRESSURE_MAX 108

/** 
 * VARIABLE
**/

uint8_t JOUR_SEMAINE;
unsigned char sortie,HEURE,MINUTE,SECONDE,JOUR,MOIS,lectureParametre;
unsigned int ANNEE;
signed int valeurParametre;

/** 
 * FONCTION
**/

void eeprom_reset(void);
void eeprom_write_int(int addr, int n);
int eeprom_read_int(int addr, bool s);