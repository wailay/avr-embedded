/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Descritpion : Ce code permet une initialisation plus simple du display LCD a partir du code source fournis lcm_....
 * 
 * Nous avons decider d'utiliser le PORTC pour le display, car c'est le seul port qui est libre au complet et que la
 * mémoire ne sera pas utilisée dans le projet final.
 * 
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "lcm_so1602dtr_m.h"
#include "headers.h"

class Display
{
public:
    Display();
    Display(volatile uint8_t *ddr, volatile uint8_t *port);
    void ecrire(const char *data);
    void effacerAffichage();

private:
    volatile uint8_t *port_, *ddr_;
};
#endif