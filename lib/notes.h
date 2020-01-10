/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Classe qui implemente le piezoelectrique pour generer des son a l'aide de la minuterie 0 CTC.
 * 
*/

#ifndef NOTES_H
#define NOTES_H

#include "headers.h"
#include "commun.h"
#include "minuterie.h"

void playNote(uint8_t note);
void playNote(uint8_t note, uint16_t duree_ms, uint8_t nRepetition);
void stopNote();

#endif // NOTES_H