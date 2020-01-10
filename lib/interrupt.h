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

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "headers.h"

enum TYPE_CONTROLE
{
  LOW_LEVEL,
  RISING_EDGE,
  FALLING_EDGE,
  ANY_EDGE
};

class Interrupt
{
public:
  //TODO
  Interrupt(uint8_t id, TYPE_CONTROLE type = LOW_LEVEL);

  //TODO
  //Ces variables permette de prendre les reference des registres ISCn0, et ISCn1
  volatile uint8_t ISCn1, ISCn0;
  //TODO
  // PINX et PX serviront pour le masque de test dans la fonction isButtonPressed PINX &= (1 << PX) <- tester le bit PX dans PINX
  volatile uint8_t PX;
  volatile uint8_t *PINX;

  bool isButtonPressed();

private:
  void initINTX(uint8_t id);
  void initEICRA(TYPE_CONTROLE type);
};
#endif // INTERRUPT_H