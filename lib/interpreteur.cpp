/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description : Interpreteur du TP9
 * 
 */

#include "interpreteur.h"

Interpreteur::Interpreteur()
{
    isTerminated_ = false;
}

uint8_t Interpreteur::getPC()
{
    return programCounter_;
}

void Interpreteur::setProgramCounter(uint8_t programCounter)
{
    programCounter_ = programCounter;
}

void Interpreteur::parse_bytcode(uint8_t byte_code[])
{
    uint8_t operande, code;
    //Le code se trouve dans les 8 bits les plus hauts
    code = byte_code[0];
    //L'operande se trouve dans les 8 premier bits.
    operande = byte_code[1];
    executerByteCode(code, operande);
    programCounter_ += 2;
}

void Interpreteur::executerByteCode(uint8_t code, uint8_t operande)
{
    switch (code)
    {
    //Si pour une raison il ya encore le opcode dbt alors qu'on a deja debuter le programme, ne rien faire et continuer
    case dbt_bc: // Routine de début
        ;        //ne rien faire
        break;
    case att_bc:
        //Petit probleme ici, semple plutot lent pour un delai de 25ms
        commun::delai_ms(operande * 25);
        break;
    //Allumer la LED, ce code est juste la pour tester la boucle, il reste a implementer le fait
    //qu'on puisse allumer jusqua 8 DEL. Le prof a dit en cours aujourdhui juste allumer une DEL pas besoin de faire 8 peu importe loperande.
    case dal_bc:
        del_.allumerDEL(ROUGE);
        break;
    case det_bc: //Eteindre la LED
        del_.allumerDEL(OFF);
        break;
    case sgo_bc:
        playNote(operande);
        break;
    case sar_bc:
        stopNote();
        break;
    case mar_bc:
    case mar2_bc:
        moteurs_.arreter();
        break;
    case mav_bc:
        moteurs_.avancer(operande);
        break;
    case mre_bc:
        moteurs_.reculer(operande);
        break;
    case trd_bc:
        moteurs_.tournerProchaineCondition(SENS_ANTIHORAIRE);
        break;
    case trg_bc:
        moteurs_.tournerProchaineCondition(SENS_HORAIRE);
        break;
    case dbc_bc: //Debut de la loop
        //save the loop counter
        compteurBoucle_ = operande + 1;
        debutBoucle_ = programCounter_;
        break;
    case fbc_bc: //Fin de la loop
        compteurBoucle_--;
        if (compteurBoucle_ != 0)
            programCounter_ = debutBoucle_;
        break;
    case fin_bc:
        isTerminated_ = true;
        break;
    }
}
bool Interpreteur::terminated()
{
    return isTerminated_;
}