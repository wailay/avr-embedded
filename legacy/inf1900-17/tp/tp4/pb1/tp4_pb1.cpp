//Aymeric Labrie 1995467

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

typedef unsigned char uint8_t ;
enum ETAT {INIT, ETAT1, ETAT2};
volatile ETAT etat;

void initialisation(){
    cli();
    etat = INIT;
    DDRA = 0xff; // PORT A est en mode sortie
    DDRB = 0xff; // PORT B est en mode sortie
    DDRC = 0xff; // PORT C est en mode sortie
    DDRD = 0xfb; // PORT D1, D3 et D4 en mode sortie. PORT D2 est en mode entree.
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);
    sei();
}

void allumerDEL(){
    PORTA = 0x02;
    _delay_ms(3000);
    PORTA = 0x00;
}



void changerEtat(){
    switch(etat){
        case INIT:
            break;
        case ETAT1:
            etat = INIT;
            break;
        default:
            etat = INIT;
            break;
    }
}

void effectuerEtat(){
    switch(etat){
        case INIT:
            break;
        case ETAT1:
            allumerDEL();
            break;
        default:
            break;
    }
}

ISR(INT0_vect){
    _delay_ms(30);
    if(PIND & 0x04){
        etat = ETAT1;
        effectuerEtat();
    }
}

int main(){
    initialisation();
    while (true) {
        changerEtat();
        _delay_ms(100); // faire perdre du temps au microcontrôleur
    }
}
