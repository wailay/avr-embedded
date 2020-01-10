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
enum ETAT : uint8_t {INIT, ETAT2, ETAT3};
enum COULER : uint8_t {FERMEE, VERT, ROUGE};
volatile uint8_t compteur;

void initialisation(){
    cli();
    DDRA = 0x00; // PORT A est en mode entree
    DDRB = 0xff; // PORT B est en mode sortie
    DDRC = 0xff; // PORT C est en mode sortie
    DDRD = 0xff; // PORT D est en mode sortie
    sei();
}

void partirMinuterie(uint16_t duree_ms) {
    // mode CTC du timer 1 avec horloge divisée par 1024
    // interruption après la durée spécifiée
    TCCR1A |= (1 << COM1A1); //Clear OCnA/OCnB on Compare Match (Set output tolow level).
    TCCR1B |= (1 << CS02) | (0 << CS01) | (1 << CS00) | (1 << WGM12);
    TIMSK1 |= (1 << OCIE1A); //Timer/Counter1, Output Compare A Match Interrupt Enable
    TCNT1 = 0;
    TCCR1C = 0;
    OCR1A = (duree_ms/1000.0)*F_CPU/1024.0;
}

void arreterMinuterie(){
    TCCR1B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
}

void modifierDEL(COULER couleur){
    switch(couleur){
        case FERMEE:
            PORTB = 0x00;
            break;
        case VERT:
            PORTB = 0x01;
            break;
        case ROUGE:
            PORTB = 0x02;
            break;
        default:
            break;
    }
}

bool bouttonAppuye(){
    if((PINA & 0x01) == 0){
        _delay_ms(30);
        if((PINA & 0x01) == 0)
            return true;
        else
            return false;
    }
    return false;
}

void changerEtat(ETAT& etat){
    switch(etat){
        case INIT:
            if(bouttonAppuye())
                etat = ETAT2;
            break;
        case ETAT2:
            if(compteur >= 120 || !bouttonAppuye())
                etat = ETAT3;
            break;
        case ETAT3:
            etat = INIT;
            break;
        default:
            etat = INIT;
            break;
    }
}

void effectuerEtat(const ETAT& etat, bool& minuterieOn){
    switch(etat){
        case INIT:
            compteur = 0;
            minuterieOn = false;
        case ETAT2:
            if(!minuterieOn){
                partirMinuterie(100); // Augmente le compteur de 1 au 100 ms.
                minuterieOn = true;
            }
            break;
        case ETAT3:
            arreterMinuterie();
            modifierDEL(VERT);
            _delay_ms(500);
            modifierDEL(FERMEE);
            _delay_ms(2000);
            for (uint8_t i = 0; i < compteur; i++){
                modifierDEL(ROUGE);
                _delay_ms(400);
                modifierDEL(FERMEE);
                _delay_ms(100);
            }
            modifierDEL(VERT);
            _delay_ms(1000);
            modifierDEL(FERMEE);
            break;
        default:
            break;
    }
}

ISR(TIMER1_COMPA_vect){
    compteur++;
}

int main(){
    ETAT etat = INIT;
    bool minuterieOn = false;
    initialisation();
    while (true){
        effectuerEtat(etat, minuterieOn);
        changerEtat(etat);    
    }
}
