/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Classe qui implemente la transmission et reception serielle a l'aide de USART
*/

#ifndef MINUTERIE_H
#define MINUTERIE_H

#include "headers.h"

void startTimer0CTC(const uint8_t duration, const uint8_t PBx = PB2);
void stopTimer0CTC();
void partirMinuterie2CTC();
void arreterMinuterie2CTC();

#endif // MINUTERIE_H
