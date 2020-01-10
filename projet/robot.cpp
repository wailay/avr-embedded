/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Classe robot qui englobe la machine a etats qui execute les quatre sections.
 * 
 * Diagramme : 
 * 
 *                 | Entree|                  
 * | Etats Present |   SInit   |  Etats futurs  |   
 * | ------------- | --------- | -------------  |
 * |    COULOIR    |     X     |      MUR       |      
 * |    COULOIR    |    MUR    |      FIN       |  
 * |    MUR        |     X     |    BOUCLES     | 
 * |    MUR        |  BOUCLES  |     FIN        |  
 * |    BOUCLES    |     X     |   COUPURES     |  
 * |    BOUCLES    |  COUPURES |     FIN        |       
 * |    COUPURES   |    X      |   COULOIR      |
 * |    COUPURES   |  COULOIR  |     FIN        | 
 * |    FIN        |    X      |     FIN        |   
 * 
*/

#include "robot.h"

Robot::Robot()
{
    section_ = COULOIR; //par defaut la section initial a afficher dans le display est LE COULOIR
    estTerminee_ = false;
    display_.ecrire("LE COULOIR");
}

void Robot::setSectionInitial(Section section)
{
    sectionInitial_ = section;
    section_ = section;
}

void Robot::afficherSection()
{
    switch (section_)
    {
    case COULOIR: // affiche « le couloir »
        display_.ecrire("LE COULOIR");
        break;
    case MUR: // affiche « le mur »
        display_.ecrire("LE MUR");
        break;
    case BOUCLES: // affiche « les deux boucles »
        display_.ecrire("LES DEUX BOUCLES");
        break;
    case COUPURES: // affiche « les coupures »
        display_.ecrire("LES COUPURES");
        break;
    case FIN: // affiche « fin »
        display_.ecrire("FIN");
        break;
    }
}

void Robot::effectuerRoutineDebut()
{
    commun::delai_ms(2000);
}

void Robot::changerEtat()
{
    switch (section_)
    {
    case COULOIR:
        if (sectionInitial_ != MUR)
            section_ = MUR;
        else
            section_ = FIN;
        break;
    case MUR:
        if (sectionInitial_ != BOUCLES)
            section_ = BOUCLES;
        else
            section_ = FIN;
        break;
    case BOUCLES:
        if (sectionInitial_ != COUPURES)
            section_ = COUPURES;
        else
            section_ = FIN;
        break;
    case COUPURES:
        if (sectionInitial_ != COULOIR)
            section_ = COULOIR;
        else
            section_ = FIN;
        break;
    case FIN:
        break;
    default:
        break;
    }
}

void Robot::changerEtat(Section section)
{
    section_ = section;
}

void Robot::effectuerEtat()
{
    switch (section_)
    {
    case COULOIR:
        effectuerCouloir_();
        effectuerTransition_(COULOIR_MUR);
        break;
    case MUR:
        effectuerMur_();
        effectuerTransition_(MUR_BOUCLES);
        break;
    case BOUCLES:
        effectuerBoucles_();
        effectuerTransition_(BOUCLES_COUPURES);
        break;
    case COUPURES:
        effectuerCoupures_();
        effectuerTransition_(COUPURES_COULOIR);
        break;
    case FIN:
        effectuerFin_();
        break;
    default:
        break;
    }
}

bool Robot::estTerminee()
{
    return estTerminee_;
}

void Robot::effectuerCouloir_()
{
    // Position A direction AB
    moteur_.avancerBoost();
    // Position A direction AB
    moteur_.avancerProchaineCoupureCentre();
    // Position B | Bondir entre les segments CD et EF
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, EXTREMITE_G);
    moteur_.avancerAjustementCouloir();
    // Position G direction GH
    moteur_.avancerProchainAngleDroitCoupure();
    moteur_.arreter();
    // Position H direction GH
}

void Robot::effectuerMur_()
{
    moteur_.avancerBoost();
    // Position H direction HI
    moteur_.avancerProchaineCoupure();
    //Suivre le mur
    moteur_.avancerAjustementMur();
    commun::delai_ms(200);
    // Segment JK-KL
    //suit la ligne un peu sans detecter dangle droit, pendant environ 1.5 sec
    moteur_.avancerProchaineCoupureSansAjustement();
    moteur_.tournerProchaineCondition(SENS_HORAIRE, LIGNE_CENTREE);
    moteur_.avancerProchainAngleDroitCoupure();
    moteur_.arreter();
}

void Robot::effectuerBoucles_()
{
    static const uint16_t tempsAngleSorti = 500;
    moteur_.avancerBoost();
    // Position L direction LM
    // Segment LM
    moteur_.avancerLigne(1500);
    moteur_.avancerProchainAngleDroit();
    // Segment MN
    moteur_.avancerProchainAngleDroit();
    // Segment NO
    moteur_.avancerProchainAngleDroit(500);
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, COUPURE);
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, LIGNE_CENTREE);
    // Segment OP
    moteur_.avancerLigne(2000);
    moteur_.avancerProchainAngleDroitCoupure(tempsAngleSorti);
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, LIGNE_CENTREE);
    // Segment PQ
    moteur_.avancerLigne(2000);
    moteur_.avancerProchainAngleDroitCoupure(tempsAngleSorti);
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, LIGNE_CENTREE);
    // Segment QRM
    moteur_.avancerLigne(2000);
    moteur_.avancerProchainAngleDroitCoupure(tempsAngleSorti);
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, LIGNE_CENTREE);
    // Segment MN /*petite boucle */
    moteur_.avancerLigne(2000);
    moteur_.avancerProchainAngleDroitCoupure();
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, COUPURE);
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, EXTREMITE_G);
    // Segment NS
    moteur_.avancerLigne(2000);
    moteur_.avancerProchainAngleDroitCoupure();
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, EXTREMITE_G);
    // Segment SR
    moteur_.avancerLigne(2000);
    moteur_.avancerProchainAngleDroitCoupure(tempsAngleSorti);
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, LIGNE_CENTREE);
    // Segment RM
    moteur_.avancerLigne(2000);
    moteur_.avancerProchainAngleDroitCoupure(tempsAngleSorti);
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, LIGNE_CENTREE);
    // Segment MN
    moteur_.avancerLigne(2000);
    moteur_.avancerProchainAngleDroit();
    // Segment NO
    moteur_.avancerProchainAngleDroit();
    // Segment OT
    moteur_.avancerProchainAngleDroitCoupure();
    // Position T direction OT
    moteur_.arreter();
}

void Robot::effectuerCoupures_()
{
    static const uint16_t tempsAttente = 50;
    static const uint8_t vitesseRotation = 0.4 * 0.9 * 0xFF;
    moteur_.avancerBoost();
    // Position T direction TU
    // Segment TUV
    moteur_.avancerProchaineCoupure();
    // commun::delai_ms(500);
    // Jouer un son aigu a la coupure V
    playNote(81);
    // Segment VW
    moteur_.tournerProchaineCondition(SENS_HORAIRE, LIGNE_CENTREE, vitesseRotation);
    _delay_ms(tempsAttente);
    stopNote();
    // Segement WXY
    moteur_.avancerProchaineCoupure();
    // commun::delai_ms(500);
    //Jouer un son grave a la coupure Y
    playNote(45);
    // Segment YZ
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, LIGNE_CENTREE, vitesseRotation);
    _delay_ms(tempsAttente);
    stopNote();
    // Segment Z12
    moteur_.avancerProchaineCoupure();
    // commun::delai_ms(500);
    //Jouer un son aigu a la coupure 2
    playNote(81);
    // Segment 23
    moteur_.tournerProchaineCondition(SENS_HORAIRE, LIGNE_CENTREE, vitesseRotation);
    _delay_ms(tempsAttente);
    stopNote();
    // Segment 345
    moteur_.avancerProchaineCoupure();
    // commun::delai_ms(500);
    //Jouer un son grave a la coupure 5
    playNote(45);
    // Segment 56
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, LIGNE_CENTREE, vitesseRotation);
    _delay_ms(tempsAttente);
    stopNote();

    // Segment 6A | avant de commencer a detecter langle droit, dire au robot de suivre la ligne
    // pendant 1 seconde avant de detecter langle droit, des fois en reprenant le suivi de ligne, le robot pense que cest un angle droit.
    moteur_.avancerLigne(2000);
    //s'arreter a l'angle droit
    moteur_.avancerProchainAngleDroit();
    moteur_.arreter();
}

void Robot::effectuerFin_()
{
    estTerminee_ = true;
    moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, LIGNE_CENTREE);
}

void Robot::effectuerTransition_(Transition transition)
{
    switch (transition)
    {
    case COULOIR_MUR:
        if (sectionInitial_ != MUR)
            moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, LIGNE_CENTREE);
        break;
    case MUR_BOUCLES:
        if (sectionInitial_ != BOUCLES)
            moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, LIGNE_CENTREE);
        break;
    case BOUCLES_COUPURES:
        if (sectionInitial_ != COUPURES)
            moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, LIGNE_CENTREE);
    case COUPURES_COULOIR:
        if (sectionInitial_ != COULOIR)
            moteur_.tournerProchaineCondition(SENS_ANTIHORAIRE, LIGNE_CENTREE);
        break;
    default:
        break;
    }
}