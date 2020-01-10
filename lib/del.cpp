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

#include "del.h"
#include "commun.h"

// Utilise les pins 0 et 1 du port B pour la del
DEL::DEL()
{
    PinMSB_ = 1;
    PinLSB_ = 0;
    DDRB |= (1 << PinMSB_) | (1 << PinLSB_); // PORTB1 et PORTB0 en mode sortie
    ddrx_ = &DDRB;
    portx_ = &PORTB;
}

// Utilise les pins 0 et 1 d'un port spécifier pour la del
DEL::DEL(volatile uint8_t *portx, volatile uint8_t *ddrx)
{
    PinMSB_ = 1;
    PinLSB_ = 0;
    portx_ = portx;
    ddrx_ = ddrx;
    *ddrx_ |= (1 << PinMSB_) | (1 << PinLSB_);
}

//Cette fonction utilise par defaut les ports PX0 et PX1 d'un certain PORTX
void DEL::allumerDEL(COULEUR couleurDEL)
{
    switch (couleurDEL)
    {
    case VERTE:
        *portx_ &= ~(1 << PinMSB_);
        *portx_ |= (1 << PinLSB_);
        break;
    case ROUGE:
        *portx_ &= ~(1 << PinLSB_);
        *portx_ |= (1 << PinMSB_);
        break;
    case OFF:
        *portx_ &= ~(1 << PinLSB_) & ~(1 << PinMSB_);
        break;
    }
};

void DEL::allumerDEL(COULEUR couleurDEL, uint16_t duree_ms)
{
    switch (couleurDEL)
    {
    case VERTE:
        *portx_ &= ~(1 << PinMSB_);
        *portx_ |= (1 << PinLSB_);
        commun::delai_ms(duree_ms);
        *portx_ &= ~(1 << PinLSB_) & ~(1 << PinMSB_);
        break;
    case ROUGE:
        *portx_ &= ~(1 << PinLSB_);
        *portx_ |= (1 << PinMSB_);
        commun::delai_ms(duree_ms);
        *portx_ &= ~(1 << PinLSB_) & ~(1 << PinMSB_);
        break;
    case OFF:
        *portx_ &= ~(1 << PinLSB_) & ~(1 << PinMSB_);
        commun::delai_ms(duree_ms);
        break;
    }
};