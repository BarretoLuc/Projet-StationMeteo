#include <stdio.h>
#include "structure.h"


//Fonction décalage, ajout d'une valeur de capteur (Utilisable pour les valeurs)
void decalageValeur(struct capteur *Tab, int info, char n)
{
    for(int i=0; i<n; i++)
    {

        Tab[i].val = Tab[i+1].val;
    }
    Tab[n-1].val = info;
}


//Fonction décalage, ajout d'une erreur de capteur (Utilisable pour les erreurs)
void decalageErreur(struct capteur *Tab, int info, char n)
{
    for(int i=0; i<n; i++)
    {
        Tab[i].erreur = Tab[i+1].erreur;
    }
    Tab[n-1].erreur = info;
}


//Fonction décalage, ajout d'une moyenne de capteur (Utilisable pour les moyennes)
void decalageMoyenne(struct capteur *Tab, int *info, char n)
{
    for(int i=0; i<n; i++)
    {
        Tab[i].moyInstant = Tab[i+1].moyInstant;
    }
    Tab[n-1].moyInstant = *info;
    *info=0;
}