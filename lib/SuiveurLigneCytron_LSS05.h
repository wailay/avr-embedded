/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Classe qui traite les donnees du suiveur de ligne. Elle s'occupe de corriger le PWM des moteurs
 * et d'avertir le moteur des differents etats du robots sur les rubans.
 * 
*/

#ifndef SUIVEUR_LIGNE_CYTRON_LSS05_H
#define SUIVEUR_LIGNE_CYTRON_LSS05_H

#include "headers.h"
#include "commun.h"

enum TYPE_DETECTION
{
    LIGNE,
    COUPURE,
    LIGNE_CENTREE,
    COUPURE_CENTRE,
    ANGLE_DROIT,
    ANGLE_DROIT_COTE_G,
    ANGLE_DROIT_COTE_D,
    EXTREMITE_G,
    EXTREMITE_D
};

// Type d'etat du suiveur de ligne (et du robot) selon la detection de la bande noir
enum NIVEAU_DEVIATION
{
    TROP_A_GAUCHE_LV4, // x x x 0 1
    TROP_A_GAUCHE_LV3, // x x x 1 1
    TROP_A_GAUCHE_LV2, // x x x 1 0
    TROP_A_GAUCHE_LV1, // x x 1 1 x
    CENTREE,           // x x 1 x x
    TROP_A_DROITE_LV1, // x 1 1 x x
    TROP_A_DROITE_LV2, // 0 1 x x x
    TROP_A_DROITE_LV3, // 1 1 x x x
    TROP_A_DROITE_LV4, // 1 0 x x x
    EXTREMITES,        // 1 x x x 1 //Cas special pour le couloir
};

class SuiveurLigneCytron_LSS05
{
public:
    SuiveurLigneCytron_LSS05();
    void corrigerPWM(uint8_t vitesseG, uint8_t vitesseD); // ajuste le PWM pour suivre la ligne.
    bool corrigerPWMCouloir(uint8_t vitesseG, uint8_t vitesseD);
    bool verifierCondition(TYPE_DETECTION condition);
    bool verifierConditionDebounce(TYPE_DETECTION condition, uint16_t duree_ms = 200);

private:
    NIVEAU_DEVIATION verifierNiveauDeviation_();
};
#endif // SUIVEUR_LIGNE_CYTRON_LSS05_H