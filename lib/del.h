/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description : Ce fichier permet d'allumer une LED branchée au port voulue au port 0 et 1 
 * par défaut (PORTB B0 B1)
 * Elle peut etre utiliser de la facon suivante :
 * 
 * DEL del;
 * ou
 * DEL del(A);
 *  
 */

#ifndef DEL_H //empecher de definir deux fois la DEL par erreur
#define DEL_H
#include "headers.h"

enum COULEUR
{
    VERTE,
    ROUGE,
    OFF
};

class DEL
{
public:
    DEL();
    DEL(volatile uint8_t *portx, volatile uint8_t *ddrx);

    void allumerDEL(COULEUR couleurDEL);
    void allumerDEL(COULEUR couleurDEL, uint16_t duree_ms);

private:
    volatile uint8_t *portx_;
    volatile uint8_t *ddrx_;
    uint8_t PinMSB_; // numéro de pin à utiliser sur le portx  (ayant Most Significant Bit)
    uint8_t PinLSB_; // numéro de pin à utiliser sur le portx (ayant Least Significant Bit)
};
#endif
