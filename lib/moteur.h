/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Classe qui s'occupe de faire fonctionner les moteurs. Cette classe permet de dicter aux moteurs quoi faire
 * selon la detection des rubans par le suiveur de ligne.
 * 
*/

#ifndef MOTEUR_H
#define MOTEUR_H

#include "headers.h"
#include "commun.h"
#include "SuiveurLigneCytron_LSS05.h"
#include "sonar.h"
#include "del.h"

enum SENS_HORLOGE
{
    SENS_HORAIRE,
    SENS_ANTIHORAIRE
};

class Moteur
{
    static const uint8_t vitesseParDefaut = 0.40 * 0xFF;
    //valeur trouver par essair erreur pour reajuster la roue gauche du robot 17.
    static const uint8_t vitesseG = vitesseParDefaut * 1.0f;
    static const uint8_t vitesseD = vitesseParDefaut * 1.0f;

public:
    Moteur();
    void avancerBoost();
    void avancer();
    void avancerLigne(uint16_t duree = 1000);
    void avancerProchainAngleDroit(uint16_t duree_ms = 300);
    void avancerProchainAngleDroitCoupure(uint16_t duree_ms = 300);
    void tournerProchaineCondition(SENS_HORLOGE sensHoraire = SENS_HORAIRE, TYPE_DETECTION conditionArret = LIGNE_CENTREE, const uint8_t vitesse = vitesseParDefaut);
    void avancerProchaineCoupure();
    void avancerAjustementMur();
    void avancerAjustementCouloir();
    void avancerProchaineCoupureCentre();
    void avancerProchaineCoupureSansAjustement();
    void reculer();
    void arreter();
    void ajusterPWM(uint8_t roueG, uint8_t roueD);

private:
    void rotationHoraire(const uint8_t vitesse = vitesseParDefaut);
    void rotationAntiHoraire(const uint8_t vitesse = vitesseParDefaut);
    SuiveurLigneCytron_LSS05 suiveurLigne_;
    Sonar sonar_;
    DEL del_;
};

#endif // MOTEUR_H