#include <stdio.h>
// Définition de la strucutre capteur et la taille des tableaux de valeurs :

const char n = 5;

typedef struct capteur 
{
    signed int val;
    bool erreur;
    signed int moyInstant;
}capteur;