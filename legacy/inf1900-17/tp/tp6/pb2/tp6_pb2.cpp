//Aymeric Labrie 1995467
//Youva Boutora 1986737
/*
_____________________________________________
ETAT PRESENT | ETAT SUIVANT |
=============================================
INIT            ETAT2       
ETAT2           ETAT3
ETAT3           INIT
=============================================
*/
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "can.h"

enum ETAT : uint8_t {INIT, ETAT_VERT, ETAT_AMBRE, ETAT_ROUGE};
enum COULER : uint8_t {FERMEE, VERT, AMBRE, ROUGE};
static const double AREF = 4.3;
static const double AREF = 4.3;
void initialisation(){
    uint8_t sreg = SREG;
    cli();
    DDRA = 0x00; // PORT A est en mode entree
    DDRB = 0xff; // PORT B est en mode sortie
    DDRC = 0xff; // PORT C est en mode sortie
    DDRD = 0xff; // PORT D est en mode sortie
    SREG = sreg; // reactive interupt enable s'il etait active
}

void modifierDEL(COULER couleur){
    switch(couleur){
        case FERMEE:
            PORTB = 0x00;
            break;
        case VERT:
            PORTB = 0x01;
            break;
        case AMBRE:
            PORTB = 0x01;
            _delay_ms(1);
            PORTB = 0x02;            
        case ROUGE:
            PORTB = 0x02;
            break;
        default:
            break;
    }
}

void changerEtat(ETAT& etat, can& can, const uint8_t& pos){
    //4.3 avec cellulaire 10cm 3.6 avec lumiere ambiante 2.7 avec main 10 cm au dessus
    uint16_t adcVal = can.lecture(pos);
    if(adcVal > 0x3FF*(4.0/AREF)) // 0x3FF est la valeur maximale de adcVal sur 10 bits
        etat = ETAT_ROUGE;
    else if(adcVal > 0x3FF*(3.1/AREF))
        etat = ETAT_AMBRE;
    else
        etat = ETAT_VERT;
}

void effectuerEtat(const ETAT& etat){
    switch(etat){
        case INIT:
            break;
        case ETAT_VERT:
            modifierDEL(VERT);
            break;
        case ETAT_AMBRE:
            modifierDEL(AMBRE);
            break;
        case ETAT_ROUGE:
            modifierDEL(ROUGE);
            break;
        default:
            break;
    }
}

int main(){
    ETAT etat = INIT;
    can can;
    uint8_t posPinADC0 = 0;
    initialisation();
    while (true){
        effectuerEtat(etat);
        changerEtat(etat, can, posPinADC0);    
    }
}