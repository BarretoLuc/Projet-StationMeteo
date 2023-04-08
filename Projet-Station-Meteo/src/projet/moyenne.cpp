#include <Arduino.h>
#include <stdio.h>
#include "structure.h"

//Fonction moyenne des valeurs d'un tableau (Utilisable pour calculer une moyenne instantané, sur n valeurs d'un tableau valeur)
void moyenne(struct capteur *Tab, signed int *valMoy, char n)
{
    unsigned char nbError = 0;
    for(int i=0; i<n; i++)
    {
        if(Tab[i].erreur == 0)
        {
            *valMoy += Tab[i].val;
        }
        else
        {
            ++nbError;
        }
    }
    *valMoy = (*valMoy / (n - nbError));
    //Serial.println(*valMoy);
}


void moyenneMoyenne(struct capteur *Tab, char n)
{
    signed int MoyenneInstant;
    for(int i=0; i<n; i++)
    {
        MoyenneInstant += Tab[i].moyInstant;
        //Serial.println(Tab[i].moyInstant);
    }
    MoyenneInstant = MoyenneInstant / n;
    
    Serial.println("La moyenne instantané est ");
    Serial.println(MoyenneInstant);
}