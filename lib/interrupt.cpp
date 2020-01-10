/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description : Ce code permet l'utilisation des interruptions externes offerte par le ATmega324P qui sont INT0, INT1 et INT2
 * On doit spécifier par le constructeur quel INTx utilisé par un id. L'activation par défaut d'une interrutption sera LOW_LEVEL.
 * 
 * Ainsi, pour initialiser l'interruption INT0, par exemple :
 * 
 * BInterrupt b_int(0); // Par défaut elle agira sur LOW_LEVEL
 * 
 * Pour choisir le type de trigger
 * 
 * BInterrupt b_int(0, ANY_EDGE);
 * 
 */

#include "interrupt.h"

Interrupt::Interrupt(uint8_t id, TYPE_CONTROLE type)
{
    cli();
    initINTX(id);
    initEICRA(type);
    sei();
};

// Cette fonction doit etre utilisée dans une routine d'interruption, on assume qu'un délai de debounce est traité dans la routine.
// Il n'est donc pas nécessaire d'ajouter de debounce explicite ici.
bool Interrupt::isButtonPressed()
{
    return (*PINX & _BV(PX));
};

void Interrupt::initINTX(uint8_t id)
{
    switch (id)
    {
    case 0:
        PX = PD2;
        PINX = &PIND;
        DDRD &= ~_BV(PX);
        ISCn0 = ISC00;
        ISCn1 = ISC01;
        EIMSK |= _BV(INT0);
        break;

    case 1:
        PX = PD3;
        PINX = &PIND;
        DDRD &= ~(_BV(PX));
        ISCn0 = ISC10;
        ISCn1 = ISC11;
        EIMSK |= _BV(INT1);
        break;
    case 2:
        PX = PB2;
        PINX = &PINB;
        DDRB &= ~(_BV(PX));
        ISCn0 = ISC20;
        ISCn1 = ISC21;
        EIMSK |= _BV(INT2);
        break;
    }
};

void Interrupt::initEICRA(TYPE_CONTROLE type)
{
    switch (type)
    {
    case LOW_LEVEL:
        EICRA = 0;
        break;
    case ANY_EDGE:
        EICRA |= _BV(ISCn0);
        break;
    case FALLING_EDGE:
        EICRA |= _BV(ISCn1);
        break;
    case RISING_EDGE:
        EICRA |= _BV(ISCn0);
        EICRA |= _BV(ISCn1);
        break;
    }
};