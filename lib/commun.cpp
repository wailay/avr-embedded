/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * 
 * Description : Permet d'utiliser _delay_ms et _delay_us avec une variable non constante apres la compilation
 *  
 */

#include "commun.h"

namespace commun
{
void delai_ms(const uint16_t duree_ms = 1000)
{
    for (uint16_t i = 0; i < duree_ms; i++)
        _delay_ms(1);
}

void delai_us(const uint16_t duree_us = 10)
{
    for (uint16_t i = 0; i < duree_us; i++)
        _delay_us(1);
}
} // namespace commun