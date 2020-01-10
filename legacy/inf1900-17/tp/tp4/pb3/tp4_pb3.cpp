//Aymeric Labrie 1995467

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
static const uint8_t PWM_100 = 0x00;
static const uint8_t PWM_75 = 0xFF*0.25;
static const uint8_t PWM_50 = 0xFF*0.50;
static const uint8_t PWM_25 = 0xFF*0.75;
static const uint8_t PWM_0 = 0xFF;

void initialisation(){
    cli();
    DDRA = 0xff; // PORT A est en mode sortie
    DDRB = 0xff; // PORT B est en mode sortie
    DDRC = 0xff; // PORT C est en mode sortie
    DDRD = 0xff; // PORT D est en mode sortie
    sei();
}

void ajustementPWM (uint8_t pwm) {
    // mise à un des sorties OC1A et OC1B sur comparaison
    // réussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6
    // page 177 de la description technique du ATmega324PA)
    //PWM, Phase Correct, 8-bit
    uint8_t sreg = SREG;
    cli();
    OCR1A = pwm;
    OCR1B = pwm;
    // division d'horloge par 8 - implique une frequence de PWM fixe
    TCCR1A |= (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0) | (1 << WGM10); //Set OCnA/OCnB on Compare Match (Set output to high level).
    TCCR1B |= (1 << CS11); // division d'horloge par 8 - implique une frequence de PWM fixe
    TCCR1C = 0;
    SREG = sreg;
}

int main(){
    initialisation();
    ajustementPWM(PWM_75);
}