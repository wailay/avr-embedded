/*
 * Andrew Abdo et Wail Ayad 
 * Numero equipe : 12
 * Description: Probleme 3 du TP4 
 * 
*/

#define F_CPU 8000000
#include <util/delay.h>

#include <avr/interrupt.h>
#include <avr/io.h>


void init(){
    // cli();

    DDRD |= (1 << PD4) | (1 << PD5) | ( 1 << PD2) | (1 << PD3); //mettre les pins associe a OC1A et OC1B en sortie

    // sei();
}
void ajustementPWM (uint8_t ratioRoue1, uint8_t ratioRoue2) {

// mise à un des sorties OC1A et OC1B sur comparaison
// réussie en mode PWM 8 bits, phase correcte
// et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6
// page 177 de la description technique du ATmega324PA)

OCR1A = 0xFF * (100 - ratioRoue1) / 100;
OCR1B = 0xFF * (100 - ratioRoue2) / 100;


// division d'horloge par 8 - implique une frequence de PWM fixe

//TCCR Timer Control Register for A
//COM1A, Compare output mode for A, dicte comment les registres OC1A et OC1B vont agir
//Dans ce cas OC1A et OC1B vont set lors dun upcount et clear lors dun downcount. Phase correct PWM. (COM1A1 et COM1A0 et COM1B1 et COM1B0)
//WGM10 -> Mode 1 -> Phase correct PWM  8 bits avec 0xFF pour TOP
TCCR1A |= (1 << COM1A1) | (1 << COM1A0) | (1 << WGM10) | (1 << COM1B1) | (1 << COM1B0);

//Prescaling de 8, 
TCCR1B |= (1 << CS11) ;

TCCR1C = 0;

}


int main(){
    init();

    // MAX TOP = 0xFF = 255
    //0 %
    ajustementPWM(0, 0);
    _delay_ms(2000);


    //25%
    ajustementPWM(25, 25);
    _delay_ms(2000);

    //50%
    ajustementPWM(50, 50);
    _delay_ms(2000);

    //75%
    ajustementPWM(75, 75);
    _delay_ms(2000);
    
    //100%
    ajustementPWM(100, 100);
    _delay_ms(2000);


    //arret 
    ajustementPWM(0, 0);
    return 0;
    
}
