/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Classe qui traite les donnees du suiveur de ligne. Elle s'occupe de corriger le PWM des moteurs
 * et d'avertir le moteur des differents etats du robots sur les rubans.
 * 
*/
#include "SuiveurLigneCytron_LSS05.h"
#include "debug.h"
SuiveurLigneCytron_LSS05::SuiveurLigneCytron_LSS05()
{
    // PINA2 à PINA6 en entrée
    DDRA &= ~(1 << PA2) & ~(1 << PA3) & ~(1 << PA4) & ~(1 << PA5) & ~(1 << PA6);
}

void SuiveurLigneCytron_LSS05::corrigerPWM(uint8_t vitesseG, uint8_t vitesseD)
{
    NIVEAU_DEVIATION niveau = verifierNiveauDeviation_();
    //Valeurs trouvees par essais et erreurs pour redresser le robot
    static const uint8_t REDRESSEMENT_LV4 = 33;
    static const uint8_t REDRESSEMENT_LV3 = 30;
    static const uint8_t REDRESSEMENT_LV2 = 25;
    static const uint8_t REDRESSEMENT_LV1 = 20;

    switch (niveau)
    {
    case TROP_A_GAUCHE_LV4:
        OCR1A = vitesseD + REDRESSEMENT_LV4; // diminue roueD
        OCR1B = vitesseG - REDRESSEMENT_LV4; // augmente roueG
        break;
    case TROP_A_GAUCHE_LV3:

        OCR1A = vitesseD + REDRESSEMENT_LV3; // diminue roueD
        OCR1B = vitesseG - REDRESSEMENT_LV3; // augmente roueG
        break;
    case TROP_A_GAUCHE_LV2:
        OCR1A = vitesseD + REDRESSEMENT_LV2; // diminue roueD
        OCR1B = vitesseG - REDRESSEMENT_LV2; // augmente roueG
        break;
    case TROP_A_GAUCHE_LV1:
        OCR1A = vitesseD + REDRESSEMENT_LV1; // diminue roueD
        OCR1B = vitesseG - REDRESSEMENT_LV1; // augmente roueG
        break;
    case CENTREE:
        OCR1A = vitesseD;
        OCR1B = vitesseG;
        break;
    case TROP_A_DROITE_LV1:
        OCR1A = vitesseD - REDRESSEMENT_LV1; // augmente roueD
        OCR1B = vitesseG + REDRESSEMENT_LV1; // diminue roueG
        break;
    case TROP_A_DROITE_LV2:
        OCR1A = vitesseD - REDRESSEMENT_LV2; // augmente roueD
        OCR1B = vitesseG + REDRESSEMENT_LV2; // diminue roueG
        break;
    case TROP_A_DROITE_LV3:
        OCR1A = vitesseD - REDRESSEMENT_LV3; // augmente roueD
        OCR1B = vitesseG + REDRESSEMENT_LV3; // diminue roueG
        break;
    case TROP_A_DROITE_LV4:
        OCR1A = vitesseD - REDRESSEMENT_LV4; // augmente roueD
        OCR1B = vitesseG + REDRESSEMENT_LV4; // diminue roueG
        break;
    default:
        break;
    }
}

bool SuiveurLigneCytron_LSS05::corrigerPWMCouloir(uint8_t vitesseG, uint8_t vitesseD)
{
    static uint32_t dureeRedressement = 0;
    static uint32_t ancienRedressement = 0;
    static bool rebondirVersG = true;
    static bool rebondirVersD = false;
    static const uint8_t REDRESSEMENT = 25;
    static const uint8_t REDRESSEMENT_D = 35;
    static const uint8_t REDRESSEMENT_G = 30;

    if (verifierCondition(EXTREMITE_G) && rebondirVersD)
    {
        OCR1A = vitesseD + REDRESSEMENT;
        OCR1B = vitesseG - REDRESSEMENT;
        while (!verifierCondition(LIGNE_CENTREE))
        {
        }
        return true;
    }
    if (verifierCondition(EXTREMITE_D) && rebondirVersG)
    {
        OCR1A = vitesseD - REDRESSEMENT;
        OCR1B = vitesseG + REDRESSEMENT;
        while (!verifierCondition(LIGNE_CENTREE))
        {
        }
        return true;
    }
    if (verifierCondition(LIGNE_CENTREE))
        return true;

    dureeRedressement += 1;

    if (verifierCondition(EXTREMITE_G) && rebondirVersG)
    {
        if (dureeRedressement <= ancienRedressement / 4)
        {
            while (!verifierCondition(LIGNE_CENTREE))
            {
            }
            return true;
        }

        PORTD &= ~(1 << PD6);
        PORTD |= (1 << PD7);
        OCR1A = vitesseD - REDRESSEMENT;
        OCR1B = vitesseG + REDRESSEMENT_G;

        while (verifierCondition(EXTREMITE_G))
        {
        }

        PORTD &= ~(1 << PD7);
        rebondirVersG = false;
        rebondirVersD = true;
        ancienRedressement = dureeRedressement;
        dureeRedressement = 0;
    }
    if (verifierCondition(EXTREMITE_D) && rebondirVersD)
    {
        if (dureeRedressement <= ancienRedressement / 4)
        {
            while (!verifierCondition(LIGNE_CENTREE))
            {
            }
            return true;
        }

        PORTD |= (1 << PD6);
        PORTD &= ~(1 << PD7);

        OCR1A = vitesseD + REDRESSEMENT_D;
        OCR1B = vitesseG - REDRESSEMENT;

        while (verifierCondition(EXTREMITE_D))
        {
        }

        PORTD &= ~(1 << PD6);

        rebondirVersG = true;
        rebondirVersD = false;
        ancienRedressement = dureeRedressement;
        dureeRedressement = 0;
    }

    // OCR1A = vitesseD;
    // OCR1B = vitesseG;
    return false;
}

bool SuiveurLigneCytron_LSS05::verifierCondition(TYPE_DETECTION condition)
{
    switch (condition)
    {
    case LIGNE:
        if (((PINA & (1 << PA6)) || (PINA & (1 << PA5)) || (PINA & (1 << PA4)) || (PINA & (1 << PA3)) || (PINA & (1 << PA2))))
            return true;
        else
            return false;
        break;
    case COUPURE:
        if (!((PINA & (1 << PA6)) || (PINA & (1 << PA5)) || (PINA & (1 << PA4)) || (PINA & (1 << PA3)) || (PINA & (1 << PA2))))
            return true;
        else
            return false;
        break;
    case LIGNE_CENTREE:
        if ((PINA & (1 << PA4)))
            return true;
        else
            return false;
        break;
    case COUPURE_CENTRE:
        if (!((PINA & (1 << PA3)) || (PINA & (1 << PA4)) || (PINA & (1 << PA5))))
            return true;
        else
            return false;
        break;
    case ANGLE_DROIT:
        if (((PINA & (1 << PA2)) && (PINA & (1 << PA3)) && (PINA & (1 << PA4))) || ((PINA & (1 << PA6)) && (PINA & (1 << PA5)) && (PINA & (1 << PA4))))
            return true;
        else
            return false;
        break;
    case ANGLE_DROIT_COTE_G:
        if ((PINA & (1 << PA2)) && (PINA & (1 << PA3)) && (PINA & (1 << PA4)))
            return true;
        else
            return false;
        break;
    case ANGLE_DROIT_COTE_D:
        if ((PINA & (1 << PA6)) && (PINA & (1 << PA5)) && (PINA & (1 << PA4)))
            return true;
        else
            return false;
        break;

    case EXTREMITE_G:
        if ((PINA & (1 << PA2)))
            return true;
        else
            return false;
        break;
    case EXTREMITE_D:
        if ((PINA & (1 << PA6)))
            return true;
        else
            return false;
        break;
    default:
        return false;
        break;
    }
}

bool SuiveurLigneCytron_LSS05::verifierConditionDebounce(TYPE_DETECTION condition, uint16_t duree_ms)
{
    if (verifierCondition(condition))
    {
        commun::delai_ms(duree_ms);
        if (verifierCondition(condition))
        {
            return true;
        }
    }
    return false;
}

NIVEAU_DEVIATION SuiveurLigneCytron_LSS05::verifierNiveauDeviation_()
{
    uint8_t etatSL = PINA & 0b01111100; // prend les 5 bits de données du suiveur de ligne

    if (!(etatSL & (1 << PA3)) && (etatSL & (1 << PA2)))
        return TROP_A_GAUCHE_LV4; // x x x 0 1

    else if ((etatSL & (1 << PA6)) && !(etatSL & (1 << PA5)))
        return TROP_A_DROITE_LV4; // 1 0 x x x

    else if ((etatSL & (1 << PA3)) && (etatSL & (1 << PA2)))
        return TROP_A_GAUCHE_LV3; // x x x 1 1

    else if ((etatSL & (1 << PA6)) && (etatSL & (1 << PA5)))
        return TROP_A_DROITE_LV3; // 1 1 x x x

    else if ((etatSL & (1 << PA4)) && (etatSL & (1 << PA3)))
        return TROP_A_GAUCHE_LV1; // x x 1 1 x

    else if ((etatSL & (1 << PA5)) && (etatSL & (1 << PA4)))
        return TROP_A_DROITE_LV1; // x 1 1 x x

    else if ((etatSL & (1 << PA3)) && !(etatSL & (1 << PA2)))
        return TROP_A_GAUCHE_LV2; // x x x 1 0

    else if (!(etatSL & (1 << PA6)) && (etatSL & (1 << PA5)))
        return TROP_A_DROITE_LV2; // 0 1 x x x

    else if ((etatSL & (1 << PA2) && (etatSL & (1 << PA6))))
        return EXTREMITES; // 1 x x x 1 Cas special pour le couloir

    return CENTREE; // x x x x x
}