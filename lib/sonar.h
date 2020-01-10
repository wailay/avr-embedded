/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Classe qui implemente le sonar pour detecter des objets a proximites.
 * 
*/

#ifndef SONAR_H
#define SONAR_H

#include "headers.h"
#include "commun.h"

class Sonar
{
public:
    Sonar();
    void envoyerSignal();
    void ecouterEcho();
    uint16_t wavelength;
    int distance;
};
#endif
