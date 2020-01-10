/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * 
 * Description : Permet d'utiliser _delay_ms et _delay_us avec une variable non constante apres la compilation
 *  
 */

#ifndef COMMUN_H
#define COMMUN_H
#include "headers.h"

namespace commun
{
void delai_ms(const uint16_t duree_ms);
void delai_us(const uint16_t duree_us);
} // namespace commun

#endif // COMMUN_H
