/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Classe qui s'occupe de faire fonctionner les moteurs. Cette classe permet de dicter aux moteurs quoi faire
 * selon la detection des rubans par le suiveur de ligne.
 * 
*/

#include "moteur.h"

Moteur::Moteur()
{
    DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7); //Mise en sortie des ports
}

void Moteur::avancerBoost()
{
    static const uint8_t vitesseBoost = 0.7 * 0xFF;
    ajusterPWM(vitesseBoost, vitesseBoost); // ceci est pourcontrer la friction
    commun::delai_ms(30);
    arreter();
}
void Moteur::avancer()
{

    PORTD &= ~(1 << PD7) & ~(1 << PD6);
    ajusterPWM(vitesseG, vitesseD);
}

//Cette fonction va agir comme un suiveur de ligne pour un certain temps et arreter
//cette fonction est utile pour suivre un ruban sans condiction specifique (ex angle droit) et eviter que notre robot sarrete pour rien
void Moteur::avancerLigne(uint16_t duree)
{
    avancer();
    for (uint16_t i = 0; i < duree; i++)
    {
        suiveurLigne_.corrigerPWM(vitesseG, vitesseD);
        commun::delai_ms(1);
    }
    arreter();
}

void Moteur::tournerProchaineCondition(SENS_HORLOGE sensHorloge, TYPE_DETECTION conditionArret, const uint8_t vitesse)
{
    if (sensHorloge == SENS_HORAIRE)
    {
        rotationHoraire(vitesse);
    }
    else
    {
        rotationAntiHoraire(vitesse);
    }
    while (!suiveurLigne_.verifierCondition(conditionArret))
        ;

    arreter();
}

void Moteur::avancerProchainAngleDroit(uint16_t duree_ms)
{
    avancer();
    bool estAngleDroit = false;
    while (!estAngleDroit)
    {

        suiveurLigne_.corrigerPWM(vitesseG, vitesseD);
        estAngleDroit = suiveurLigne_.verifierCondition(ANGLE_DROIT);
    }
    // je comprend la logique du code, mais le robot vire trop vers la gauche
    // while(estAngleDroit){
    //     estAngleDroit = suiveurLigne_.verifierCondition(ANGLE_DROIT);
    // }

    commun::delai_ms(duree_ms);
    arreter();
}

void Moteur::avancerProchainAngleDroitCoupure(uint16_t duree_ms)
{
    avancer();
    bool estAngleDroitOuCoupure = false;

    while (!estAngleDroitOuCoupure)
    {

        suiveurLigne_.corrigerPWM(vitesseG, vitesseD);
        estAngleDroitOuCoupure = (suiveurLigne_.verifierConditionDebounce(ANGLE_DROIT, 50) || suiveurLigne_.verifierConditionDebounce(COUPURE));
    }

    commun::delai_ms(duree_ms);
    arreter();
}

void Moteur::avancerProchaineCoupure()
{
    avancer();
    bool estCoupure = false;
    while (!estCoupure)
    {
        suiveurLigne_.corrigerPWM(vitesseG, vitesseD);
        estCoupure = suiveurLigne_.verifierConditionDebounce(COUPURE);
    }

    arreter();
}

void Moteur::avancerProchaineCoupureSansAjustement()
{
    avancer();
    bool estCoupure = false;
    while (!estCoupure)
    {

        estCoupure = suiveurLigne_.verifierConditionDebounce(COUPURE);
    }

    arreter();
}

void Moteur::avancerProchaineCoupureCentre()
{
    avancer();
    bool estCoupureCentre = false;
    while (!estCoupureCentre)
    {
        suiveurLigne_.corrigerPWM(vitesseG, vitesseD);
        estCoupureCentre = suiveurLigne_.verifierConditionDebounce(COUPURE_CENTRE);
    }
    arreter();
}

void Moteur::avancerAjustementMur()
{
    static const uint8_t REDRESSEMENT_MUR = 20;
    static const uint16_t DISTANCE_MUR_MIN = 14 * 58;
    static const uint16_t DISTANCE_MUR_MAX = 16 * 58;

    avancer();
    bool estLigne = false;
    
    while (!estLigne)
    {
        sonar_.envoyerSignal();
        sonar_.ecouterEcho();

        if (sonar_.wavelength > DISTANCE_MUR_MAX)
        {
            del_.allumerDEL(ROUGE);
            OCR1A = vitesseD + REDRESSEMENT_MUR;
            OCR1B = vitesseG - REDRESSEMENT_MUR; 
        }
        else if (sonar_.wavelength < DISTANCE_MUR_MIN)
        {
            del_.allumerDEL(ROUGE);
            OCR1A = vitesseD - REDRESSEMENT_MUR;
            OCR1B = vitesseG + REDRESSEMENT_MUR; 
        }
        else
        {
            del_.allumerDEL(VERTE);
            OCR1A = vitesseD;
            OCR1B = vitesseG;
        }

        estLigne = suiveurLigne_.verifierCondition(LIGNE_CENTREE);
        commun::delai_ms(60);
    }

    del_.allumerDEL(OFF);
    arreter();
}

void Moteur::avancerAjustementCouloir()
{
    avancer();
    bool estFini = false;
    
    while (!estFini)
    {
        estFini = suiveurLigne_.corrigerPWMCouloir(vitesseG, vitesseD);
    }
    arreter();
}

void Moteur::reculer()
{
    PORTD |= (1 << PD7) | (1 << PD6);
    ajusterPWM(vitesseG, vitesseD);
}

void Moteur::arreter()
{
    OCR1A = 0;
    OCR1B = 0;
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
}

void Moteur::ajusterPWM(uint8_t roueG, uint8_t roueD)
{
    OCR1A = roueD;
    OCR1B = roueG;

    TCCR1A |= (1 << COM1A1) | (1 << WGM10) | (1 << COM1B1);
    TCCR1B |= (1 << CS11);
    TCCR1C = 0;
}

void Moteur::rotationHoraire(uint8_t vitesse)
{
    PORTD &= ~(1 << PD6);
    PORTD |= (1 << PD7);
    ajusterPWM(0.6 * 0xFF, 0.6 * 0xFF); // ceci est pourcontrer la friction
    commun::delai_ms(5);
    ajusterPWM(vitesse, vitesse);
}

void Moteur::rotationAntiHoraire(uint8_t vitesse)
{
    PORTD |= (1 << PD6);
    PORTD &= ~(1 << PD7);
    ajusterPWM(0.6 * 0xFF, 0.6 * 0xFF); // ceci est pourcontrer la friction
    commun::delai_ms(5);
    ajusterPWM(vitesse, vitesse);
}